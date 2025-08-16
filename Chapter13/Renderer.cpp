#include "Renderer.h"

#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "CameraComponent.h"
#include "Game.h"
#include "UIScreen.h"
#include "SkeletalMeshComponent.h"
#include "GBuffer.h"
#include "PointLightComponent.h"

Renderer::Renderer(Game* const pGame)
	: mTextures()
	, mMeshes()
	, mSprites()
	, mMeshComps()
	, mpGame(pGame)
	, mpSpriteShader(nullptr)
	, mpSpriteVerts(nullptr)
	, mpMeshShader(nullptr)
	, mpSkinnedShader(nullptr)
	, mView()
	, mProjection()
	, mScreenWidth(0.f)
	, mScreenHeight(0.f)
	, mAmbientLight()
	, mDirLight()
	, mpWindow(nullptr)
	, mContext(nullptr)
	, mMirrorBuffer(0)
	, mpMirrorTexture(nullptr)
	, mMirrorView()
	, mpGBuffer(nullptr)
	, mpGGlobalShader(nullptr)
	, mPointLights()
	, mpGPointLightShader(nullptr)
	, mpPointLightMesh(nullptr)
{
}

bool Renderer::Initialize(const float screenWidth, const float screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mpWindow = SDL_CreateWindow("Chapter12", mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
	if (mpWindow == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());

		return false;
	}

	mContext = SDL_GL_CreateContext(mpWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");

		return false;
	}

	glGetError();

	// load shader
	{
		mpSpriteShader = new Shader();
		if (!mpSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
		{
			return false;
		}

		mpSpriteShader->SetActive();

		const Matrix4 spriteViewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
		mpSpriteShader->SetMatrixUniform("uViewProj", spriteViewProj);

		mpMeshShader = new Shader();
		if (!mpMeshShader->Load("Shaders/Phong.vert", "Shaders/GBufferWrite.frag"))
		{
			return false;
		}

		mpMeshShader->SetActive();

		mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
		mProjection = Matrix4::CreatePerspectiveFOV(
			Math::ToRadians(70.f),
			mScreenWidth,
			mScreenHeight,
			10.f,
			10000.f
		);

		mpMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

		mpSkinnedShader = new Shader();
		if (!mpSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/GBufferWrite.frag"))
		{
			return false;
		}

		mpSkinnedShader->SetActive();
		mpSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);

		mpGGlobalShader = new Shader();
		if (!mpGGlobalShader->Load("Shaders/GBufferGlobal.vert", "Shaders/GBufferGlobal.frag"))
		{
			return false;
		}

		mpGGlobalShader->SetActive();
		mpGGlobalShader->SetIntUniform("uGDiffuse", 0);
		mpGGlobalShader->SetIntUniform("uGNormal", 1);
		mpGGlobalShader->SetIntUniform("uGWorldPos", 2);

		mpGGlobalShader->SetMatrixUniform("uViewProj", spriteViewProj);

		const Matrix4 gbufferWorld = Matrix4::CreateScale(mScreenWidth, -mScreenHeight, 1.f);

		mpGGlobalShader->SetMatrixUniform("uWorldTransform", gbufferWorld);

		mpGPointLightShader = new Shader();
		if (!mpGPointLightShader->Load("Shaders/BasicMesh.vert", "Shaders/GBufferPointLight.frag"))
		{
			return false;
		}

		mpGPointLightShader->SetActive();
		mpGPointLightShader->SetIntUniform("uGDiffuse", 0);
		mpGPointLightShader->SetIntUniform("uGNormal", 1);
		mpGPointLightShader->SetIntUniform("uGWorldPos", 2);
		mpGPointLightShader->SetVector2Uniform("uScreenDimensions", Vector2(mScreenWidth, mScreenHeight));
	}

	// create sprite verts
	{
		float vertices[] = {
			-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
			0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
			0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
			-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		mpSpriteVerts = new VertexArray(vertices, 4, VertexArray::ELayout::POS_NORM_TEX, indices, 6);
	}

	if (!createMirrorTarget())
	{
		SDL_Log("Failed to create render target for mirror.");

		return false;
	}

	mpGBuffer = new GBuffer();
	if (!mpGBuffer->Create(screenWidth, screenHeight))
	{
		SDL_Log("Failed to create G-buffer.");

		return false;
	}

	mpPointLightMesh = GetMesh("Assets/PointLight.gpmesh");

	SDL_SetWindowRelativeMouseMode(mpWindow, true);
	SDL_GetRelativeMouseState(nullptr, nullptr);

	return true;
}

void Renderer::Shutdown()
{
	if (mpMirrorTexture != nullptr)
	{
		glDeleteFramebuffers(1, &mMirrorBuffer);
		mpMirrorTexture->Unload();

		delete mpMirrorTexture;
	}

	if (mpGBuffer != nullptr)
	{
		mpGBuffer->Destroy();

		delete mpGBuffer;
	}

	delete mpSpriteVerts;

	mpSpriteShader->Unload();
	delete mpSpriteShader;

	mpMeshShader->Unload();
	delete mpMeshShader;

	SDL_GL_DestroyContext(mContext);
	SDL_DestroyWindow(mpWindow);
}

void Renderer::UnloadData()
{
	for (std::pair<const std::string, Texture*>& pair : mTextures)
	{
		pair.second->Unload();

		delete pair.second;
	}
	mTextures.clear();

	for (std::pair<const std::string, Mesh*>& pair : mMeshes)
	{
		pair.second->Unload();

		delete pair.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	draw3DScene(mMirrorBuffer, mMirrorView, mProjection, 0.25f);
	draw3DScene(mpGBuffer->GetBufferID(), mView, mProjection, 1.f, false);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	drawFromGBuffer();

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	mpSpriteShader->SetActive();
	mpSpriteVerts->SetActive();
	for (SpriteComponent* const p : mSprites)
	{
		if (p->IsVisible())
		{
			p->Draw(mpSpriteShader);
		}
	}

	for (UIScreen* const p : mpGame->GetUIStack())
	{
		p->Draw(mpSpriteShader);
	}

	SDL_GL_SwapWindow(mpWindow);
}

void Renderer::AddSprite(SpriteComponent* const pSprite)
{
	const int order = pSprite->GetDrawOrder();

	std::vector<SpriteComponent*>::iterator iter;
	for (iter = mSprites.begin(); iter != mSprites.end(); ++iter)
	{
		if (order < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mSprites.insert(iter, pSprite);
}

void Renderer::RemoveSprite(SpriteComponent* const pSprite)
{
	std::vector<SpriteComponent*>::iterator iter = std::find(mSprites.begin(), mSprites.end(), pSprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* const pMesh)
{
	if (pMesh->IsSkeletal())
	{
		SkeletalMeshComponent* const pSK = static_cast<SkeletalMeshComponent*>(pMesh);
		mSkeletalMeshes.push_back(pSK);
	}
	else
	{
		mMeshComps.push_back(pMesh);
	}
}

void Renderer::RemoveMeshComp(MeshComponent* const pMesh)
{
	if (pMesh->IsSkeletal())
	{
		SkeletalMeshComponent* const pSK = static_cast<SkeletalMeshComponent*>(pMesh);

		std::vector<SkeletalMeshComponent*>::iterator iter = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), pSK);
		mSkeletalMeshes.erase(iter);
	}
	else
	{
		std::vector<MeshComponent*>::iterator iter = std::find(mMeshComps.begin(), mMeshComps.end(), pMesh);
		mMeshComps.erase(iter);
	}
}

void Renderer::AddPointLight(PointLightComponent* const pLight)
{
	mPointLights.push_back(pLight);
}

void Renderer::RemovePointLight(PointLightComponent* const pLight)
{
	std::vector<PointLightComponent*>::iterator iter = std::find(mPointLights.begin(), mPointLights.end(), pLight);
	mPointLights.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* pTex = nullptr;

	std::unordered_map<std::string, Texture*>::iterator iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		pTex = iter->second;
	}
	else
	{
		pTex = new Texture();
		if (pTex->Load(fileName))
		{
			mTextures.insert({ fileName, pTex });
		}
		else
		{
			delete pTex;
			pTex = nullptr;
		}
	}

	return pTex;
}

Mesh* Renderer::GetMesh(const std::string& fileName)
{
	Mesh* pMesh = nullptr;
	std::unordered_map<std::string, Mesh*>::iterator iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		pMesh = iter->second;
	}
	else
	{
		pMesh = new Mesh();
		if (pMesh->Load(fileName, this))
		{
			mMeshes.insert({ fileName, pMesh });
		}
		else
		{
			delete pMesh;
			pMesh = nullptr;
		}
	}

	return pMesh;
}

void Renderer::SetViewMatrix(const Matrix4& view)
{
	mView = view;
}

void Renderer::SetAmbientLight(const Vector3& ambient)
{
	mAmbientLight = ambient;
}

DirectionalLight& Renderer::GetDirectionalLight()
{
	return mDirLight;
}

Vector3 Renderer::Unproject(const Vector3& screenPoint) const
{
	Vector3 deviceCoord = screenPoint;
	deviceCoord.X /= (mScreenWidth) * 0.5f;
	deviceCoord.Y /= (mScreenHeight) * 0.5f;

	Matrix4 unprojection = mView * mProjection;
	unprojection.Invert();

	return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

void Renderer::GetScreenDirection(Vector3& outStart, Vector3& outDir) const
{
	Vector3 screenPoint(0.f, 0.f, 0.f);

	outStart = Unproject(screenPoint);
	screenPoint.Z = 0.9f;

	const Vector3 end = Unproject(screenPoint);

	outDir = end - outStart;
	outDir.Normalize();
}

void Renderer::setLightUniforms(Shader* const pShader, const Matrix4& view)
{
	Matrix4 invView = view;
	invView.Invert();

	pShader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	pShader->SetVectorUniform("uAmbientLight", mAmbientLight);
	pShader->SetVectorUniform("uDirLight.direction", mDirLight.direction);
	pShader->SetVectorUniform("uDirLight.diffuseColor", mDirLight.diffuseColor);
	pShader->SetVectorUniform("uDirLight.specColor", mDirLight.specColor);
}

bool Renderer::createMirrorTarget()
{
	const float width = mScreenWidth * 0.25f;
	const float height = mScreenHeight * 0.25f;

	glGenFramebuffers(1, &mMirrorBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mMirrorBuffer);

	mpMirrorTexture = new Texture();
	mpMirrorTexture->CreateForRendering(width, height, GL_RGB);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mpMirrorTexture->GetTextureID(), 0);

	const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		glDeleteFramebuffers(1, &mMirrorBuffer);
		mpMirrorTexture->Unload();

		delete mpMirrorTexture;

		mpMirrorTexture = nullptr;

		return false;
	}

	return true;
}

void Renderer::draw3DScene(const unsigned int framebuffer, const Matrix4& view, const Matrix4& proj, const float viewPortScale, const bool bLit)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glViewport(0, 0, mScreenWidth * viewPortScale, mScreenHeight * viewPortScale);

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	const Matrix4 viewProj = view * proj;

	mpMeshShader->SetActive();
	mpMeshShader->SetMatrixUniform("uViewProj", viewProj);

	if (bLit)
	{
		setLightUniforms(mpMeshShader, view);
	}

	for (MeshComponent* const p : mMeshComps)
	{
		if (p->IsVisible())
		{
			p->Draw(mpMeshShader);
		}
	}

	mpSkinnedShader->SetActive();

	mpSkinnedShader->SetMatrixUniform("uViewProj", viewProj);

	if (bLit)
	{
		setLightUniforms(mpSkinnedShader, view);
	}

	for (SkeletalMeshComponent* p : mSkeletalMeshes)
	{
		if (p->IsVisible())
		{
			p->Draw(mpSkinnedShader);
		}
	}
}

void Renderer::drawFromGBuffer()
{
	glDisable(GL_DEPTH_TEST);

	mpGGlobalShader->SetActive();

	mpSpriteVerts->SetActive();

	mpGBuffer->SetTextureActive();

	setLightUniforms(mpGGlobalShader, mView);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, mpGBuffer->GetBufferID());

	const int width = static_cast<int>(mScreenWidth);
	const int height = static_cast<int>(mScreenHeight);

	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	mpGPointLightShader->SetActive();

	VertexArray* const pVA = mpPointLightMesh->GetVertexArray();
	pVA->SetActive();

	mpGPointLightShader->SetMatrixUniform("uViewProj", mView * mProjection);

	mpGBuffer->SetTextureActive();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	for (PointLightComponent* p : mPointLights)
	{
		p->Draw(mpGPointLightShader, mpPointLightMesh);
	}
}
