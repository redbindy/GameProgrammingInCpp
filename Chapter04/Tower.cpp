#include "Tower.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Enemy.h"
#include "Bullet.h"

Tower::Tower(Game* const pGame)
	: Actor(pGame)
	, mpMove(nullptr)
	, mNextAttack(ATTACK_TIME)
{
	SpriteComponent* sc = new SpriteComponent(this, 200);

	SDL_Texture* const pTexture = pGame->GetTexture("Assets/Tower.png");
	sc->SetTexture(pTexture);

	mpMove = new MoveComponent(this);
}

void Tower::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	Game* const pGame = GetGame();

	mNextAttack -= deltaTime;
	if (mNextAttack <= 0.f)
	{
		Enemy* pEnemy = pGame->GetNearestEnemy(GetPosition());
		if (pEnemy != nullptr)
		{
			const Vector2 dir = pEnemy->GetPosition() - GetPosition();

			const float dist = dir.Length();
			if (dist < ATTACK_RANGE)
			{
				SetRotation(Math::Atan2(-dir.Y, dir.X));

				Bullet* const pBullet = new Bullet(pGame);
				pBullet->SetPosition(GetPosition());
				pBullet->SetRotation(GetRotation());
			}
		}

		mNextAttack += ATTACK_TIME;
	}
}
