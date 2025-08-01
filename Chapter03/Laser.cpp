#include "Laser.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Laser::Laser(Game* const pGame)
	: Actor(pGame)
	, mpCircle(nullptr)
	, mDeathTimer(1.f)
{
	SpriteComponent* const sc = new SpriteComponent(this);

	SDL_Texture* const pTexture = pGame->GetTexture("Assets/Laser.png");
	sc->SetTexture(pTexture);

	MoveComponent* const mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.f);

	mpCircle = new CircleComponent(this);
	mpCircle->SetRadius(11.f);
}

void Laser::UpdateActor(const float deltaTime)
{
	mDeathTimer -= deltaTime;
	if (mDeathTimer <= 0.f)
	{
		SetState(EState::DEAD);

		return;
	}

	Game* const pGame = GetGame();

	for (Asteroid* const p : pGame->GetAsteroids())
	{
		if (Intersect(*mpCircle, *p->GetCircle()))
		{
			SetState(EState::DEAD);
			p->SetState(EState::DEAD);

			break;
		}
	}
}
