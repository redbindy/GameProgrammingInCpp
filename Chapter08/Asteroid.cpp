#include "Asteroid.h"

#include "Game.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* const pGame)
	: Actor(pGame)
	, mpCircle(nullptr)
{
	Vector2 worldCoord(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	Vector2 randPos = Random::GetVector(worldCoord * -1, worldCoord);
	SetPosition(randPos);

	SetRotation(Random::GetFloatRange(0.f, Math::TWO_PI));

	SpriteComponent* const sc = new SpriteComponent(this);

	Texture* const pTexture = pGame->GetTexture("Assets/Asteroid.png");
	sc->SetTexture(pTexture);

	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.f);

	mpCircle = new CircleComponent(this);
	mpCircle->SetRadius(40.f);

	pGame->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	Game* const pGame = GetGame();

	pGame->RemoveAsteroid(this);
}
