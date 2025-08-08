#include "PlaneActor.h"

#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"

PlaneActor::PlaneActor(Game* const pGame)
	: Actor(pGame)
{
	SetScale(10.f);

	MeshComponent* const mc = new MeshComponent(this);

	Renderer* const pRenderer = pGame->GetRenderer();
	mc->SetMesh(pRenderer->GetMesh("Assets/Plane.gpmesh"));
}
