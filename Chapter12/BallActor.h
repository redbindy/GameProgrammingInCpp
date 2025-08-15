#pragma once

#include "Actor.h"

class Game;
class AudioComponent;
class BallMove;

class BallActor final : public Actor
{
public:
	BallActor(Game* const pGame);
	virtual ~BallActor() = default;

	void UpdateActor(const float deltaTime) override;
	void SetPlayer(Actor* const pPlayer);

	void HitTarget();

private:
	AudioComponent* mpAudioComp;
	BallMove* mpMyMove;

	float mLifeSpan;
};