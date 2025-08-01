#pragma once

#include "Actor.h"

class Game;
class CircleComponent;
class MoveComponent;

class Asteroid final : public Actor
{
public:
	Asteroid(Game* const pGame);
	virtual ~Asteroid();

	inline CircleComponent* GetCircle() const;
	inline MoveComponent* GetMove() const;

private:
	CircleComponent* mpCircle;

	MoveComponent* mpMoveComponent;
};

inline CircleComponent* Asteroid::GetCircle() const
{
	return mpCircle;
}

inline MoveComponent* Asteroid::GetMove() const
{
	return mpMoveComponent;
}
