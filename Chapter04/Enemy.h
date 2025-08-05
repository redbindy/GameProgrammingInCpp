#pragma once

#include <algorithm>

#include "Actor.h"

class Game;
class CircleComponent;

class Enemy final : public Actor
{
public:
	Enemy(Game* const pGame);
	virtual ~Enemy();

	void UpdateActor(const float deltaTime) override;

	inline CircleComponent* GetCircle() const;

private:
	CircleComponent* mpCircle;
};

inline CircleComponent* Enemy::GetCircle() const
{
	return mpCircle;
}