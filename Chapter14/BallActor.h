#pragma once

#include <rapidjson/document.h>

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

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;

	virtual TypeID GetType() const override
	{
		return TBallActor;
	}

private:
	AudioComponent* mpAudioComp;
	BallMove* mpMyMove;

	float mLifeSpan;
};