#include "PlaneActor.h"

#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "BoxComponent.h"
#include "Mesh.h"

PlaneActor::PlaneActor(Game* const pGame)
	: Actor(pGame)
	, mpBox(nullptr)
{
	SetScale(10.f);

	Renderer* const pRenderer = pGame->GetRenderer();

	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = pRenderer->GetMesh("Assets/Plane.gpmesh");
	mc->SetMesh(mesh);

	mpBox = new BoxComponent(this);
	mpBox->SetObjectBox(mesh->GetBox());

	pGame->AddPlane(this);
}

PlaneActor::~PlaneActor()
{
	Game* const pGame = GetGame();

	pGame->RemovePlane(this);
}
