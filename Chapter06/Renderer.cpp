#include "Renderer.h"

#include "Game.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Mesh.h"

Renderer::Renderer(Game* const pGame)
	: mpGame(pGame)
	, mScreenWidth(0.f)
	, mScreenHeight(0.f)
	, mpWindow(nullptr)
	, mContext(nullptr)
	, mTextures()
	, mMeshes()
	, mSprites()
	, mpSpriteShader(nullptr)
	, mpSpriteVerts(nullptr)
	, mpMeshShader(nullptr)
	, mView()
	, mProjection()
	, mAmbientLight(0.2f, 0.2f, 0.2f)
	, mDirLight{}
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

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mpWindow = SDL_CreateWindow("Chapter06", mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
	if (mpWindow == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());

		return false;
	}

	mContext = SDL_GL_CreateContext(mpWindow);
	if (mContext == nullptr)
	{
		SDL_Log("%s", SDL_GetError());

		return false;
	}

	glewExperimental = GL_TRUE;
	GLenum eResult = glewInit();
	if (eResult != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW: %s", glewGetErrorString(eResult));

		return false;
	}

	glGetError();

	// load shader
	{
		mpSpriteShader = new Shader();
		if (!mpSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
		{
			SDL_Log("Failed to load Shaders.");

			return false;
		}

		mpSpriteShader->SetActive();

		const Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
		mpSpriteShader->SetMatrixUniform("uViewProj", viewProj);

		mpMeshShader = new Shader();
		if (!mpMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
		{
			SDL_Log("Failed to load Shaders.");

			return false;
		}

		mpMeshShader->SetActive();

		mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
		mProjection = Matrix4::CreatePerspectiveFOV(
			Math::ToRadians(70.f),
			mScreenWidth,
			mScreenHeight,
			25.f,
			10000.f
		);

		mpMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
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

		mpSpriteVerts = new VertexArray(vertices, 4, indices, 6);
	}

	return true;
}

void Renderer::Shutdown()
{
	// unload data
	{
		for (std::pair<const std::string, Texture*>& pair : mTextures)
		{
			pair.second->UnLoad();

			delete pair.second;
		}
		mTextures.clear();

		for (std::pair<const std::string, Mesh*>& p : mMeshes)
		{
			p.second->Unload();

			delete p.second;
		}
		mMeshes.clear();
	}

	mpMeshShader->Unload();
	delete mpMeshShader;

	delete mpSpriteVerts;

	mpSpriteShader->Unload();
	delete mpSpriteShader;

	SDL_GL_DestroyContext(mContext);
	SDL_DestroyWindow(mpWindow);
}

void Renderer::Draw()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	mpMeshShader->SetActive();
	mpMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	SetLightUniforms(mpMeshShader);
	for (MeshComponent* const p : mMeshComps)
	{
		p->Draw(mpMeshShader);
	}

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	mpSpriteShader->SetActive();
	mpSpriteVerts->SetActive();

	for (SpriteComponent* const p : mSprites)
	{
		p->Draw(mpSpriteShader);
	}

	SDL_GL_SwapWindow(mpWindow);
}

void Renderer::AddSprite(SpriteComponent* const pSprite)
{
	const int drawOrder = pSprite->GetDrawOrder();

	std::vector<SpriteComponent*>::iterator iter = mSprites.begin();

	while (iter != mSprites.end())
	{
		if ((*iter)->GetDrawOrder() > drawOrder)
		{
			break;
		}

		++iter;
	}

	mSprites.insert(iter, pSprite);
}

void Renderer::RemoveSprite(SpriteComponent* const pSprite)
{
	std::vector<SpriteComponent*>::iterator iter = std::find(mSprites.begin(), mSprites.end(), pSprite);
	mSprites.erase(iter);
}


Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* pTexture = nullptr;
	std::unordered_map<std::string, Texture*>::iterator iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		pTexture = iter->second;
	}
	else
	{
		pTexture = new Texture();
		if (pTexture->Load(fileName))
		{
			mTextures.insert({ fileName, pTexture });
		}
		else
		{
			delete pTexture;

			pTexture = nullptr;
		}
	}

	return pTexture;
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
		if (pMesh->Load(fileName, mpGame))
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

void Renderer::AddMeshComp(MeshComponent* const pMeshComp)
{
	mMeshComps.push_back(pMeshComp);
}

void Renderer::RemoveMeshComp(MeshComponent* const pMeshComp)
{
	std::vector<MeshComponent*>::iterator iter = std::find(mMeshComps.begin(), mMeshComps.end(), pMeshComp);
	mMeshComps.erase(iter);
}

void Renderer::SetLightUniforms(Shader* const pShader)
{
	Matrix4 invView = mView;
	invView.Invert();

	pShader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	pShader->SetVectorUniform("uAmbientLight", mAmbientLight);

	pShader->SetVectorUniform("uDirLight.direction", mDirLight.direction);
	pShader->SetVectorUniform("uDirLight.diffuseColor", mDirLight.diffuse);
	pShader->SetVectorUniform("uDirLight.specColor", mDirLight.specColor);
}
