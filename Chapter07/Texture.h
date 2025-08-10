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

	void SetActive();

	int GetWidth() const;
	int GetHeight() const;

private:
	unsigned int mTextureID;

	int mWidth;
	int mHeight;
};