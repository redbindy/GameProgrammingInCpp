#include "SpriteComponent.h"

#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* const pOwner, const int drawOrder)
	: Component(pOwner)
	, mpTexture(nullptr)
	, mTextureWidth(0.f)
	, mTextureHeight(0.f)
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

	SDL_FRect dstRect;
	dstRect.w = mTextureWidth * mpOwner->GetScale();
	dstRect.h = mTextureHeight * mpOwner->GetScale();

	const Vector2& pos = mpOwner->GetPosition();
	dstRect.x = pos.X - dstRect.w * 0.5f;
	dstRect.y = pos.Y - dstRect.h * 0.5f;

	SDL_RenderTextureRotated(
		pRenderer,
		mpTexture,
		nullptr,
		&dstRect,
		-Math::ToDegrees(mpOwner->GetRotation()),
		nullptr,
		SDL_FLIP_NONE
	);
}

void SpriteComponent::SetTexture(SDL_Texture* const pTexture)
{
	mpTexture = pTexture;

	SDL_GetTextureSize(pTexture, &mTextureWidth, &mTextureHeight);
}
