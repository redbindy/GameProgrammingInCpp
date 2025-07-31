#pragma once

#include <SDL3/SDL.h>

#include "Component.h"

class Actor;

class SpriteComponent : public Component
{
public:
	SpriteComponent(Actor* const pOwner, const int drawOrder);
	SpriteComponent(Actor* const pOwner);
	virtual ~SpriteComponent();

	virtual void Draw(SDL_Renderer* const pRenderer);
	virtual void SetTexture(SDL_Texture* const pTexture);

	int GetDrawOrder() const;
	float GetTextureWidth() const;
	float GetTextureHeight() const;

protected:
	SDL_Texture* mpTexture;

	float mTextureWidth;
	float mTextureHeight;
	int mDrawOrder;
};

inline int SpriteComponent::GetDrawOrder() const
{
	return mDrawOrder;
}

inline float SpriteComponent::GetTextureWidth() const
{
	return mTextureWidth;
}

inline float SpriteComponent::GetTextureHeight() const
{
	return mTextureHeight;
}