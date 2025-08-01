#include "Asteroid.h"

#include "Game.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* const pGame)
	: Actor(pGame)
	, mpCircle(nullptr)
	, mpMoveComponent(nullptr)
{
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
	SetPosition(randPos);

	SetRotation(Random::GetFloatRange(0.f, Math::TWO_PI));

	SpriteComponent* const sc = new SpriteComponent(this);

	SDL_Texture* const pTexture = pGame->GetTexture("Assets/Asteroid.png");
	sc->SetTexture(pTexture);

	mpMoveComponent = new MoveComponent(this);
	mpMoveComponent->SetForwardSpeed(150.f);
	mpMoveComponent->SetMass(100.f);

	mpCircle = new CircleComponent(this);
	mpCircle->SetRadius(40.f);

	pGame->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	Game* const pGame = GetGame();

	pGame->RemoveAsteroid(this);
}
