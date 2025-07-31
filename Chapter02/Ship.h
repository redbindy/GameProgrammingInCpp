#pragma once

#include "Actor.h"

class Game;

class Ship final : public Actor
{
public:
	Ship(Game* pGame);
	virtual ~Ship() = default;

	void UpdateActor(const float deltaTime) override;

	void ProcessKeyboard(const bool* pbStates);

	inline float GetRightSpeed() const;
	inline float GetDownSpeed() const;

private:
	float mRightSpeed;
	float mDownSpeed;
};

inline float Ship::GetRightSpeed() const
{
	return mRightSpeed;
}

inline float Ship::GetDownSpeed() const
{
	return mDownSpeed;
}