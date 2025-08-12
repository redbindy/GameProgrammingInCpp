#pragma once

#include <SDL3/SDL.h>

#include "Actor.h"

class Game;
class OrbitCamera;
class MeshComponent;

class OrbitActor final : public Actor
{
public:
	OrbitActor(Game* const pGame);
	virtual ~OrbitActor() = default;

	void ActorInput(const bool* const pbKeys) override;

	void SetVisible(const bool visible);

private:
	OrbitCamera* mpCameraComp;
	MeshComponent* mpMeshComp;
};