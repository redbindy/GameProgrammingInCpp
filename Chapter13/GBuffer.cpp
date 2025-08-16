#include "GBuffer.h"

#include "Texture.h"

GBuffer::GBuffer()
	: mTextures()
	, mBufferID(0)
{
}

bool GBuffer::Create(const int width, const int height)
{
	glGenFramebuffers(1, &mBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	for (int i = 0; i < static_cast<int>(EType::NUM_GBUFFER_TEXTURES); ++i)
	{
		Texture* pTex = new Texture();
		pTex->CreateForRendering(width, height, GL_RGB32F);

		mTextures.push_back(pTex);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, pTex->GetTextureID(), 0);
	}

	std::vector<GLenum> attachments;
	for (int i = 0; i < static_cast<int>(EType::NUM_GBUFFER_TEXTURES); ++i)
	{
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Destroy();

		return false;
	}

	return true;
}

void GBuffer::Destroy()
{
	glDeleteFramebuffers(1, &mBufferID);
	for (Texture* const pTex : mTextures)
	{
		pTex->Unload();

		delete pTex;
	}
}

Texture* GBuffer::GetTexture(const EType type)
{
	if (mTextures.size() > 0)
	{
		return mTextures[static_cast<int>(type)];
	}

	return nullptr;
}

void GBuffer::SetTextureActive()
{
	for (int i = 0; i < static_cast<int>(EType::NUM_GBUFFER_TEXTURES); ++i)
	{
		mTextures[i]->SetActive(i);
	}
}
