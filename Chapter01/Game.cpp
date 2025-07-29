#include "Game.h"

constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;

constexpr float THICKNESS = 15.f;

constexpr float PADDLE_HEIGHT = 100.f;
constexpr float WINDOW_PADDLE_GAP = 10.f;

std::random_device sRandomDevice;
std::mt19937 sGenerator(sRandomDevice());
std::uniform_real_distribution<float> sDistribution;

Game::Game()
	: mpWindow(nullptr)
	, mpRenderer(nullptr)
	, mbRunning(true)
	, mTicksCount(0)
	, mPlayers{ { {WINDOW_PADDLE_GAP, SCREEN_HEIGHT * 0.5f}, 0 }, { {SCREEN_WIDTH - THICKNESS - WINDOW_PADDLE_GAP, SCREEN_HEIGHT * 0.5f}, 0 } }
	, mBalls()
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
		"Chapter01",
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

	for (int i = 4; i > 0; --i)
	{
		const float x = sDistribution(sGenerator) * SCREEN_WIDTH;
		const float y = sDistribution(sGenerator) * SCREEN_HEIGHT;

		Ball ball;
		ball.pos.x = x;
		ball.pos.y = y;
		ball.vel.x = x * sDistribution(sGenerator);
		ball.vel.y = y * sDistribution(sGenerator);
		ball.bAlive = true;

		mBalls.push_back(ball);
	}

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
	SDL_DestroyRenderer(mpRenderer);
	SDL_DestroyWindow(mpWindow);
	SDL_Quit();
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

	const bool* pbKeyboardStates = SDL_GetKeyboardState(nullptr);
	if (pbKeyboardStates[SDL_SCANCODE_ESCAPE])
	{
		mbRunning = true;
	}

	for (Paddle& paddle : mPlayers)
	{
		paddle.dir = 0;
	}

	if (pbKeyboardStates[SDL_SCANCODE_W])
	{
		--mPlayers[0].dir;
	}

	if (pbKeyboardStates[SDL_SCANCODE_S])
	{
		++mPlayers[0].dir;
	}

	if (pbKeyboardStates[SDL_SCANCODE_I])
	{
		--mPlayers[1].dir;
	}

	if (pbKeyboardStates[SDL_SCANCODE_K])
	{
		++mPlayers[1].dir;
	}
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

	constexpr float MAX_DELTA_TIME = 0.05f;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.f;
	if (deltaTime > MAX_DELTA_TIME)
	{
		deltaTime = MAX_DELTA_TIME;
	}

	mTicksCount = SDL_GetTicks();

	constexpr float PADDLE_HALF_LENGTH = PADDLE_HEIGHT * 0.5f;

	for (Paddle& paddle : mPlayers)
	{
		paddle.pos.y += paddle.dir * 300.f * deltaTime;
		if (paddle.pos.y < PADDLE_HALF_LENGTH + THICKNESS)
		{
			paddle.pos.y = PADDLE_HALF_LENGTH + THICKNESS;
		}
		else if (paddle.pos.y > (SCREEN_HEIGHT - PADDLE_HALF_LENGTH - THICKNESS))
		{
			paddle.pos.y = SCREEN_HEIGHT - PADDLE_HALF_LENGTH - THICKNESS;
		}
	}

	int ballCount = 4;
	for (Ball& ball : mBalls)
	{
		if (!ball.bAlive)
		{
			--ballCount;

			continue;
		}

		ball.pos.x += ball.vel.x * deltaTime;
		if (ball.pos.x < 0.f || ball.pos.x > SCREEN_WIDTH)
		{
			ball.bAlive = false;

			--ballCount;

			return;
		}

		ball.pos.y += ball.vel.y * deltaTime;
		if (ball.pos.y <= THICKNESS && ball.vel.y < 0.f
			|| ball.pos.y >= SCREEN_HEIGHT - THICKNESS && ball.vel.y > 0.f)
		{
			ball.vel.y *= -1;
		}

		float diffY1 = mPlayers[0].pos.y - ball.pos.y;
		if (diffY1 < 0)
		{
			diffY1 *= -1;
		}

		float diffY2 = mPlayers[1].pos.y - ball.pos.y;
		if (diffY2 < 0)
		{
			diffY2 *= -1;
		}

		if (diffY1 <= PADDLE_HALF_LENGTH && ball.pos.x >= WINDOW_PADDLE_GAP * 2 && ball.pos.x <= WINDOW_PADDLE_GAP + THICKNESS
			|| diffY2 <= PADDLE_HALF_LENGTH && ball.pos.x >= mPlayers[1].pos.x && ball.pos.x <= SCREEN_WIDTH - WINDOW_PADDLE_GAP * 2)
		{
			ball.vel.x *= -1;
		}
	}

	if (ballCount == 0)
	{
		mbRunning = false;
	}
}

void Game::generateOutput()
{
	SDL_SetRenderDrawColor(mpRenderer, 100, 100, 100, 255);
	SDL_RenderClear(mpRenderer);

	// wall
	{
		SDL_SetRenderDrawColor(mpRenderer, 255, 255, 255, 255);

		SDL_FRect up;
		up.x = 0;
		up.y = 0;
		up.w = SCREEN_WIDTH;
		up.h = THICKNESS;

		SDL_RenderFillRect(mpRenderer, &up);

		SDL_FRect bottom;
		bottom.x = 0;
		bottom.y = SCREEN_HEIGHT - THICKNESS;
		bottom.w = SCREEN_WIDTH;
		bottom.h = THICKNESS;

		SDL_RenderFillRect(mpRenderer, &bottom);
	}

	// paddle
	{
		for (Paddle& paddle : mPlayers)
		{
			SDL_FRect paddleRect;
			paddleRect.x = paddle.pos.x;
			paddleRect.y = paddle.pos.y - PADDLE_HEIGHT * 0.5f;
			paddleRect.w = THICKNESS;
			paddleRect.h = PADDLE_HEIGHT;

			SDL_RenderFillRect(mpRenderer, &paddleRect);
		}
	}

	// ball
	{
		for (const Ball& ball : mBalls)
		{
			if (!ball.bAlive)
			{
				continue;
			}

			SDL_FRect ballRect;
			ballRect.x = ball.pos.x - THICKNESS * 0.5f;
			ballRect.y = ball.pos.y - THICKNESS * 0.5f;
			ballRect.w = THICKNESS;
			ballRect.h = THICKNESS;

			SDL_RenderFillRect(mpRenderer, &ballRect);
		}

	}

	SDL_RenderPresent(mpRenderer);
}

void Game::bounceBall()
{

}
