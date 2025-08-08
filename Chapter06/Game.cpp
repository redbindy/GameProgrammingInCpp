#include "Game.h"

#include "Actor.h"
#include "Random.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "CameraActor.h"
#include "SpriteComponent.h"

Game::Game()
	: mTicksCount(0)
	, mbRunning(true)
	, mActorPtrs()
	, mPendingActorPtrs()
	, mbUpdatingActors(false)
	, mpRenderer(nullptr)
	, mpCameraActor(nullptr)
{

}

bool Game::Initialize()
{
	bool bResult = SDL_Init(SDL_INIT_VIDEO);
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

	Random::Init();

	// load data
	{
		Actor* const pCube = new Actor(this);
		pCube->SetPosition(Vector3(200.f, 75.f, 0.f));
		pCube->SetScale(100.f);

		Quaternion q(Vector3::UnitY, -Math::PI_OVER_2);
		q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PI + Math::PI / 4.f));
		pCube->SetRotation(q);

		MeshComponent* const pCubeMeshComp = new MeshComponent(pCube);
		pCubeMeshComp->SetMesh(mpRenderer->GetMesh("Assets/Cube.gpmesh"));

		Actor* const pSphere = new Actor(this);
		pSphere->SetPosition(Vector3(200.f, -75.f, 0.f));
		pSphere->SetScale(3.f);

		MeshComponent* const pSphereMeshComp = new MeshComponent(pSphere);
		pSphereMeshComp->SetMesh(mpRenderer->GetMesh("Assets/Sphere.gpmesh"));

		constexpr float START = -1250.f;
		constexpr float SIZE = 250.f;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				Actor* const pPlane = new PlaneActor(this);
				pPlane->SetPosition(Vector3(START + i * SIZE, START + j * SIZE, -100.f));
			}
		}

		q = Quaternion(Vector3::UnitX, Math::PI_OVER_2);
		for (int i = 0; i < 10; ++i)
		{
			Actor* const pPlane1 = new PlaneActor(this);
			pPlane1->SetPosition(Vector3(START + i * SIZE, START - SIZE, 0.f));
			pPlane1->SetRotation(q);

			Actor* const pPlane2 = new PlaneActor(this);
			pPlane2->SetPosition(Vector3(START + i * SIZE, -START + SIZE, 0.f));
			pPlane2->SetRotation(q);
		}

		q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PI_OVER_2));
		for (int i = 0; i < 10; ++i)
		{
			Actor* const pPlane1 = new PlaneActor(this);
			pPlane1->SetPosition(Vector3(START - SIZE, START + i * SIZE, 0.f));
			pPlane1->SetRotation(q);

			Actor* const pPlane2 = new PlaneActor(this);
			pPlane2->SetPosition(Vector3(-START + SIZE, START + i * SIZE, 0.f));
			pPlane2->SetRotation(q);
		}

		mpRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
		DirectionalLight& dir = mpRenderer->GetDirectionalLight();
		dir.direction = Vector3(0.f, -0.7f, -0.7f);
		dir.diffuse = Vector3(0.78f, 0.88f, 1.f);
		dir.specColor = Vector3(0.8f, 0.8f, 0.8f);

		mpCameraActor = new CameraActor(this);

		Actor* const pUI = new Actor(this);
		pUI->SetPosition(Vector3(-350.f, -350.f, 0.f));

		SpriteComponent* const sc = new SpriteComponent(pUI);
		sc->SetTexture(mpRenderer->GetTexture("Assets/HealthBar.png"));

		Actor* const pRadar = new Actor(this);
		pRadar->SetPosition(Vector3(375.f, -275.f, 0.f));
		pRadar->SetScale(0.75f);

		SpriteComponent* rd = new SpriteComponent(pRadar);
		rd->SetTexture(mpRenderer->GetTexture("Assets/Radar.png"));
	}

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	// unload data
	{
		while (!mActorPtrs.empty())
		{
			delete mActorPtrs.back();
		}
	}

	mpRenderer->Shutdown();

	SDL_Quit();
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

void Game::AddActor(Actor* const pActor)
{
	if (mbUpdatingActors)
	{
		mPendingActorPtrs.push_back(pActor);
	}
	else
	{
		mActorPtrs.push_back(pActor);
	}
}

void Game::RemoveActor(Actor* const pActor)
{
	std::vector<Actor*>::iterator iter = std::find(mPendingActorPtrs.begin(), mPendingActorPtrs.end(), pActor);
	if (iter != mPendingActorPtrs.end())
	{
		std::iter_swap(iter, mPendingActorPtrs.end() - 1);
		mPendingActorPtrs.pop_back();
	}

	iter = std::find(mActorPtrs.begin(), mActorPtrs.end(), pActor);
	if (iter != mActorPtrs.end())
	{
		std::iter_swap(iter, mActorPtrs.end() - 1);
		mActorPtrs.pop_back();
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
		}
	}

	const bool* pbKeyStates = SDL_GetKeyboardState(nullptr);
	if (pbKeyStates[SDL_SCANCODE_ESCAPE])
	{
		mbRunning = false;
	}

	mbUpdatingActors = true;
	for (Actor* const pActor : mActorPtrs)
	{
		pActor->ProcessInput(pbKeyStates);
	}
	mbUpdatingActors = false;
}

void Game::updateGame()
{
	const uint64_t deadline = mTicksCount + 16;
	while (true)
	{
		const uint64_t currTicks = SDL_GetTicks();
		if (currTicks >= deadline)
		{
			break;
		}
	}

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	if (deltaTime > MAX_DELTA_TIME)
	{
		deltaTime = MAX_DELTA_TIME;
	}
	mTicksCount = SDL_GetTicks();

	mbUpdatingActors = true;
	for (Actor* const p : mActorPtrs)
	{
		p->Update(deltaTime);
	}
	mbUpdatingActors = false;

	for (Actor* const p : mPendingActorPtrs)
	{
		p->ComputeWorldTransform();
		mActorPtrs.push_back(p);
	}
	mPendingActorPtrs.clear();

	std::vector<Actor*> deadActors;
	deadActors.reserve(mActorPtrs.size());

	for (Actor* p : mActorPtrs)
	{
		if (p->GetState() != Actor::EState::DEAD)
		{
			continue;
		}

		deadActors.push_back(p);
	}

	for (Actor* p : deadActors)
	{
		delete p;
	}
}

void Game::generateOutput()
{
	mpRenderer->Draw();
}
