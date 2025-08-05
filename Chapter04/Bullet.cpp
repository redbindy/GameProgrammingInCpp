#include "Bullet.h"

#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Enemy.h"

Bullet::Bullet(Game* const pGame)
	: Actor(pGame)
	, mpCircle(nullptr)
	, mLiveTime(1.f)
{
	SpriteComponent* sc = new SpriteComponent(this);

	SDL_Texture* const pTexture = pGame->GetTexture("Assets/Projectile.png");
	sc->SetTexture(pTexture);

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(400.f);

	mpCircle = new CircleComponent(this);
	mpCircle->SetRadius(5.f);
}

void Bullet::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	for (Enemy* const p : GetGame()->GetEnemies())
	{
		if (Intersect(*mpCircle, *(p->GetCircle())))
		{
			p->SetState(EState::DEAD);
			SetState(EState::DEAD);

			break;
		}
	}

	mLiveTime -= deltaTime;
	if (mLiveTime <= 0.f)
	{
		SetState(EState::DEAD);
	}
}


