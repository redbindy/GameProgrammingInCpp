#include "CameraActor.h"

#include "Game.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "MoveComponent.h"
#include "AudioComponent.h"

CameraActor::CameraActor(Game* const pGame)
	: Actor(pGame)
	, mLastFootStep(0.f)
{
	mpMoveComp = new MoveComponent(this);
	mpAudioComp = new AudioComponent(this);

	mFootStep = mpAudioComp->PlayEvent("event:/Footstep");
	mFootStep.SetPaused(true);
}

void CameraActor::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLastFootStep -= deltaTime;
	if (!Math::IsNearZero(mpMoveComp->GetForwardSpeed()) && mLastFootStep <= 0.f)
	{
		mFootStep.SetPaused(false);
		mFootStep.Restart();
		mLastFootStep = 0.5f;
	}

	const Vector3 cameraPos = GetPosition();
	const Vector3 target = cameraPos + GetForward() * 100.f;
	const Vector3 up = Vector3::UnitZ;

	Game* const pGame = GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	const Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	pRenderer->SetViewMatrix(view);

	AudioSystem* const pAudioSystem = pGame->GetAudioSystem();
	pAudioSystem->SetListener(view);
}

void CameraActor::ActorInput(const bool* const pbKey)
{
	float forwardSpeed = 0.f;
	float angularSpeed = 0.f;

	if (pbKey[SDL_SCANCODE_W])
	{
		forwardSpeed += 300.f;
	}

	if (pbKey[SDL_SCANCODE_S])
	{
		forwardSpeed -= 300.f;
	}

	if (pbKey[SDL_SCANCODE_A])
	{
		angularSpeed -= Math::TWO_PI;
	}

	if (pbKey[SDL_SCANCODE_D])
	{
		angularSpeed += Math::TWO_PI;
	}

	mpMoveComp->SetForwardSpeed(forwardSpeed);
	mpMoveComp->SetAngularSpeed(angularSpeed);
}

void CameraActor::SetFootstepSurface(const float value)
{
	mFootStep.SetPaused(true);
	mFootStep.SetParameter("Surface", value);
}
