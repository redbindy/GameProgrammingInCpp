#pragma once

#include "Actor.h"

class BoxComponent;

class PlaneActor final : public Actor
{
public:
	PlaneActor(Game* const pGame);
	virtual ~PlaneActor();

	BoxComponent* GetBox()
	{
		return mpBox;
	}

private:
	BoxComponent* mpBox;
};