#include "Ship.h"

#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"
#include "CircleComponent.h"
#include "Asteroid.h"
#include "InputSystem.h"

Ship::Ship(Game* const pGame)
	: Actor(pGame)
	, mLaserCooldown(0.f)
	, mVelocityDir(Vector2::Zero)
	, mRotationDir(Vector2::Zero)
	, mSpeed(400.f)
{
	SpriteComponent* sc = new SpriteComponent(this, 150);

	Texture* pTexture = pGame->GetTexture("Assets/Ship.png");
	sc->SetTexture(pTexture);

	/*InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.f);
	ic->SetMaxAngularSpeed(Math::TWO_PI);*/
}

void Ship::UpdateActor(const float deltaTime)
{
	mLaserCooldown -= deltaTime;

	Vector2 pos = GetPosition();
	pos += mVelocityDir * mSpeed * deltaTime;
	SetPosition(pos);

	const float angle = Math::Atan2(mRotationDir.Y, mRotationDir.X);
	SetRotation(angle);
}

void Ship::ActorInput(const InputState& state)
{
	/*if (mLaserCooldown <= 0.f && state[SDL_SCANCODE_SPACE])
	{
		Laser* const laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		mLaserCooldown = 0.5f;
	}*/

	if (state.controller.GetRightTrigger() > 0.25f && mLaserCooldown <= 0.f)
	{
		Laser* const laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		mLaserCooldown = 0.25f;
	}

	if (state.controller.IsConnected())
	{
		mVelocityDir = state.controller.GetLeftStick();

		const Vector2 rightStick = state.controller.GetRightStick();
		if (!Math::IsNearZero(rightStick.Length()))
		{
			mRotationDir = state.controller.GetRightStick();
		}
	}
}

