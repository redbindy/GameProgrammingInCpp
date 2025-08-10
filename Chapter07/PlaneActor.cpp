#include "PlaneActor.h"

#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"

PlaneActor::PlaneActor(Game* const pGame)
	: Actor(pGame)
{
	SetScale(10.f);

	Renderer* const pRenderer = pGame->GetRenderer();

	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(pRenderer->GetMesh("Assets/Plane.gpmesh"));
}
