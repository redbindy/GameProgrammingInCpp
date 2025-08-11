#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <SOIL2/SOIL2.h>

class Texture final
{
public:
	Texture();
	virtual ~Texture() = default;

	bool Load(const std::string& fileName);
	void UnLoad();

	void SetActive();

	inline int GetWidth() const;
	inline int GetHeight() const;

private:
	unsigned int mTextureID;

	int mWidth;
	int mHeight;
};

inline int Texture::GetWidth() const
{
	return mWidth;
}

inline int Texture::GetHeight() const
{
	return mHeight;
}
