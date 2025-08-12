#pragma once

#include "Actor.h"

class Game;
class SplineCamera;

class SplineActor final : public Actor
{
public:
	SplineActor(Game* const pGame);
	virtual ~SplineActor() = default;

	void ActorInput(const bool* const pbkeys) override;

	void RestartSpline();

private:
	SplineCamera* mpCameraComp;
};