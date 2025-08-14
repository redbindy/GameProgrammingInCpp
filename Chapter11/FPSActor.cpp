#include "FPSActor.h"

#include "Game.h"
#include "MoveComponent.h"
#include "AudioComponent.h"
#include "MeshComponent.h"
#include "FPSCamera.h"
#include "Actor.h"
#include "Renderer.h"
#include "BoxComponent.h"
#include "PlaneActor.h"
#include "BallActor.h"

FPSActor::FPSActor(Game* const pGame)
	: Actor(pGame)
	, mpMoveComp(nullptr)
	, mpAudioComp(nullptr)
	, mpMeshComp(nullptr)
	, mpCameraComp(nullptr)
	, mpBoxComp(nullptr)
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

	mpBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-25.f, -25.f, -87.5f), Vector3(25.f, 25.f, 87.5f));

	mpBoxComp->SetObjectBox(myBox);
	mpBoxComp->SetShouldRotate(false);
}

void FPSActor::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	FixCollisions();

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

void FPSActor::FixCollisions()
{
	ComputeWorldTransform();

	const AABB& playerBox = mpBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	Game* const pGame = GetGame();

	std::vector<PlaneActor*>& planes = pGame->GetPlanes();

	for (PlaneActor* const p : planes)
	{
		BoxComponent* const pBox = p->GetBox();
		const AABB& planeBox = pBox->GetWorldBox();

		if (Intersect(playerBox, planeBox))
		{
			const float dx1 = planeBox.Max.X - playerBox.Min.X;
			const float dx2 = planeBox.Min.X - playerBox.Max.X;

			const float dy1 = planeBox.Max.Y - playerBox.Min.Y;
			const float dy2 = planeBox.Min.Y - playerBox.Max.Y;

			const float dz1 = planeBox.Max.Z - playerBox.Min.Z;
			const float dz2 = planeBox.Min.Z - playerBox.Max.Z;

			const float dx = Math::Abs(dx1) < Math::Abs(dx2) ? dx1 : dx2;
			const float dy = Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2;
			const float dz = Math::Abs(dz1) < Math::Abs(dz2) ? dz1 : dz2;

			const float absDx = Math::Abs(dx);
			const float absDy = Math::Abs(dy);
			const float absDz = Math::Abs(dz);

			if (absDx <= absDy && absDx <= absDz)
			{
				pos.X += dx;
			}
			else if (absDy <= absDx && absDy <= absDz)
			{
				pos.Y += dy;
			}
			else
			{
				pos.Z += dz;
			}

			SetPosition(pos);
			mpBoxComp->OnUpdateWorldTransform();
		}
	}
}

void FPSActor::Shoot()
{
	Vector3 screenPoint(0.f, 0.f, 0.f);

	Game* const pGame = GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	const Vector3 start = pRenderer->Unproject(screenPoint);

	screenPoint.Z = 0.9f;

	const Vector3 end = pRenderer->Unproject(screenPoint);

	Vector3 dir = end - start;
	dir.Normalize();

	BallActor* const pBall = new BallActor(pGame);
	pBall->SetPlayer(this);
	pBall->SetPosition(start + dir * 20.f);

	pBall->RotateToNewForward(dir);

	mpAudioComp->PlayEvent("event:/Shot");
}
