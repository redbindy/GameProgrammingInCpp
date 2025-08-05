#include "AIComponent.h"

#include "Actor.h"
#include "AIState.h"

AIComponent::AIComponent(Actor* const pOwner)
	: Component(pOwner)
	, mStateMap()
	, mpCurrentState(nullptr)
{

}

void AIComponent::Update(const float deltaTime)
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->Update(deltaTime);
	}
}

void AIComponent::ChangeState(const std::string& name)
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->OnExit();
	}

	std::unordered_map<std::string, AIState*>::iterator iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mpCurrentState = iter->second;

		mpCurrentState->OnEnter();
	}
	else
	{
		SDL_Log("Could not find AIState %s in state map", name.c_str());

		mpCurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* const state)
{
	mStateMap.insert({ state->GetName(), state });
}
