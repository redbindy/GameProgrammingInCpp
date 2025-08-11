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
	void ActorInput(const InputState& state) override;

private:
	float mLaserCooldown;

	Vector2 mVelocityDir;
	Vector2 mRotationDir;
	float mSpeed;
};