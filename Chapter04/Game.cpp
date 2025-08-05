#include "Game.h"

#include "Actor.h"
#include "SpriteComponent.h"
#include "Grid.h"
#include "Tile.h"
#include "Enemy.h"
#include "AIComponent.h"
#include "AIState.h"

Game::Game()
	: mpWindow(nullptr)
	, mpRenderer(nullptr)
	, mTicksCount(0)
	, mbRunning(true)
	, mTexturePtrMap()
	, mActorPtrs()
	, mPendingActorPtrs()
	, mSprites()
	, mbUpdatingActors(false)
	, mEnemies()
	, mpGrid(nullptr)
	, mNextEnemy(0.f)
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

	mpWindow = SDL_CreateWindow("Chapter04", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (mpWindow == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());

		return false;
	}

	mpRenderer = SDL_CreateRenderer(mpWindow, nullptr);
	if (mpRenderer == nullptr)
	{
		SDL_Log("Failed to craete renderer: %s", SDL_GetError());

		return false;
	}

	// load data
	{
		mpGrid = new Grid(this);

		// AI test
		/*Actor* a = new Actor(this);
		AIComponent* aic = new AIComponent(a);
		aic->RegisterState(new AIPatrol(aic));
		aic->RegisterState(new AIDeath(aic));
		aic->RegisterState(new AIAttack(aic));
		aic->ChangeState("Patrol");*/
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

		for (std::pair<const std::string, SDL_Texture*>& pair : mTexturePtrMap)
		{
			SDL_DestroyTexture(pair.second);
		}
		mTexturePtrMap.clear();
	}

	SDL_DestroyRenderer(mpRenderer);
	SDL_DestroyWindow(mpWindow);
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

void Game::AddSprite(SpriteComponent* const pSprite)
{
	const int drawOrder = pSprite->GetDrawOrder();

	std::vector<SpriteComponent*>::iterator iter = mSprites.begin();

	while (iter != mSprites.end())
	{
		if ((*iter)->GetDrawOrder() > drawOrder)
		{
			break;
		}

		++iter;
	}

	mSprites.insert(iter, pSprite);
}

void Game::RemoveSprite(SpriteComponent* const pSprite)
{
	std::vector<SpriteComponent*>::iterator iter = std::find(mSprites.begin(), mSprites.end(), pSprite);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* pTexture = nullptr;
	std::unordered_map<std::string, SDL_Texture*>::iterator iter = mTexturePtrMap.find(fileName);
	if (iter != mTexturePtrMap.end())
	{
		pTexture = iter->second;
	}
	else
	{
		SDL_Surface* pSurface = IMG_Load(fileName.c_str());
		if (pSurface == nullptr)
		{
			SDL_Log("Failed to load texture file: %s", fileName.c_str());

			return nullptr;
		}

		pTexture = SDL_CreateTextureFromSurface(mpRenderer, pSurface);
		SDL_DestroySurface(pSurface);
		if (pTexture == nullptr)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());

			return nullptr;
		}

		mTexturePtrMap.insert({ fileName, pTexture });
	}

	return pTexture;
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

	if (pbKeyStates[SDL_SCANCODE_B])
	{
		mpGrid->BuildTower();
	}

	float x;
	float y;
	SDL_MouseButtonFlags buttons = SDL_GetMouseState(&x, &y);
	if (SDL_BUTTON_MASK(buttons) & SDL_BUTTON_LEFT)
	{
		mpGrid->ProcessClick(x, y);
	}

	mbUpdatingActors = true;
	for (Actor* const p : mActorPtrs)
	{
		p->ProcessInput(pbKeyStates);
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
	SDL_SetRenderDrawColor(mpRenderer, 34, 139, 34, 255);
	SDL_RenderClear(mpRenderer);

	for (SpriteComponent* const p : mSprites)
	{
		p->Draw(mpRenderer);
	}

	SDL_RenderPresent(mpRenderer);
}

Enemy* Game::GetNearestEnemy(const Vector2& pos)
{
	Enemy* pBest = nullptr;

	if (mEnemies.size() > 0)
	{
		pBest = mEnemies[0];

		Vector2 enemyToPos = pos - pBest->GetPosition();
		float bestDistSq = enemyToPos.LengthSq();

		for (size_t i = 1; i < mEnemies.size(); ++i)
		{
			Enemy* const pCurr = mEnemies[i];

			enemyToPos = pos - pCurr->GetPosition();

			const float newDistSq = enemyToPos.LengthSq();
			if (newDistSq < bestDistSq)
			{
				bestDistSq = newDistSq;
				pBest = pCurr;
			}
		}
	}

	return pBest;
}