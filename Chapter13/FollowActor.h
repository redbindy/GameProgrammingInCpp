#pragma once

#include <SDL3/SDL.h>

#include "Actor.h"

class Game;
class MoveComponent;
class FollowCamera;
class SkeletalMeshComponent;

class FollowActor final : public Actor
{
public:
	FollowActor(Game* const pGame);
	virtual ~FollowActor() = default;

	void ActorInput(const bool* const pbKeys) override;

	void SetVisible(const bool bVisible);

private:
	MoveComponent* mpMoveComp;
	FollowCamera* mpCameraComp;
	SkeletalMeshComponent* mpMeshComp;

	bool mbMoving;
};