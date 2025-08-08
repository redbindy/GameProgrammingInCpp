#include "MeshComponent.h"

#include "Actor.h"
#include "Shader.h"
#include "Renderer.h"
#include "Game.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"

MeshComponent::MeshComponent(Actor* const pOwner)
	: Component(pOwner)
	, mpMesh(nullptr)
	, mTextureIndex(0)
{
	Game* const pGame = pOwner->GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	pRenderer->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	Game* const pGame = mpOwner->GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	pRenderer->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* const pShader)
{
	if (mpMesh == nullptr)
	{
		return;
	}

	pShader->SetMatrixUniform("uWorldTransform", mpOwner->GetWorldTransform());
	pShader->SetFloatUniform("uSpecPower", mpMesh->GetSpecPower());

	Texture* const pTexture = mpMesh->GetTexture(mTextureIndex);

	if (pTexture != nullptr)
	{
		pTexture->SetActive();
	}

	VertexArray* const pVA = mpMesh->GetVertexArray();
	pVA->SetActive();

	glDrawElements(GL_TRIANGLES, pVA->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

void MeshComponent::SetMesh(Mesh* const mesh)
{
	mpMesh = mesh;
}

void MeshComponent::SetTextureIndex(const size_t index)
{
	mTextureIndex = index;
}
