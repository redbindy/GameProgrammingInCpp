#include "PointLightComponent.h"

#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include "LevelLoader.h"

PointLightComponent::PointLightComponent(Actor* const pOwner)
	: Component(pOwner)
	, DiffuseColor()
	, InnerRadius(0.f)
	, OuterRadius(0.f)
{
	Game* const pGame = pOwner->GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	pRenderer->AddPointLight(this);
}

PointLightComponent::~PointLightComponent()
{
	Game* const pGame = mpOwner->GetGame();
	Renderer* const pRenderer = pGame->GetRenderer();

	pRenderer->RemovePointLight(this);
}

void PointLightComponent::Draw(Shader* const pShader, Mesh* const pMesh)
{
	const Matrix4 scale = Matrix4::CreateScale(mpOwner->GetScale() * OuterRadius / pMesh->GetRadius());
	const Matrix4 trans = Matrix4::CreateTranslation(mpOwner->GetPosition());
	const Matrix4 worldTransform = scale * trans;

	pShader->SetMatrixUniform("uWorldTransform", worldTransform);
	pShader->SetVectorUniform("uPointLight.worldPos", mpOwner->GetPosition());
	pShader->SetVectorUniform("uPointLight.diffuseColor", DiffuseColor);
	pShader->SetFloatUniform("uPointLight.innerRadius", InnerRadius);
	pShader->SetFloatUniform("uPointLight.outerRadius", OuterRadius);

	VertexArray* const pVA = pMesh->GetVertexArray();
	glDrawElements(GL_TRIANGLES, pVA->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

void PointLightComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	JsonHelper::GetVector3(inObj, "color", DiffuseColor);
	JsonHelper::GetFloat(inObj, "innerRadius", InnerRadius);
	JsonHelper::GetFloat(inObj, "outerRadius", OuterRadius);
}

void PointLightComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	JsonHelper::AddVector3(alloc, inObj, "color", DiffuseColor);
	JsonHelper::AddFloat(alloc, inObj, "innerRadius", InnerRadius);
	JsonHelper::AddFloat(alloc, inObj, "outerRadius", OuterRadius);
}
