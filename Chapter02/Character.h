#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Actor.h"

class Game;
class AnimSpriteComponent;

class Character final : public Actor
{
public:
	enum class EState
	{
		WALK,
		JUMP,
		PUNCH,
		STATE_COUNT
	};

public:
	Character(Game* const pGame);
	virtual ~Character() = default;

	void UpdateActor(const float deltaTime) override;

	void ProcessKeyboard(const bool* pbStates);

private:
	EState mState;

	AnimSpriteComponent* mpAnimationComponent;
};