#pragma once

#include "Math.h"
#include "Component.h"

class Actor;
class Shader;
class Mesh;

class PointLightComponent final : public Component
{
public:
	Vector3 DiffuseColor;

	float InnerRadius;
	float OuterRadius;

public:
	PointLightComponent(Actor* const pOwner);
	virtual ~PointLightComponent();

	void Draw(Shader* const pShader, Mesh* const pMesh);
	
	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const override;

	TypeID GetType() const override
	{
		return TPointLightComponent;
	}

private:
};