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

Game::Game()
	: mActors()
	, mPendingActors()
	, mpRenderer(nullptr)
	, mpAudioSystem(nullptr)
	, mpPhysWorld(nullptr)
	, mTicksCount(0)
	, mbRunning(true)
	, mbUpdatingActors(false)
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

	// LoadData
	{
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

		a = new Actor(this);
		a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
		SpriteComponent* sc = new SpriteComponent(a);
		sc->SetTexture(mpRenderer->GetTexture("Assets/HealthBar.png"));

		a = new Actor(this);
		a->SetPosition(Vector3(-390.0f, 275.0f, 0.0f));
		a->SetScale(0.75f);
		sc = new SpriteComponent(a);
		sc->SetTexture(mpRenderer->GetTexture("Assets/Radar.png"));

		a = new Actor(this);
		a->SetScale(2.0f);
		mpCrosshair = new SpriteComponent(a);
		mpCrosshair->SetTexture(mpRenderer->GetTexture("Assets/Crosshair.png"));

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
	while (mbRunning)
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
	}

	if (mpRenderer != nullptr)
	{
		mpRenderer->UnloadData();
		mpRenderer->Shutdown();
	}

	if (mpAudioSystem != nullptr)
	{
		mpAudioSystem->Shutdown();
	}

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

void Game::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			mbRunning = false;
			break;

		case SDL_EVENT_KEY_DOWN:
			if (!event.key.repeat)
			{
				handleKeyPress(event.key.key);
			}
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			handleKeyPress(event.button.button);
			break;

		default:
			break;
		}
	}

	const bool* pKeyStates = SDL_GetKeyboardState(nullptr);
	if (pKeyStates[SDL_SCANCODE_ESCAPE])
	{
		mbRunning = false;
	}

	for (Actor* const p : mActors)
	{
		p->ProcessInput(pKeyStates);
	}
}

void Game::handleKeyPress(const int key)
{
	switch (key)
	{
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

	mpAudioSystem->Update(deltaTime);
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