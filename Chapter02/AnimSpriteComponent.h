#pragma once

#include "SpriteComponent.h"

class Actor;

struct AnimationInfo
{
	float animFPS;
	int startIndex;
	int animCount;
	bool bRepeat;
};

class AnimSpriteComponent final : public SpriteComponent
{
public:
	AnimSpriteComponent(Actor* const pOwner, const int drawOrder);
	AnimSpriteComponent(Actor* const pOnwer);
	virtual ~AnimSpriteComponent() = default;

	void Update(const float deltaTime) override;

	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	void SetAnimationInfos(const std::vector<AnimationInfo>& animInfos);
	void SetAnimation(const int infoIndex);

	inline float GetAnimFPS() const;
	inline void SetAnimFPS(const float fps);

	inline bool IsFinished() const;
	inline bool IsRepeating() const;

public:
	std::vector<SDL_Texture*> mAnimTexturePtrs;

	float mCurrFrame;

	AnimationInfo mCurrInfo;

	std::vector<AnimationInfo> mAnimInfos;
};

inline float AnimSpriteComponent::GetAnimFPS() const
{
	return mCurrInfo.animFPS;
}

inline void AnimSpriteComponent::SetAnimFPS(const float fps)
{
	mCurrInfo.animFPS = fps;
}

inline bool AnimSpriteComponent::IsFinished() const
{
	return mCurrFrame >= mCurrInfo.animCount;
}

inline bool AnimSpriteComponent::IsRepeating() const
{
	return mCurrInfo.bRepeat;
}
