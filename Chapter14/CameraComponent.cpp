#include "CameraComponent.h"

#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "AudioSystem.h"

CameraComponent::CameraComponent(Actor* const pOwner, const int updateOrder)
	: Component(pOwner)
{
}

CameraComponent::CameraComponent(Actor* const pOwner)
	: CameraComponent(pOwner, 200)
{
}

void CameraComponent::SetViewMatrix(const Matrix4& view)
{
	Game* const pGame = mpOwner->GetGame();

	Renderer* const pRenderer = pGame->GetRenderer();
	pRenderer->SetViewMatrix(view);

	AudioSystem* const pAudioSystem = pGame->GetAudioSystem();
	pAudioSystem->SetListener(view);
}