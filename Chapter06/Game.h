#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <algorithm>

class Actor;
class Renderer;
class CameraActor;

enum
{
	SCREEN_WIDTH = 1024,
	SCREEN_HEIGHT = 768
};

constexpr float MAX_DELTA_TIME = 0.05f;

class Game
{
public:
	Game();
	virtual ~Game() = default;

	bool Initialize();
	void Shutdown();

	void RunLoop();

	void AddActor(Actor* const pActor);
	void RemoveActor(Actor* const pActor);

	inline Renderer* GetRenderer();

private:
	uint64_t mTicksCount;

	bool mbRunning;

	std::vector<Actor*> mActorPtrs;
	std::vector<Actor*> mPendingActorPtrs;

	bool mbUpdatingActors;

	Renderer* mpRenderer;

	CameraActor* mpCameraActor;

private:
	void processInput();
	void updateGame();
	void generateOutput();
};

inline Renderer* Game::GetRenderer()
{
	return mpRenderer;
}
