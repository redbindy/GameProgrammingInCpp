#include "Texture.h"
#include "Font.h"

Texture::Texture()
	: mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{
}

bool Texture::Load(const std::string& fileName)
{
	mFileName = fileName;

	int channels = 0;
	unsigned char* pImage = SOIL_load_image(
		fileName.c_str(),
		&mWidth,
		&mHeight,
		&channels,
		SOIL_LOAD_AUTO
	);

	if (pImage == nullptr)
	{
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());

		return false;
	}

	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, pImage);

	SOIL_free_image_data(pImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (GLEW_EXT_texture_filter_anisotropic)
	{
		GLfloat largest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
	}

	return true;
}

void Texture::SetActive(const int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::CreateFromSurface(SDL_Surface* pSurface)
{
	mWidth = pSurface->w;
	mHeight = pSurface->h;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, pSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::CreateForRendering(const int width, const int height, const unsigned format)
{
	mWidth = width;
	mHeight = height;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

int Texture::GetWidth() const
{
	return mWidth;
}

int Texture::GetHeight() const
{
	return mHeight;
}
