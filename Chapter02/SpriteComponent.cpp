#include "SpriteComponent.h"

#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* const pOwner, const int drawOrder)
	: Component(pOwner)
	, mpTexture(nullptr)
	, mTextureWidth(0)
	, mTextureHeight(0)
	, mDrawOrder(drawOrder)
{
	Game* const pGame = pOwner->GetGame();

	pGame->AddSprite(this);
}

SpriteComponent::SpriteComponent(Actor* const pOwner)
	: SpriteComponent(pOwner, 100)
{

}

SpriteComponent::~SpriteComponent()
{
	Game* const pGame = mpOwner->GetGame();
	pGame->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* const pRenderer)
{
	if (mpTexture == nullptr)
	{
		return;
	}

	const Vector2 position = mpOwner->GetPosition();

	SDL_FRect rect;
	rect.w = mTextureWidth * mpOwner->GetScale();
	rect.h = mTextureHeight * mpOwner->GetScale();
	rect.x = position.X - rect.w * 0.5f;
	rect.y = position.Y - rect.h * 0.5f;

	SDL_RenderTextureRotated(
		pRenderer,
		mpTexture,
		nullptr,
		&rect,
		-Math::ToDegree(mpOwner->GetRotation()),
		nullptr,
		SDL_FLIP_NONE
	);
}

void SpriteComponent::SetTexture(SDL_Texture* const pTexture)
{
	mpTexture = pTexture;

	SDL_GetTextureSize(pTexture, &mTextureWidth, &mTextureHeight);
}