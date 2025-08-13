#pragma once

#include <vector>
#include <algorithm>

#include <SDL3/SDL.h>

#include "Math.h"
#include "SoundEvent.h"

constexpr float SCREEN_WIDTH = 1024.f;
constexpr float SCREEN_HEIGHT = 768.f;

class Actor;
class Renderer;
class AudioSystem;
class CameraActor;
class FPSActor;
class FollowActor;
class OrbitActor;
class SplineActor;
class SpriteComponent;
class PhysWorld;
class PlaneActor;

class Game final
{
public:
	Game();
	virtual ~Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(Actor* const pActor);
	void RemoveActor(Actor* const pActor);

	inline Renderer* GetRenderer();
	inline AudioSystem* GetAudioSystem();
	inline PhysWorld* GetPhysWorld();

	void AddPlane(PlaneActor* const pPlane);
	void RemovePlane(PlaneActor* const pPlane);

	inline std::vector<PlaneActor*>& GetPlanes();

private:
	std::vector<Actor*> mActors;
	std::vector<Actor*> mPendingActors;

	Renderer* mpRenderer;
	AudioSystem* mpAudioSystem;
	PhysWorld* mpPhysWorld;

	uint64_t mTicksCount;

	bool mbRunning;

	bool mbUpdatingActors;

	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;

	std::vector<PlaneActor*> mPlanes;

	FPSActor* mpFPSActor;
	SpriteComponent* mpCrosshair;

private:
	void processInput();
	void handleKeyPress(const int key);
	void updateGame();
	void generateOutput();
};

inline Renderer* Game::GetRenderer()
{
	return mpRenderer;
}

inline AudioSystem* Game::GetAudioSystem()
{
	return mpAudioSystem;
}

inline PhysWorld* Game::GetPhysWorld()
{
	return mpPhysWorld;
}

inline std::vector<PlaneActor*>& Game::GetPlanes()
{
	return mPlanes;
}