#pragma once

#include "MoveComponent.h"
#include "Math.h"
#include "Collision.h"

class BallMove final : public MoveComponent
{
public:
	BallMove(Actor* const pOwner);
	virtual ~BallMove() = default;

	void SetPlayer(Actor* const pPlayer)
	{
		mpPlayer = pPlayer;
	}

	void Update(const float deltaTime) override;

private:
	Actor* mpPlayer;
};