#include "OrbitActor.h"

#include "Game.h"
#include "OrbitCamera.h"
#include "MeshComponent.h"
#include "Renderer.h"

OrbitActor::OrbitActor(Game* const pGame)
	: Actor(pGame)
	, mpCameraComp(nullptr)
	, mpMeshComp(nullptr)
{
	mpMeshComp = new MeshComponent(this);

	Renderer* const pRenderer = pGame->GetRenderer();
	mpMeshComp->SetMesh(pRenderer->GetMesh("Assets/RacingCar.gpmesh"));

	SetPosition(Vector3(0.f, 0.f, -100.f));

	mpCameraComp = new OrbitCamera(this);
}

void OrbitActor::ActorInput(const bool* const pbKeys)
{
	float x;
	float y;
	const SDL_MouseButtonFlags buttons = SDL_GetRelativeMouseState(&x, &y);

	if (buttons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT))
	{
		constexpr int maxMouseSpeed = 500;
		constexpr float maxOrbitSpeed = Math::PI * 8;

		float yawSpeed = 0.f;
		if (!Math::IsNearZero(x))
		{
			yawSpeed = x / maxMouseSpeed;
			yawSpeed *= maxOrbitSpeed;
		}
		mpCameraComp->SetYawSpeed(yawSpeed);

		float pitchSpeed = 0.f;
		if (!Math::IsNearZero(y))
		{
			pitchSpeed = y / maxMouseSpeed;
			pitchSpeed *= maxOrbitSpeed;
		}
		mpCameraComp->SetPicthSpeed(pitchSpeed);
	}
}

void OrbitActor::SetVisible(const bool bVisible)
{
	mpMeshComp->SetVisible(bVisible);
}
