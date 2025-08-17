#pragma once

#include "Actor.h"

class TargetActor final : public Actor
{
public:
	TargetActor(Game* const pGame);
	virtual ~TargetActor() = default;

	TypeID GetType() const override
	{
		return TTargetActor;
	}

private:
};