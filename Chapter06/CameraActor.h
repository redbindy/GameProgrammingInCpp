#pragma once

#include <SDL3/SDL_scancode.h>

#include "Actor.h"

class Game;
class MoveComponent;

class CameraActor final : public Actor
{
public:
	CameraActor(Game* const pGame);
	virtual ~CameraActor() = default;

	void UpdateActor(const float deltaTime) override;
	void ActorInput(const bool* pbKeys) override;

private:
	MoveComponent* mpMoveComp;
};