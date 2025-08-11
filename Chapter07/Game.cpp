#include "Game.h"

#include "Renderer.h"
#include "AudioSystem.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "CameraActor.h"
#include "AudioComponent.h"

Game::Game()
	: mActors()
	, mPendingActors()
	, mpRenderer(nullptr)
	, mpAudioSystem(nullptr)
	, mTicksCount(0)
	, mbRunning(true)
	, mbUpdatingActors(false)
	, mpCameraActor(nullptr)
	, mMusicEvent()
	, mReverbSnap()
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

	// LoadData
	{
		Actor* const pCube = new Actor(this);
		pCube->SetPosition(Vector3(200.f, 75.f, 0.f));
		pCube->SetScale(100.f);

		Quaternion q(Vector3::UnitY, -Math::PI_OVER_2);
		q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PI + Math::PI / 4.f));

		pCube->SetRotation(q);

		MeshComponent* const pCubeMesh = new MeshComponent(pCube);
		pCubeMesh->SetMesh(mpRenderer->GetMesh("Assets/Cube.gpmesh"));

		Actor* const pSphere = new Actor(this);
		pSphere->SetPosition(Vector3(200.f, -75.f, 0.f));
		pSphere->SetScale(3.f);

		MeshComponent* const pSphereMeshComponent = new MeshComponent(pSphere);
		pSphereMeshComponent->SetMesh(mpRenderer->GetMesh("Assets/Sphere.gpmesh"));

		constexpr float START = -1250.f;
		constexpr float SIZE = 250.f;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				PlaneActor* const pPlane = new PlaneActor(this);
				pPlane->SetPosition(Vector3(START + i * SIZE, START + j * SIZE, -100.f));
			}
		}

		q = Quaternion(Vector3::UnitX, Math::PI_OVER_2);
		for (int i = 0; i < 10; ++i)
		{
			PlaneActor* const pPlane1 = new PlaneActor(this);
			pPlane1->SetPosition(Vector3(START + i * SIZE, START - SIZE, 0.f));
			pPlane1->SetRotation(q);

			PlaneActor* const pPlane2 = new PlaneActor(this);
			pPlane2->SetPosition(Vector3(START + i * SIZE, -START + SIZE, 0.f));
			pPlane2->SetRotation(q);
		}

		q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PI_OVER_2));
		for (int i = 0; i < 10; ++i)
		{
			PlaneActor* const pPlane1 = new PlaneActor(this);
			pPlane1->SetPosition(Vector3(START - SIZE, START + i * SIZE, 0.f));
			pPlane1->SetRotation(q);

			PlaneActor* const pPlane2 = new PlaneActor(this);
			pPlane2->SetPosition(Vector3(-START + SIZE, START + i * SIZE, 0.f));
			pPlane2->SetRotation(q);
		}

		mpRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));

		DirectionalLight& dir = mpRenderer->GetDirectionalLight();
		dir.direction = Vector3(0.f, -0.707f, -0.707f);
		dir.diffuseColor = Vector3(0.78f, 0.88f, 1.f);
		dir.specColor = Vector3(0.8f, 0.8f, 0.8f);

		mpCameraActor = new CameraActor(this);

		Actor* const pHealthBar = new Actor(this);
		pHealthBar->SetPosition(Vector3(-350.f, -350.f, 0.f));

		SpriteComponent* const pHealthBarSprite = new SpriteComponent(pHealthBar);
		pHealthBarSprite->SetTexture(mpRenderer->GetTexture("Assets/HealthBar.png"));

		Actor* const pRadar = new Actor(this);
		pRadar->SetPosition(Vector3(375.f, -275.f, 0.f));
		pRadar->SetScale(0.75f);

		SpriteComponent* const pRadarSprite = new SpriteComponent(pRadar);
		pRadarSprite->SetTexture(mpRenderer->GetTexture("Assets/Radar.png"));

		Actor* const pSphereSound = new Actor(this);
		pSphere->SetPosition(Vector3(500.f, -75.f, 0.f));
		pSphere->SetScale(1.f);

		MeshComponent* const pSphereMesh = new MeshComponent(pSphereSound);
		pSphereMesh->SetMesh(mpRenderer->GetMesh("Assets/Sphere.gpmesh"));

		AudioComponent* const pAudioComp = new AudioComponent(pSphereSound);
		pAudioComp->PlayEvent("event:/FireLoop");

		mMusicEvent = mpAudioSystem->PlayEvent("event:/Music");
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

	case '1':
		mpCameraActor->SetFootstepSurface(0.f);
		break;

	case '2':
		mpCameraActor->SetFootstepSurface(0.5f);
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
