#pragma once

#include <SDL3/SDL.h>

#include "Actor.h"

class Game;
class CircleComponent;
class SpriteComponent;
class InputComponent;

class Ship final : public Actor
{
public:
	Ship(Game* const pGame);

	void UpdateActor(const float deltaTime) override;
	void ActorInput(const bool* const pbKeyStates) override;

private:
	float mLaserCooldown;

	CircleComponent* mpCollisionComponent;
	float mPauseTimer;

	SDL_Texture* mpTexture;
	SpriteComponent* mpSprite;

	InputComponent* mpInputComponent;
};