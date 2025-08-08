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
	virtual void SetMesh(Mesh* const mesh);

	void SetTextureIndex(const size_t index);

private:
	Mesh* mpMesh;
	size_t mTextureIndex;
};