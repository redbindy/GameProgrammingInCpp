#include "Enemy.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "NavComponent.h"
#include "Grid.h"
#include "Tile.h"
#include "CircleComponent.h"

Enemy::Enemy(Game* const pGame)
	: Actor(pGame)
	, mpCircle(nullptr)
{
	std::vector<Enemy*>& enemies = pGame->GetEnemies();
	enemies.push_back(this);

	SpriteComponent* sc = new SpriteComponent(this);

	SDL_Texture* const pTexture = pGame->GetTexture("Assets/Airplane.png");
	sc->SetTexture(pTexture);

	Grid* const pGrid = pGame->GetGrid();
	Tile* const pStartTile = pGrid->GetStartTile();
	SetPosition(pStartTile->GetPosition());

	NavComponent* nc = new NavComponent(this);
	nc->SetForwardSpeed(150.f);
	nc->StartPath(pStartTile);

	mpCircle = new CircleComponent(this);
	mpCircle->SetRadius(25.f);
}

Enemy::~Enemy()
{
	Game* const pGame = GetGame();

	std::vector<Enemy*>& enemies = pGame->GetEnemies();

	std::vector<Enemy*>::iterator iter = std::find(enemies.begin(), enemies.end(), this);

	enemies.erase(iter);
}

void Enemy::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	Game* const pGame = GetGame();
	Grid* const pGrid = pGame->GetGrid();
	Tile* const pEndTile = pGrid->GetEndTile();

	Vector2 diff = GetPosition() - pEndTile->GetPosition();
	if (Math::IsNearZero(diff.Length(), 10.f))
	{
		SetState(EState::DEAD);
	}
}