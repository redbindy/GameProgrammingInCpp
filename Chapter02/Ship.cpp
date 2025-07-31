#include "Ship.h"

#include "AnimSpriteComponent.h"
#include "Game.h"

Ship::Ship(Game* pGame)
	: Actor(pGame)
	, mRightSpeed(0.f)
	, mDownSpeed(0.f)
{
	AnimSpriteComponent* animationComponent = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> animations = {
		pGame->GetTexture("./Assets/Ship01.png"),
		pGame->GetTexture("./Assets/Ship02.png"),
		pGame->GetTexture("./Assets/Ship03.png"),
		pGame->GetTexture("./Assets/Ship04.png")
	};
	animationComponent->SetAnimTextures(animations);
}

void Ship::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	Vector2 pos = GetPosition();
	pos.X += mRightSpeed * deltaTime;
	pos.Y += mDownSpeed * deltaTime;

	if (pos.X < 25.f)
	{
		pos.X = 25.f;
	}
	else if (pos.X > 500.f)
	{
		pos.X = 500.f;
	}

	if (pos.Y < 25.f)
	{
		pos.Y = 25.f;
	}
	else if (pos.Y > 743.f)
	{
		pos.Y = 743.f;
	}

	SetPosition(pos);
}

void Ship::ProcessKeyboard(const bool* pbStates)
{
	mRightSpeed = 0.f;
	mDownSpeed = 0.f;

	if (pbStates[SDL_SCANCODE_D])
	{
		mRightSpeed += 250.f;
	}

	if (pbStates[SDL_SCANCODE_A])
	{
		mRightSpeed -= 250.f;
	}

	if (pbStates[SDL_SCANCODE_S])
	{
		mDownSpeed += 300.f;
	}

	if (pbStates[SDL_SCANCODE_W])
	{
		mDownSpeed -= 300.f;
	}
}
