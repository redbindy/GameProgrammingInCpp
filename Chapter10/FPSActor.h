#pragma once

#include <SDL3/SDL.h>

#include "Actor.h"
#include "SoundEvent.h"
#include "Collision.h"

class Game;
class MoveComponent;
class AudioComponent;
class MeshComponent;
class FPSCamera;
class Actor;
class BoxComponent;

class FPSActor final : public Actor
{
public:
	FPSActor(Game* const pGame);
	virtual ~FPSActor() = default;

	void UpdateActor(const float deltaTime) override;
	void ActorInput(const bool* const pbKeys) override;

	void SetFootStepSurface(const float value);

	void SetVisible(const bool bVisible);

	void FixCollisions();

	void Shoot();

private:
	MoveComponent* mpMoveComp;
	AudioComponent* mpAudioComp;
	MeshComponent* mpMeshComp;
	FPSCamera* mpCameraComp;
	BoxComponent* mpBoxComp;

	Actor* mpFPSModel;

	SoundEvent mFootStep;
	float mLastFootStep;
};