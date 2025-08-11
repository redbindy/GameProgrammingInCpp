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

private:
	CircleComponent* mpCircle;
};

inline CircleComponent* Asteroid::GetCircle() const
{
	return mpCircle;
}
