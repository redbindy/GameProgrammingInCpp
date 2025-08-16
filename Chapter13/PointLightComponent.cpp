#include "PointLightComponent.h"

#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"

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
