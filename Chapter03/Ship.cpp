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
	, mpCollisionComponent(nullptr)
	, mPauseTimer(0.f)
	, mpSprite(nullptr)
	, mpTexture(nullptr)
	, mpInputComponent(nullptr)
{
	mpSprite = new SpriteComponent(this, 150);

	mpTexture = pGame->GetTexture("Assets/Ship.png");
	mpSprite->SetTexture(mpTexture);

	mpInputComponent = new InputComponent(this);
	mpInputComponent->SetMass(10.f);
	mpInputComponent->SetForwardKey(SDL_SCANCODE_W);
	mpInputComponent->SetBackKey(SDL_SCANCODE_S);
	mpInputComponent->SetClockwiseKey(SDL_SCANCODE_A);
	mpInputComponent->SetCounterClockwiseKey(SDL_SCANCODE_D);
	mpInputComponent->SetMaxForwardSpeed(300.f);
	mpInputComponent->SetMaxAngularSpeed(Math::TWO_PI);

	mpCollisionComponent = new CircleComponent(this);
	mpCollisionComponent->SetRadius(8.f);
}

void Ship::UpdateActor(const float deltaTime)
{
	/*if (mPauseTimer > 0.f)
	{
		mPauseTimer -= deltaTime;

		return;
	}

	mpSprite->SetTexture(mpTexture);*/

	mLaserCooldown -= deltaTime;

	Game* const pGame = GetGame();
	for (Asteroid* const p : pGame->GetAsteroids())
	{
		if (Intersect(*mpCollisionComponent, *p->GetCircle()))
		{
			/*mPauseTimer = 1.5f;

			SetRotation(0);
			SetPosition(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
			mpSprite->SetTexture(nullptr);*/

			Vector2 collisionDir = p->GetPosition() - GetPosition();
			collisionDir.Normalize();

			mpInputComponent->AddForce(-1 * collisionDir * p->GetMove()->GetMass() * 100);
			p->GetMove()->AddForce(collisionDir * mpInputComponent->GetMass() * 100);
		}
	}
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

