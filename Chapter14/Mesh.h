#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL3/SDL_log.h>

#include "Math.h"
#include "Collision.h"
#include "VertexArray.h"

class Renderer;
class VertexArray;
class Texture;
class AABB;

class Mesh final
{
public:
	Mesh();
	virtual ~Mesh() = default;

	bool Load(const std::string& fileName, Renderer* const pRenderer);
	bool LoadBinary(const std::string& fileName, Renderer* const pRenderer);
	void Unload();

	inline VertexArray* GetVertexArray();
	inline Texture* GetTexture(const size_t index);

	inline const std::string& GetShaderName() const;

	float GetRadius() const
	{
		return mRadius;
	}

	inline float GetSpecPower() const;

	const AABB& GetBox() const
	{
		return mBox;
	}

	const std::string& GetFileName() const
	{
		return mFileName;
	}

	void SaveBinary(
		const std::string& fileName,
		const void* pVerts,
		const uint32_t numVerts,
		const VertexArray::ELayout layout,
		const uint32_t* pIndices,
		const uint32_t numIndices,
		const std::vector<std::string>& textureNames,
		const AABB& box,
		const float radius
	);

private:
	std::vector<Texture*> mTextures;
	VertexArray* mpVertexArray;

	std::string mShaderName;
	std::string mFileName;

	float mRadius;

	float mSpecPower;

	AABB mBox;
};

inline VertexArray* Mesh::GetVertexArray()
{
	return mpVertexArray;
}

inline Texture* Mesh::GetTexture(const size_t index)
{
	if (index >= mTextures.size())
	{
		return nullptr;
	}

	return mTextures[index];
}

inline const std::string& Mesh::GetShaderName() const
{
	return mShaderName;
}

inline float Mesh::GetSpecPower() const
{
	return mSpecPower;
}
