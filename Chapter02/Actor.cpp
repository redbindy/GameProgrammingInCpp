#include "Actor.h"

#include "Game.h"
#include "Component.h"

Actor::Actor(class Game* const pGame)
	: mState(EState::ACTIVE)
	, mPosition()
	, mScale(1.f)
	, mRotation(0.f)
	, mComponentPtrs()
	, mpGame(pGame)
{
	pGame->AddActor(this);
}

Actor::~Actor()
{
	mpGame->RemoveActor(this);

	while (!mComponentPtrs.empty())
	{
		delete mComponentPtrs.back();
	}
}

void Actor::Update(const float deltaTime)
{
	if (mState != EState::ACTIVE)
	{
		return;
	}

	UpdateComponents(deltaTime);
	UpdateActor(deltaTime);
}

void Actor::UpdateComponents(const float deltaTime)
{
	for (Component* const pComponent : mComponentPtrs)
	{
		pComponent->Update(deltaTime);
	}
}

void Actor::UpdateActor(const float deltaTime)
{

}

void Actor::AddComponent(Component* const pComponent)
{
	int myOrder = pComponent->GetUpdateOrder();
	std::vector<Component*>::iterator iter = mComponentPtrs.begin();
	while (iter != mComponentPtrs.end())
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}

		++iter;
	}

	mComponentPtrs.insert(iter, pComponent);
}

void Actor::RemoveComponent(Component* const pComponent)
{
	std::vector<Component*>::iterator iter = std::find(mComponentPtrs.begin(), mComponentPtrs.end(), pComponent);
	if (iter != mComponentPtrs.end())
	{
		mComponentPtrs.erase(iter);
	}
}
