#include "MeshComponent.h"

#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "Shader.h"
#include "SpriteComponent.h"
#include "LevelLoader.h"

MeshComponent::MeshComponent(Actor* const pOwner, const bool bSkeletal)
	: Component(pOwner)
	, mpMesh(nullptr)
	, mTextureIndex(0)
	, mbVisible(true)
	, mbSkeletal(bSkeletal)
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

void MeshComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	Game* const pGame = mpOwner->GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	std::string meshFile;
	if (JsonHelper::GetString(inObj, "meshFile", meshFile))
	{
		SetMesh(pRenderer->GetMesh(meshFile));
	}

	int idx;
	if (JsonHelper::GetInt(inObj, "textureIndex", idx))
	{
		mTextureIndex = static_cast<size_t>(idx);
	}

	JsonHelper::GetBool(inObj, "visible", mbVisible);
	JsonHelper::GetBool(inObj, "isSkeletal", mbSkeletal);
}

void MeshComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);

	if (mpMesh != nullptr)
	{
		JsonHelper::AddString(alloc, inObj, "meshFile", mpMesh->GetFileName());
	}

	JsonHelper::AddInt(alloc, inObj, "textureIndex", static_cast<int>(mTextureIndex));
	JsonHelper::AddBool(alloc, inObj, "visible", mbVisible);
	JsonHelper::AddBool(alloc, inObj, "isSkeletal", mbSkeletal);
}
