#include "Character.h"

#include "Game.h"
#include "AnimSpriteComponent.h"

Character::Character(Game* const pGame)
	: Actor(pGame)
	, mState(EState::WALK)
	, mpAnimationComponent(nullptr)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);

	const char* pathFormat = "Assets/Character%02d.png";

	char pathBuffer[32];

	constexpr int ANIMATION_COUNT = 18;

	std::vector<SDL_Texture*> anims;
	anims.reserve(ANIMATION_COUNT);

	for (int i = 1; i <= ANIMATION_COUNT; ++i)
	{
		sprintf(pathBuffer, pathFormat, i);

		SDL_Texture* const pTexture = pGame->GetTexture(pathBuffer);

		anims.push_back(pTexture);
	}

	constexpr int STATE_COUNT = static_cast<int>(Character::EState::STATE_COUNT);

	std::vector<AnimationInfo> animationInfos = {
		{ 12.f, 0, 6, true }, // walk
		{ 18.f, 6, 9, false }, // jump
		{ 6.f, 15, 3, false } // punch
	};

	asc->SetAnimTextures(anims);
	asc->SetAnimationInfos(animationInfos);
	asc->SetAnimation(static_cast<int>(mState));

	mpAnimationComponent = asc;
}

void Character::UpdateActor(const float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	if (mpAnimationComponent->IsFinished() && mState != EState::WALK)
	{
		mState = EState::WALK;
		mpAnimationComponent->SetAnimation(static_cast<int>(mState));
	}
}

void Character::ProcessKeyboard(const bool* pbStates)
{
	if (!mpAnimationComponent->IsFinished() && !mpAnimationComponent->IsRepeating())
	{
		return;
	}

	const EState prevState = mState;
	if (pbStates[SDL_SCANCODE_SPACE])
	{
		mState = EState::JUMP;
	}
	else if (pbStates[SDL_SCANCODE_X])
	{
		mState = EState::PUNCH;
	}

	if (prevState != mState)
	{
		mpAnimationComponent->SetAnimation(static_cast<int>(mState));
	}
}
