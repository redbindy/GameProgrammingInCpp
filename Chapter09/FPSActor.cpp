#include "FPSActor.h"

#include "Game.h"
#include "MoveComponent.h"
#include "AudioComponent.h"
#include "MeshComponent.h"
#include "FPSCamera.h"
#include "Actor.h"
#include "Renderer.h"

FPSActor::FPSActor(Game* const pGame)
	: Actor(pGame)
	, mpMoveComp(nullptr)
	, mpAudioComp(nullptr)
	, mpMeshComp(nullptr)
	, mpCameraComp(nullptr)
	, mpFPSModel(nullptr)
	, mFootStep()
	, mLastFootStep(0.f)

{
	mpMoveComp = new MoveComponent(this);
	mpAudioComp = new AudioComponent(this);

	mFootStep = mpAudioComp->PlayEvent("event:/FootStep");
	mFootStep.SetPaused(true);

	mpCameraComp = new FPSCamera(this);

	mpFPSModel = new Actor(pGame);
	mpFPSModel->SetScale(0.75f);

	Renderer* const pRenderer = pGame->GetRenderer();

	mpMeshComp = new MeshComponent(mpFPSModel);
	mpMeshComp->SetMesh(pRenderer->GetMesh("Assets/Rifle.gpmesh"));
}

void FPSActor::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLastFootStep -= deltaTime;
	if (!Math::IsNearZero(mpMoveComp->GetForwardSpeed()) && mLastFootStep <= 0.f)
	{
		mFootStep.SetPaused(false);
		mFootStep.Restart();
		mLastFootStep = 0.5f;
	}

	const Vector3 modelOffset(10.f, 10.f, -10.f);
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.X;
	modelPos += GetRight() * modelOffset.Y;
	modelPos.Z += modelOffset.Z;

	mpFPSModel->SetPosition(modelPos);

	Quaternion q = GetRotation();
	q = Quaternion::Concatenate(q, Quaternion(GetRight(), mpCameraComp->GetPitch()));

	mpFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const bool* const pbKeys)
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

	float strafeSpeed = 0.f;
	if (pbKeys[SDL_SCANCODE_A])
	{
		strafeSpeed -= DELTA;
	}

	if (pbKeys[SDL_SCANCODE_D])
	{
		strafeSpeed += DELTA;
	}
	mpMoveComp->SetStrafeSpeed(strafeSpeed);

	float x;
	float y;
	const SDL_MouseButtonFlags mouseFlags = SDL_GetRelativeMouseState(&x, &y);

	constexpr int maxMouseSpeed = 500;

	constexpr float maxAngularSpeed = Math::PI * 8;

	float angularSpeed = 0.f;
	if (!Math::IsNearZero(x))
	{
		angularSpeed = x / maxMouseSpeed;
		angularSpeed *= maxAngularSpeed;
	}
	mpMoveComp->SetAngularSpeed(angularSpeed);

	constexpr float maxPitchSpeed = Math::PI * 8;

	float pitchSpeed = 0.f;
	if (!Math::IsNearZero(y))
	{
		pitchSpeed = y / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	mpCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::SetFootStepSurface(const float value)
{
	mFootStep.SetPaused(true);
	mFootStep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(const bool bVisible)
{
	mpMeshComp->SetVisible(bVisible);
}
