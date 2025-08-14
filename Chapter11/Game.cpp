#include "Game.h"

#include "Renderer.h"
#include "AudioSystem.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "AudioComponent.h"
#include "FPSActor.h"
#include "PhysWorld.h"
#include "TargetActor.h"
#include "Font.h"
#include "UIScreen.h"
#include "PauseMenu.h"
#include "HUD.h"

Game::Game()
	: mActors()
	, mPendingActors()
	, mUIStack()
	, mFonts()
	, mpRenderer(nullptr)
	, mpAudioSystem(nullptr)
	, mpPhysWorld(nullptr)
	, mpHUD(nullptr)
	, mTicksCount(0)
	, mbUpdatingActors(false)
	, mGameState(EGameState::GAME_PLAY)
	, mMusicEvent()
	, mReverbSnap()
	, mpFPSActor(nullptr)
	, mpCrosshair(nullptr)
{
}

Game::~Game()
{

}

bool Game::Initialize()
{
	bool bResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (!bResult)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());

		return false;
	}

	mpRenderer = new Renderer(this);
	if (!mpRenderer->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		SDL_Log("Failed to initialize renderer");

		delete mpRenderer;
		mpRenderer = nullptr;

		return false;
	}

	mpAudioSystem = new AudioSystem(this);
	if (!mpAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mpAudioSystem->Shutdown();

		delete mpAudioSystem;
		mpAudioSystem = nullptr;

		return false;
	}

	mpPhysWorld = new PhysWorld(this);

	bResult = TTF_Init();
	if (!bResult)
	{
		SDL_Log("Failed to initialize SDL_ttf");

		return false;
	}

	// LoadData
	{
		LoadText("Assets/English.gptext");

		Actor* a = nullptr;
		Quaternion q;

		const float start = -1250.0f;
		const float size = 250.0f;
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				a = new PlaneActor(this);
				a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
			}
		}

		q = Quaternion(Vector3::UnitX, Math::PI_OVER_2);
		for (int i = 0; i < 10; i++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
			a->SetRotation(q);

			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
			a->SetRotation(q);
		}

		q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PI_OVER_2));

		for (int i = 0; i < 10; i++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
			a->SetRotation(q);

			a = new PlaneActor(this);
			a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
			a->SetRotation(q);
		}

		mpRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
		DirectionalLight& dir = mpRenderer->GetDirectionalLight();
		dir.direction = Vector3(0.0f, -0.707f, -0.707f);
		dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
		dir.specColor = Vector3(0.8f, 0.8f, 0.8f);

		mpHUD = new HUD(this);

		mMusicEvent = mpAudioSystem->PlayEvent("event:/Music");

		mpFPSActor = new FPSActor(this);

		a = new TargetActor(this);
		a->SetPosition(Vector3(1450.0f, 0.0f, 100.0f));
		a = new TargetActor(this);
		a->SetPosition(Vector3(1450.0f, 0.0f, 400.0f));
		a = new TargetActor(this);
		a->SetPosition(Vector3(1450.0f, -500.0f, 200.0f));
		a = new TargetActor(this);
		a->SetPosition(Vector3(1450.0f, 500.0f, 200.0f));
	}

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (mGameState != EGameState::QUIT)
	{
		processInput();
		updateGame();
		generateOutput();
	}
}

void Game::Shutdown()
{
	// unload data
	{
		while (!mActors.empty())
		{
			delete mActors.back();
		}

		while (!mUIStack.empty())
		{
			delete mUIStack.back();

			mUIStack.pop_back();
		}
	}

	delete mpPhysWorld;

	if (mpRenderer != nullptr)
	{
		mpRenderer->UnloadData();
		mpRenderer->Shutdown();
	}

	if (mpAudioSystem != nullptr)
	{
		mpAudioSystem->Shutdown();
	}

	TTF_Quit();
	SDL_Quit();
}

void Game::AddActor(Actor* const pActor)
{
	if (mbUpdatingActors)
	{
		mPendingActors.push_back(pActor);
	}
	else
	{
		mActors.push_back(pActor);
	}
}

