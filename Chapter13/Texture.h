#pragma once

#include <string>

#include <SOIL2/SOIL2.h>
#include <SDL3/SDL_log.h>
#include <GL/glew.h>

class Texture final
{
public:
	Texture();
	virtual ~Texture() = default;

	bool Load(const std::string& fileName);
	void Unload();

	void CreateFromSurface(struct SDL_Surface* pSurface);
	void CreateForRendering(const int width, const int height, const unsigned format);

	void SetActive(const int index = 0);

	int GetWidth() const;
	int GetHeight() const;

	int GetTextureID() const
	{
		return mTextureID;
	}

private:
	unsigned int mTextureID;

	int mWidth;
	int mHeight;
};