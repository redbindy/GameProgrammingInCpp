#include "FollowActor.h"

#include "Game.h"
#include "MoveComponent.h"
#include "SkeletalMeshComponent.h"
#include "Renderer.h"
#include "FollowCamera.h"
#include "MirrorCamera.h"

FollowActor::FollowActor(Game* const pGame)
	: Actor(pGame)
	, mpMoveComp(nullptr)
	, mpCameraComp(nullptr)
	, mpMeshComp(nullptr)
	, mbMoving(false)
{
	mpMeshComp = new SkeletalMeshComponent(this);

	Renderer* const pRenderer = pGame->GetRenderer();
	mpMeshComp->SetMesh(pRenderer->GetMesh("Assets/CatWarrior.gpmesh"));
	mpMeshComp->SetSkeleton(pGame->GetSkeleton("Assets/CatWarrior.gpskel"));
	mpMeshComp->PlayAnimation(pGame->GetAnimation("Assets/CatActionIdle.gpanim"));

	SetPosition(Vector3(0.f, 0.f, -100.f));

	mpMoveComp = new MoveComponent(this);
	mpCameraComp = new FollowCamera(this);
	mpCameraComp->SnapToIdeal();

	MirrorCamera* const pMirror = new MirrorCamera(this);
	pMirror->SnapToIdeal();
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

	Game* const pGame = GetGame();
	if (!mbMoving && !Math::IsNearZero(forwardSpeed))
	{
		mbMoving = true;
		Animation* const pAnim = pGame->GetAnimation("Assets/CatRunSprint.gpanim");

		mpMeshComp->PlayAnimation(pAnim, 1.25);
	}
	else if (mbMoving && Math::IsNearZero(forwardSpeed))
	{
		mbMoving = false;
		Animation* const pAnim = pGame->GetAnimation("Assets/CatActionIdle.gpanim");

		mpMeshComp->PlayAnimation(pAnim);
	}
}

void FollowActor::SetVisible(const bool bVisible)
{
	mpMeshComp->SetVisible(bVisible);
}
