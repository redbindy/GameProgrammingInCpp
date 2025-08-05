#pragma once

#include "Actor.h"

class Game;
class MoveComponent;

constexpr float ATTACK_TIME = 2.5f;
constexpr float ATTACK_RANGE = 100.f;

class Tower final : public Actor
{
public:
	Tower(Game* const pGame);
	virtual ~Tower() = default;

	void UpdateActor(const float deltaTime) override;

private:
	MoveComponent* mpMove;

	float mNextAttack;
};