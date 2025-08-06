#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Math.h"

class Actor;
class SpriteComponent;
class Ship;
class Asteroid;
class VertexArray;
class Shader;
class Texture;

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

	Texture* GetTexture(const std::string& fileName);

	void AddAsteroid(Asteroid* const pAst);
	void RemoveAsteroid(Asteroid* const pAst);
	inline std::vector<Asteroid*>& GetAsteroids();

private:
	SDL_Window* mpWindow;
	SDL_GLContext mContext;

	uint64_t mTicksCount;

	bool mbRunning;

	std::unordered_map<std::string, Texture*> mTexturePtrMap;

	std::vector<Actor*> mActorPtrs;
	std::vector<Actor*> mPendingActorPtrs;

	std::vector<SpriteComponent*> mSprites;

	bool mbUpdatingActors;

	Ship* mpShip;
	std::vector<Asteroid*> mAsteroids;

	Shader* mpSpriteShader;
	VertexArray* mpSpriteVerts;

	/*Vector3 mColorArr[3];
	int mBackgroundColorIndex;
	float mBackgroundBrightness;*/

private:
	void processInput();
	void updateGame();
	void generateOutput();
};

std::vector<Asteroid*>& Game::GetAsteroids()
{
	return mAsteroids;
}