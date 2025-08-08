#include "Texture.h"

Texture::Texture()
	: mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{
}

bool Texture::Load(const std::string& fileName)
{
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

	glTexImage2D(
		GL_TEXTURE_2D, // �ؽ�ó Ÿ��
		0, // LOD
		format, // open gl ���� ����
		mWidth, 
		mHeight, 
		0, // ���� - 0�̾�� ��
		format, // �Է� ���� ����
		GL_UNSIGNED_BYTE, // ä�� ũ��
		pImage
	);

	SOIL_free_image_data(pImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::UnLoad()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
