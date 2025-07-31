#include "Game.h"

#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "BGSpriteComponent.h"
#include "Character.h"
#include "TileMapComponent.h"

Game::Game()
	: mpWindow(nullptr)
	, mpRenderer(nullptr)
	, mTicksCount(0)
	, mbRunning(true)
	, mActorPtrs()
	, mPendingActorPtrs()
	, mbUpdatingActors(false)
	, mTexturePtrMap()
	, mSpritePtrs()
	, mpShip(nullptr)
	, mpCharacter(nullptr)
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

	mpWindow = SDL_CreateWindow(
		"Chapter02",
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0
	);

	if (mpWindow == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());

		return false;
	}

	mpRenderer = SDL_CreateRenderer(mpWindow, nullptr);
	if (mpRenderer == nullptr)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());

		return false;
	}

	SDL_SetRenderVSync(mpRenderer, 1);

	// load data
	{
		mpShip = new Ship(this);
		mpShip->SetPosition(Vector2(100.f, 384.f));
		mpShip->SetScale(1.5f);

		mpCharacter = new Character(this);
		mpCharacter->SetPosition(Vector2(100.f, 600.f));

		
		Actor* pTemp = new Actor(this);
		pTemp->SetPosition(Vector2(512.f, 384.f));

		/*BGSpriteComponent* bg = new BGSpriteComponent(pTemp);
		bg->SetScreenSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));

		std::vector<SDL_Texture*> bgTextures = {
			GetTexture("./Assets/Farback01.png"),
			GetTexture("./Assets/Farback02.png")
		};

		bg->SetBGTextures(bgTextures);
		bg->SetScrollSpeed(-100.f);

		bg = new BGSpriteComponent(pTemp, 50);
		bg->SetScreenSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));

		bgTextures = {
			GetTexture("./Assets/Stars.png"),
			GetTexture("./Assets/Stars.png")
		};

		bg->SetBGTextures(bgTextures);
		bg->SetScrollSpeed(-200.f);
		*/

		TileMapComponent* tileMap = new TileMapComponent(pTemp);
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
	const int myDrawOrder = pSprite->GetDrawOrder();

	std::vector<SpriteComponent*>::iterator iter = mSpritePtrs.begin();
	while (iter != mSpritePtrs.end())
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}

		++iter;
	}

	mSpritePtrs.insert(iter, pSprite);
}

void Game::RemoveSprite(SpriteComponent* const pSprite)
{
	std::vector<SpriteComponent*>::iterator iter = std::find(mSpritePtrs.begin(), mSpritePtrs.end(), pSprite);
	mSpritePtrs.erase(iter);
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
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			SDL_Log(SDL_GetError());

			return nullptr;
		}

		pTexture = SDL_CreateTextureFromSurface(mpRenderer, pSurface);
		SDL_DestroySurface(pSurface);

		if (pTexture == nullptr)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());

			return nullptr;
		}

		mTexturePtrMap.insert({ fileName.c_str(), pTexture });
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

	const bool* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_ESCAPE])
	{
		mbRunning = false;
	}

	mpShip->ProcessKeyboard(pStates);
	mpCharacter->ProcessKeyboard(pStates);
}

void Game::updateGame()
{
	const uint64_t deadline = mTicksCount + 16;
	while (true)
	{
		const uint64_t currTicks = SDL_GetTicks();
		if (currTicks > deadline)
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
	for (Actor* const pActor : mActorPtrs)
	{
		pActor->Update(deltaTime);
	}
	mbUpdatingActors = false;

	for (Actor* const pActor : mPendingActorPtrs)
	{
		mActorPtrs.push_back(pActor);
	}
	mPendingActorPtrs.clear();

	std::vector<Actor*> deadActorPtrs;
	deadActorPtrs.reserve(mActorPtrs.size());

	for (Actor* const pActor : mActorPtrs)
	{
		if (pActor->GetState() == Actor::EState::DEAD)
		{
			deadActorPtrs.push_back(pActor);
		}
	}

	for (Actor* pActor : deadActorPtrs)
	{
		delete pActor;
	}
}

void Game::generateOutput()
{
	SDL_SetRenderDrawColor(mpRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mpRenderer);

	for (SpriteComponent* const p : mSpritePtrs)
	{
		p->Draw(mpRenderer);
	}

	SDL_RenderPresent(mpRenderer);
}