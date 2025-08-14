#include "TargetComponent.h"

#include "Game.h"
#include "Renderer.h"
#include "HUD.h"
#include "Actor.h"

TargetComponent::TargetComponent(Actor* const pOwner)
	: Component(pOwner)
{
	Game* const pGame = pOwner->GetGame();
	HUD* const pHUD = pGame->GetHUD();

	pHUD->AddTargetComponent(this);
}

TargetComponent::~TargetComponent()
{
	Game* const pGame = mpOwner->GetGame();
	HUD* const pHUD = pGame->GetHUD();

	pHUD->RemoveTargetComponent(this);
}
