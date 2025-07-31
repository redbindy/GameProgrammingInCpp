#pragma once

#include <vector>

#include "SpriteComponent.h"
#include "Math.h"

class Actor;

class BGSpriteComponent final : public SpriteComponent
{
public:
	BGSpriteComponent(Actor* const pOwner, const int drawOrder);
	BGSpriteComponent(Actor* const pOwner);

	void Update(const float deltaTime) override;
	void Draw(SDL_Renderer* pRenderer) override;

	void SetBGTextures(const std::vector<SDL_Texture*>& textures);

	inline void SetScreenSize(const Vector2& size);
	inline void SetScrollSpeed(const float speed);
	float GetScrollSpeed() const;

private:
	struct BGTexture
	{
		SDL_Texture* pTexture;
		Vector2 offset;
	};

private:
	std::vector<BGTexture> mBGTextures;

	Vector2 mScreenSize;

	float mScrollSpeed;
};

inline void BGSpriteComponent::SetScreenSize(const Vector2& size)
{
	mScreenSize = size;
}

inline void BGSpriteComponent::SetScrollSpeed(const float speed)
{
	mScrollSpeed = speed;
}

inline float BGSpriteComponent::GetScrollSpeed() const
{
	return mScrollSpeed;
}