#pragma once

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Math.h"
#include "SoundEvent.h"

constexpr float SCREEN_WIDTH = 1024.f;
constexpr float SCREEN_HEIGHT = 768.f;

class Actor;
class Renderer;
class AudioSystem;
class FollowActor;
class OrbitActor;
class SplineActor;
class SpriteComponent;
class PhysWorld;
class PlaneActor;
class Font;
class UIScreen;
class HUD;
class Skeleton;
class Animation;

class Game final
{
public:
	enum class EGameState
	{
		GAME_PLAY,
		PAUSED,
		QUIT
	};

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

	inline const std::vector<UIScreen*>& GetUIStack();
	void PushUI(UIScreen* const pScreen);

	Font* GetFont(const std::string& fileName);

	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);

	inline HUD* GetHUD();
	inline FollowActor* GetPlayer();
	void SetFollowActor(FollowActor* const pActor)
	{
		mpFollowActor = pActor;
	}

	Skeleton* GetSkeleton(const std::string& fileName);
	Animation* GetAnimation(const std::string& fileName);

	inline EGameState GetState() const;
	inline void SetState(const EGameState state);

	void AddPlane(PlaneActor* const pPlane);
	void RemovePlane(PlaneActor* const pPlane);

	inline std::vector<PlaneActor*>& GetPlanes();

	const std::vector<Actor*>& GetActors() const
	{
		return mActors;
	}

private:
	std::vector<Actor*> mActors;
	std::vector<Actor*> mPendingActors;

	std::vector<UIScreen*> mUIStack;
	std::unordered_map<std::string, Font*> mFonts;

	std::unordered_map<std::string, std::string> mText;

	std::unordered_map<std::string, Skeleton*> mSkeletons;
	std::unordered_map<std::string, Animation*> mAnims;

	Renderer* mpRenderer;
	AudioSystem* mpAudioSystem;
	PhysWorld* mpPhysWorld;
	HUD* mpHUD;

	uint64_t mTicksCount;

	bool mbUpdatingActors;

	EGameState mGameState;

	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;

	std::vector<PlaneActor*> mPlanes;

	FollowActor* mpFollowActor;
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

inline const std::vector<UIScreen*>& Game::GetUIStack()
{
	return mUIStack;
}

inline std::vector<PlaneActor*>& Game::GetPlanes()
{
	return mPlanes;
}

inline Game::EGameState Game::GetState() const
{
	return mGameState;
}

inline void Game::SetState(const Game::EGameState state)
{
	mGameState = state;
}

inline HUD* Game::GetHUD()
{
	return mpHUD;
}

inline FollowActor* Game::GetPlayer()
{
	return mpFollowActor;
}
