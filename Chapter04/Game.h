#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <vector>

#include "Math.h"

class Actor;
class SpriteComponent;
class Grid;
class Enemy;

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

	void AddSprite(SpriteComponent* const pSprite);
	void RemoveSprite(SpriteComponent* const pSprite);

	SDL_Texture* GetTexture(const std::string& fileName);

	inline Grid* GetGrid() const;
	inline std::vector<Enemy*>& GetEnemies();
	Enemy* GetNearestEnemy(const Vector2& pos);

private:
	SDL_Window* mpWindow;
	SDL_Renderer* mpRenderer;

	uint64_t mTicksCount;

	bool mbRunning;

	std::unordered_map<std::string, SDL_Texture*> mTexturePtrMap;

	std::vector<Actor*> mActorPtrs;
	std::vector<Actor*> mPendingActorPtrs;

	std::vector<SpriteComponent*> mSprites;

	bool mbUpdatingActors;

	std::vector<Enemy*> mEnemies;
	Grid* mpGrid;

	float mNextEnemy;

private:
	void processInput();
	void updateGame();
	void generateOutput();

};

inline Grid* Game::GetGrid() const
{
	return mpGrid;
}

inline std::vector<Enemy*>& Game::GetEnemies()
{
	return mEnemies;
}
