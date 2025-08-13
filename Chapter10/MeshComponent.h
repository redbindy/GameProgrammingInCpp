#pragma once

#include <GL/glew.h>

#include "Component.h"

class Actor;
class Shader;
class Mesh;

class MeshComponent final : public Component
{
public:
	MeshComponent(Actor* const pOwner);
	virtual ~MeshComponent();

	virtual void Draw(Shader* const pShader);

	virtual void SetMesh(Mesh* const pMesh);

	void SetTextureIndex(const size_t index);

	bool IsVisible() const
	{
		return mbVisible;
	}

	void SetVisible(const bool bVisible)
	{
		mbVisible = bVisible;
	}

protected:
	Mesh* mpMesh;

	size_t mTextureIndex;

	bool mbVisible;
};