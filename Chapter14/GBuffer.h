#pragma once

#include <vector>
#include <GL/glew.h>

class Texture;

class GBuffer final
{
public:
	enum class EType
	{
		DIFFUSE,
		NORMAL,
		WORLD_POS,
		NUM_GBUFFER_TEXTURES
	};

public:
	GBuffer();
	virtual ~GBuffer() = default;

	bool Create(const int width, const int height);
	void Destroy();

	Texture* GetTexture(const EType type);

	unsigned int GetBufferID() const
	{
		return mBufferID;
	}

	void SetTextureActive();

private:
	std::vector<Texture*> mTextures;

	unsigned int mBufferID;
};