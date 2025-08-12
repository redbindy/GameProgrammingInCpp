#include "MeshComponent.h"

#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "Shader.h"
#include "SpriteComponent.h"

MeshComponent::MeshComponent(Actor* const pOwner)
	: Component(pOwner)
	, mpMesh(nullptr)
	, mTextureIndex(0)
	, mbVisible(true)
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

	Texture* const pTex = mpMesh->GetTexture(mTextureIndex);
	if (pTex != nullptr)
	{
		pTex->SetActive();
	}

	VertexArray* const pVA = mpMesh->GetVertexArray();
	pVA->SetActive();

	glDrawElements(GL_TRIANGLES, pVA->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

void MeshComponent::SetMesh(Mesh* const pMesh)
{
	mpMesh = pMesh;
}

void MeshComponent::SetTextureIndex(const size_t index)
{
	mTextureIndex = index;
}