void Game::RemoveActor(Actor* const pActor)
{
	std::vector<Actor*>::iterator pendingIter = std::find(mPendingActors.begin(), mPendingActors.end(), pActor);
	if (pendingIter != mPendingActors.end())
	{
		std::iter_swap(pendingIter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	std::vector<Actor*>::iterator actorIter = std::find(mActors.begin(), mActors.end(), pActor);
	if (actorIter != mActors.end())
	{
		std::iter_swap(actorIter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::PushUI(UIScreen* const pScreen)
{
	mUIStack.push_back(pScreen);
}

Font* Game::GetFont(const std::string& fileName)
{
	Font* pFont = nullptr;

	std::unordered_map<std::string, Font*>::iterator iter = mFonts.find(fileName);
	if (iter != mFonts.end())
	{
		pFont = iter->second;
	}
	else
	{
		pFont = new Font(this);
		if (pFont->Load(fileName))
		{
			mFonts.insert({ fileName, pFont });
		}
		else
		{
			pFont->Unload();

			delete pFont;

			pFont = nullptr;
		}
	}

	return pFont;
}

void Game::LoadText(const std::string& fileName)
{
	mText.clear();

	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found", fileName.c_str());

		return;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();

	std::string contents = fileStream.str();

	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;

	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());

		return;
	}

	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin(); itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			mText.insert({ itr->name.GetString(), itr->value.GetString() });
		}
	}
}

const std::string& Game::GetText(const std::string& key)
{
	static std::string errorMsg("**KEY NOT FOUND**");

	std::unordered_map<std::string, std::string>::iterator iter = mText.find(key);
	if (iter != mText.end())
	{
		return iter->second;
	}

	return errorMsg;
}

void Game::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			mGameState = EGameState::QUIT;
			break;

		case SDL_EVENT_KEY_DOWN:
			if (!event.key.repeat)
			{
				if (mGameState == EGameState::GAME_PLAY)
				{
					handleKeyPress(event.key.key);
				}
				else if (!mUIStack.empty())
				{
					UIScreen* const pUI = mUIStack.back();

					pUI->HandleKeyPress(event.key.key);
				}
			}
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (mGameState == EGameState::GAME_PLAY)
			{
				handleKeyPress(event.button.button);
			}
			else if (!mUIStack.empty())
			{
				UIScreen* const pUI = mUIStack.back();

				pUI->HandleKeyPress(event.button.button);
			}
			break;

		default:
			break;
		}
	}

	const bool* pKeyStates = SDL_GetKeyboardState(nullptr);
	if (mGameState == EGameState::GAME_PLAY)
	{
		for (Actor* const p : mActors)
		{
			if (p->GetState() == Actor::EState::ACTIVE)
			{
				p->ProcessInput(pKeyStates);
			}
		}
	}
	else if (!mUIStack.empty())
	{
		UIScreen* const pUI = mUIStack.back();

		pUI->ProcessInput(pKeyStates);
	}
}

void Game::handleKeyPress(const int key)
{
	switch (key)
	{
	case SDLK_ESCAPE:
		new PauseMenu(this);
		break;

	case '-':
		{
			float volume = mpAudioSystem->GetBusVolume("bus:/");
			volume = Math::Max(0.f, volume - 0.1f);

			mpAudioSystem->SetBusVolume("bus:/", volume);
		}
		break;

	case '=':
		{
			float volume = mpAudioSystem->GetBusVolume("bus:/");
			volume = Math::Min(1.f, volume + 0.1f);

			mpAudioSystem->SetBusVolume("bus:/", volume);
		}
		break;

	case 'e':
		{
			mpAudioSystem->PlayEvent("event:/Explosion2D");
		}
		break;

	case 'm':
		{
			mMusicEvent.SetPaused(!mMusicEvent.GetPaused());
		}
		break;

	case 'r':
		if (!mReverbSnap.IsValid())
		{
			mReverbSnap = mpAudioSystem->PlayEvent("snapshot:/WithReverb");
		}
		else
		{
			mReverbSnap.Stop();
		}
		break;

	case SDL_BUTTON_LEFT:
		{
			mpFPSActor->Shoot();
		}
		break;

	case '1':
		{
			LoadText("Assets/English.gptext");
		}
		break;

	case '2':
		{
			LoadText("Assets/Russian.gptext");
		}
		break;

	default:
		break;
	}
}

void Game::updateGame()
{
	const uint64_t deadline = mTicksCount + 16;
	while (true)
	{
		const uint64_t ticks = SDL_GetTicks();
		if (ticks > deadline)
		{
			break;
		}
	}

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	if (mGameState == EGameState::GAME_PLAY)
	{
		mbUpdatingActors = true;
		for (Actor* const p : mActors)
		{
			p->Update(deltaTime);
		}
		mbUpdatingActors = false;

		for (Actor* const p : mPendingActors)
		{
			p->ComputeWorldTransform();
			mActors.push_back(p);
		}
		mPendingActors.clear();

		std::vector<Actor*> deadActors;
		deadActors.reserve(mActors.size() / 2);

		for (Actor* const p : mActors)
		{
			if (p->GetState() == Actor::EState::DEAD)
			{
				deadActors.push_back(p);
			}
		}

		for (Actor* const p : deadActors)
		{
			delete p;
		}
	}

	mpAudioSystem->Update(deltaTime);

	for (UIScreen* const p : mUIStack)
	{
		if (p->GetState() == UIScreen::EUIState::ACTIVE)
		{
			p->Update(deltaTime);
		}
	}

	std::vector<UIScreen*>::iterator iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == UIScreen::EUIState::CLOSING)
		{
			delete (*iter);

			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Game::generateOutput()
{
	mpRenderer->Draw();
}

void Game::AddPlane(PlaneActor* const pPlane)
{
	mPlanes.push_back(pPlane);
}

void Game::RemovePlane(PlaneActor* const pPlane)
{
	std::vector<PlaneActor*>::iterator iter = std::find(mPlanes.begin(), mPlanes.end(), pPlane);

	mPlanes.erase(iter);
}