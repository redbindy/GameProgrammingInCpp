#pragma once

#include "Actor.h"

class Game;
class CircleComponent;

class Laser final : public Actor
{
public:
	Laser(Game* const pGame);
	virtual ~Laser() = default;

	void UpdateActor(const float deltaTime) override;

private:
	CircleComponent* mpCircle;
	float mDeathTimer;
};