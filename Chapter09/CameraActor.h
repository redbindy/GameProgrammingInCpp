#pragma once

#include <SDL3/SDL_scancode.h>

#include "Actor.h"
#include "SoundEvent.h"

class Game;
class MoveComponent;
class AudioComponent;

class CameraActor final : public Actor
{
public:
	CameraActor(Game* const pGame);
	virtual ~CameraActor() = default;

	void UpdateActor(const float deltaTime) override;
	void ActorInput(const bool* const pbKey) override;

	void SetFootstepSurface(const float value);

private:
	MoveComponent* mpMoveComp;
	AudioComponent* mpAudioComp;

	SoundEvent mFootStep;
	float mLastFootStep;
};