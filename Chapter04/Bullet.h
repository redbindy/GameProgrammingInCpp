#pragma once

#include "Actor.h"

class Game;
class CircleComponent;

class Bullet final : public Actor
{
public:
	Bullet(Game* const pGame);
	virtual ~Bullet() = default;

	void UpdateActor(const float deltaTime) override;

private:
	CircleComponent* mpCircle;

	float mLiveTime;
};