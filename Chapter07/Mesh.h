#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL3/SDL_log.h>

#include "Math.h"

class Renderer;
class VertexArray;
class Texture;

class Mesh final
{
public:
	Mesh();
	virtual ~Mesh() = default;

	bool Load(const std::string& fileName, Renderer* const pRenderer);
	void Unload();

	inline VertexArray* GetVertexArray();
	inline Texture* GetTexture(const size_t index);

	inline const std::string& GetShaderName() const;

	inline float GetRadius() const;

	inline float GetSpecPower() const;

private:
	std::vector<Texture*> mTextures;
	VertexArray* mpVertexArray;

	std::string mShaderName;

	float mRadius;

	float mSpecPower;
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