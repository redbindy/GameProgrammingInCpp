#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>

enum
{
	SCREEN_WIDTH = 1024,
	SCREEN_HEIGHT = 768
};

constexpr float MAX_DELTA_TIME = 0.05f;

class Actor;
class SpriteComponent;
class Ship;
class Character;

class Game final
{
public:
	Game();
	virtual ~Game() = default;

	bool Initialize();
	void Shutdown();

	void RunLoop();

	void AddActor(class Actor* const pActor);
	void RemoveActor(class Actor* const pActor);

	void AddSprite(SpriteComponent* const pSprite);
	void RemoveSprite(SpriteComponent* const pSprite);

	SDL_Texture* GetTexture(const std::string& fileName);
private:
	SDL_Window* mpWindow;
	SDL_Renderer* mpRenderer;

	uint64_t mTicksCount;
	bool mbRunning;

	std::vector<Actor*> mActorPtrs;
	std::vector<Actor*> mPendingActorPtrs;

	bool mbUpdatingActors;

	std::unordered_map<std::string, SDL_Texture*> mTexturePtrMap;

	std::vector<SpriteComponent*> mSpritePtrs;

	Ship* mpShip;
	Character* mpCharacter;

private:
	void processInput();
	void updateGame();
	void generateOutput();
};