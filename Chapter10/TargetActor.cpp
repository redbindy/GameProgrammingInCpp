#include "TargetActor.h"

#include "MeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "BoxComponent.h"
#include "Game.h"

TargetActor::TargetActor(Game* const pGame)
	: Actor(pGame)
{
	SetRotation(Quaternion(Vector3::UnitZ, Math::PI));

	MeshComponent* mc = new MeshComponent(this);

	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Target.gpmesh");
	mc->SetMesh(mesh);

	BoxComponent* bc = new BoxComponent(this);
	bc->SetObjectBox(mesh->GetBox());
}
