#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* const pOwner, const int drawOrder)
	: SpriteComponent(pOwner, drawOrder)
	, mAnimTexturePtrs()
	, mCurrFrame(0.f)
	, mCurrInfo({ 24.f, })
{
}

AnimSpriteComponent::AnimSpriteComponent(Actor* const pOwner)
	: AnimSpriteComponent(pOwner, 100)
{
}

void AnimSpriteComponent::Update(const float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTexturePtrs.size() == 0)
	{
		return;
	}

	mCurrFrame += deltaTime * mCurrInfo.animFPS;
	if (IsFinished() && !mCurrInfo.bRepeat)
	{
		return;
	}

	while (mCurrFrame >= mCurrInfo.animCount)
	{
		mCurrFrame -= mCurrInfo.animCount;
	}

	SetTexture(mAnimTexturePtrs[mCurrInfo.startIndex + static_cast<int>(mCurrFrame)]);
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTexturePtrs = textures;
	if (mAnimTexturePtrs.size() > 0)
	{
		mCurrFrame = 0.f;
		SetTexture(mAnimTexturePtrs[0]);

		mCurrInfo.startIndex = 0;
		mCurrInfo.animCount = static_cast<int>(mAnimTexturePtrs.size());

		mCurrInfo.bRepeat = true;
	}
}

void AnimSpriteComponent::SetAnimationInfos(const std::vector<AnimationInfo>& animInfos)
{
	mAnimInfos = animInfos;
}

void AnimSpriteComponent::SetAnimation(const int infoIndex)
{
	mCurrFrame = 0.f;
	mCurrInfo = mAnimInfos[infoIndex];
}

