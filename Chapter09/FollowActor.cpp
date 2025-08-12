#include "FollowActor.h"

#include "Game.h"
#include "MoveComponent.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "FollowCamera.h"

FollowActor::FollowActor(Game* const pGame)
	: Actor(pGame)
	, mpMoveComp(nullptr)
	, mpCameraComp(nullptr)
	, mpMeshComp(nullptr)
{
	mpMeshComp = new MeshComponent(this);

	Renderer* const pRenderer = pGame->GetRenderer();
	mpMeshComp->SetMesh(pRenderer->GetMesh("Assets/RacingCar.gpmesh"));

	SetPosition(Vector3(0.f, 0.f, -100.f));

	mpMoveComp = new MoveComponent(this);
	mpCameraComp = new FollowCamera(this);
	mpCameraComp->SnapToIdeal();
}

void FollowActor::ActorInput(const bool* const pbKeys)
{
	constexpr float DELTA = 400.f;

	float forwardSpeed = 0.f;
	if (pbKeys[SDL_SCANCODE_W])
	{
		forwardSpeed += DELTA;
	}

	if (pbKeys[SDL_SCANCODE_S])
	{
		forwardSpeed -= DELTA;
	}
	mpMoveComp->SetForwardSpeed(forwardSpeed);

	float angularSpeed = 0.f;
	if (pbKeys[SDL_SCANCODE_A])
	{
		angularSpeed -= Math::PI;
	}

	if (pbKeys[SDL_SCANCODE_D])
	{
		angularSpeed += Math::PI;
	}
	mpMoveComp->SetAngularSpeed(angularSpeed);

	if (!Math::IsNearZero(forwardSpeed))
	{
		mpCameraComp->SetHorzDist(500.f);
	}
	else
	{
		mpCameraComp->SetHorzDist(350.f);
	}
}

void FollowActor::SetVisible(const bool bVisible)
{
	mpMeshComp->SetVisible(bVisible);
}
