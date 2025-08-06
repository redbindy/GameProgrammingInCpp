#include "Game.h"

#include "Actor.h"
#include "SpriteComponent.h"
#include "Asteroid.h"
#include "Random.h"
#include "Ship.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"

Game::Game()
	: mpWindow(nullptr)
	, mContext(nullptr)
	, mTicksCount(0)
	, mbRunning(true)
	, mTexturePtrMap()
	, mActorPtrs()
	, mPendingActorPtrs()
	, mSprites()
	, mbUpdatingActors(false)
	, mpShip(nullptr)
	, mAsteroids()
	, mpSpriteShader(nullptr)
	, mpSpriteVerts(nullptr)
	/*, mColorArr{ Color::Green, Color::Blue, Color::LightBlue }
	, mBackgroundColorIndex(0)
	, mBackgroundBrightness(0.f)*/
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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mpWindow = SDL_CreateWindow("Chapter05", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (mpWindow == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());

		return false;
	}

	mContext = SDL_GL_CreateContext(mpWindow);
	if (mContext == nullptr)
	{
		SDL_Log("%s", SDL_GetError());

		return false;
	}

	glewExperimental = GL_TRUE;
	GLenum eResult = glewInit();
	if (eResult != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW: %s", glewGetErrorString(eResult));

		return false;
	}

	glGetError();

	// load shader
	{
		mpSpriteShader = new Shader();
		if (!mpSpriteShader->Load("Shaders/Exercise.vert", "Shaders/Exercise.frag"))
		{
			SDL_Log("Failed to load Shaders.");

			return false;
		}

		mpSpriteShader->SetActive();

		Matrix4 viewProj = Matrix4::CreateSimpleViewProj(SCREEN_WIDTH, SCREEN_HEIGHT);
		mpSpriteShader->SetMatrixUniform("uViewProj", viewProj);
	}

	// create sprite verts
	{
		float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, // top left
		 0.5f,  0.5f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, // top right
		 0.5f, -0.5f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, // bottom right
		-0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f // bottom left
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		mpSpriteVerts = new VertexArray(vertices, 4, indices, 6);
	}

	Random::Init();

	// load data
	{
		constexpr int numAsteroids = 20;
		for (int i = numAsteroids; i > 0; --i)
		{
			new Asteroid(this);
		}

		mpShip = new Ship(this);
		mpShip->SetRotation(Math::PI_OVER_2);
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

		for (std::pair<const std::string, Texture*>& pair : mTexturePtrMap)
		{
			pair.second->UnLoad();

			delete pair.second;
		}
		mTexturePtrMap.clear();
	}

	mpSpriteShader->Unload();
	delete mpSpriteShader;

	delete mpSpriteVerts;

	SDL_GL_DestroyContext(mContext);
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

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* pTexture = nullptr;
	std::unordered_map<std::string, Texture*>::iterator iter = mTexturePtrMap.find(fileName);
	if (iter != mTexturePtrMap.end())
	{
		pTexture = iter->second;
	}
	else
	{
		pTexture = new Texture();
		if (pTexture->Load(fileName))
		{
			mTexturePtrMap.insert({ fileName, pTexture });
		}
		else
		{
			delete pTexture;

			pTexture = nullptr;
		}
	}

	return pTexture;
}

void Game::AddAsteroid(Asteroid* const pAst)
{
	mAsteroids.push_back(pAst);
}

void Game::RemoveAsteroid(Asteroid* const pAst)
{
	std::vector<Asteroid*>::iterator iter = std::find(mAsteroids.begin(), mAsteroids.end(), pAst);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
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

	/*constexpr float GRAD = 0.3f;
	mBackgroundBrightness += GRAD * deltaTime;
	if (mBackgroundBrightness >= 1.f)
	{
		mBackgroundBrightness = 0.f;
		mBackgroundColorIndex = (mBackgroundColorIndex + 1) % 3;
	}*/
}

void Game::generateOutput()
{
	/*const Vector3& backgroundColor = mColorArr[mBackgroundColorIndex] * mBackgroundBrightness;

	glClearColor(backgroundColor.X, backgroundColor.Y, backgroundColor.Z, 1.f);*/

	constexpr float GRAY_BRIGHTNESS = 0.86f;
	glClearColor(GRAY_BRIGHTNESS, GRAY_BRIGHTNESS, GRAY_BRIGHTNESS, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mpSpriteShader->SetActive();
	mpSpriteVerts->SetActive();

	for (SpriteComponent* const p : mSprites)
	{
		p->Draw(mpSpriteShader);
	}

	SDL_GL_SwapWindow(mpWindow);
}
