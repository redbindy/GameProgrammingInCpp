#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <SDL3/SDL.h>
#include <rapidjson/document.h>

#include "Math.h"

class Game;
class VertexArray;
class Texture;

class Mesh final
{
public:
	Mesh();
	virtual ~Mesh() = default;

	bool Load(const std::string& fileName, Game* const pGame);
	void Unload();

	inline VertexArray* GetVertexArray();
	inline Texture* GetTexture(const size_t index);

	inline const std::string& GetShaderName() const;

	inline float GetRadius() const;
	inline float GetSpecPower() const;

private:
	VertexArray* mpVertexArray;

	std::vector<Texture*> mTextures;

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