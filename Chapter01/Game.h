#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <random>

struct Vector2
{
	float x;
	float y;
};

struct Paddle
{
	Vector2 pos;
	int dir;
};

struct Ball
{
	Vector2 pos;
	Vector2 vel;
	bool bAlive;
};

class Game final
{
public:
	Game();
	virtual ~Game() = default;

	bool Initialize();

	void RunLoop();

	void Shutdown();

private:
	SDL_Window* mpWindow;
	SDL_Renderer* mpRenderer;
	bool mbRunning;

	uint64_t mTicksCount;

	Paddle mPlayers[2];

	std::vector<Ball> mBalls;

private:
	void processInput();
	void updateGame();
	void generateOutput();

	void bounceBall();
};