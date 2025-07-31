#include "BGSpriteComponent.h"

#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(Actor* const pOwner, const int drawOrder)
	: SpriteComponent(pOwner, drawOrder)
	, mBGTextures()
	, mScreenSize()
	, mScrollSpeed(0.f)
{
}

BGSpriteComponent::BGSpriteComponent(Actor* const pOwner)
	: BGSpriteComponent(pOwner, 10)
{

}

void BGSpriteComponent::Update(const float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	for (BGTexture& bg : mBGTextures)
	{
		bg.offset.X += mScrollSpeed * deltaTime;

		if (bg.offset.X < -mScreenSize.X)
		{
			bg.offset.X = (mBGTextures.size() - 1) * mScreenSize.X - 1;
		}
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* pRenderer)
{
	for (BGTexture& bg : mBGTextures)
	{
		Vector2 position = mpOwner->GetPosition();

		SDL_FRect rect;
		rect.w = mScreenSize.X;
		rect.h = mScreenSize.Y;
		rect.x = position.X - rect.w * 0.5f + bg.offset.X;
		rect.y = position.Y - rect.h * 0.5f + bg.offset.Y;

		SDL_RenderTexture(pRenderer, bg.pTexture, nullptr, &rect);
	}
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;

	for (SDL_Texture* const pTexture : textures)
	{
		BGTexture temp;
		temp.pTexture = pTexture;
		temp.offset.X = count * mScreenSize.X;
		temp.offset.Y = 0;

		mBGTextures.push_back(temp);

		++count;
	}
}
