#include "Ship.h"

#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Ship::Ship(Game* const pGame)
	: Actor(pGame)
	, mLaserCooldown(0.f)
{
	SpriteComponent* sc = new SpriteComponent(this, 150);

	Texture* pTexture = pGame->GetTexture("Assets/Ship.png");
	sc->SetTexture(pTexture);

	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.f);
	ic->SetMaxAngularSpeed(Math::TWO_PI);
}

void Ship::UpdateActor(const float deltaTime)
{
	mLaserCooldown -= deltaTime;
}

void Ship::ActorInput(const bool* const pbKeyStates)
{
	if (mLaserCooldown <= 0.f && pbKeyStates[SDL_SCANCODE_SPACE])
	{
		Laser* const laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		mLaserCooldown = 0.5f;
	}
}

