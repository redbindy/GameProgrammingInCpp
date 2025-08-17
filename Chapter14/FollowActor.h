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

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;

	TypeID GetType() const override
	{
		return TFollowActor;
	}

private:
	MoveComponent* mpMoveComp;
	FollowCamera* mpCameraComp;
	SkeletalMeshComponent* mpMeshComp;

	bool mbMoving;
};