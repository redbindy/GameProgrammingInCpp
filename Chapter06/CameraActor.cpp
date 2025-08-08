#include "CameraActor.h"

#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "MoveComponent.h"

CameraActor::CameraActor(Game* const pGame)
	: Actor(pGame)
	, mpMoveComp(nullptr)
{
	mpMoveComp = new MoveComponent(this);
}

void CameraActor::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	const Vector3 cameraPos = GetPosition();
	const Vector3 target = GetPosition() + GetForward() * 100.f;
	const Vector3 up = Vector3::UnitZ;

	const Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);

	Game* const pGame = GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	pRenderer->SetViewMatrix(view);
}

void CameraActor::ActorInput(const bool* pbKeys)
{
	constexpr float FORWARD_SPEED = 300.f;

	float forwardSpeed = 0.f;
	if (pbKeys[SDL_SCANCODE_W])
	{
		forwardSpeed += FORWARD_SPEED;
	}

	if (pbKeys[SDL_SCANCODE_S])
	{
		forwardSpeed -= FORWARD_SPEED;
	}

	float angularSpeed = 0.f;
	if (pbKeys[SDL_SCANCODE_A])
	{
		angularSpeed -= Math::TWO_PI;
	}

	if (pbKeys[SDL_SCANCODE_D])
	{
		angularSpeed += Math::TWO_PI;
	}

	mpMoveComp->SetForwardSpeed(forwardSpeed);
	mpMoveComp->SetAngularSpeed(angularSpeed);
}
