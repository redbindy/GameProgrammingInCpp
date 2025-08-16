#pragma once

#include "Math.h"
#include "Component.h"

class Actor;
class Shader;
class Mesh;

class PointLightComponent final : Component
{
public:
	Vector3 DiffuseColor;

	float InnerRadius;
	float OuterRadius;

public:
	PointLightComponent(Actor* const pOwner);
	virtual ~PointLightComponent();

	void Draw(Shader* const pShader, Mesh* const pMesh);
	
private:
};