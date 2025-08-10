#include "Renderer.h"

#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"

Renderer::Renderer(Game* const pGame)
	: mTextures()
	, mMeshes()
	, mSprites()
	, mMeshComps()
	, mpGame(pGame)
	, mpSpriteShader(nullptr)
	, mpSpriteVerts(nullptr)
	, mpMeshShader(nullptr)
	, mView()
	, mProjection()
	, mScreenWidth(0.f)
	, mScreenHeight(0.f)
	, mAmbientLight()
	, mDirLight()
	, mpWindow(nullptr)
	, mContext(nullptr)
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

	mpWindow = SDL_CreateWindow("Chapter07", mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
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

		const Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
		mpSpriteShader->SetMatrixUniform("uViewProj", viewProj);

		mpMeshShader = new Shader();
		if (!mpMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
		{
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
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	mpMeshShader->SetActive();
	mpMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// Set light uniforms
	{
		Matrix4 invView = mView;
		invView.Invert();

		mpMeshShader->SetVectorUniform("uCameraPos", invView.GetTranslation());
		mpMeshShader->SetVectorUniform("uAmbientLight", mAmbientLight);
		mpMeshShader->SetVectorUniform("uDirLight.direction", mDirLight.direction);
		mpMeshShader->SetVectorUniform("uDirLight.diffuseColor", mDirLight.diffuseColor);
		mpMeshShader->SetVectorUniform("uDirLight.specColor", mDirLight.specColor);
	}

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
	mMeshComps.push_back(pMesh);
}

void Renderer::RemoveMeshComp(MeshComponent* const pMesh)
{
	std::vector<MeshComponent*>::iterator iter = std::find(mMeshComps.begin(), mMeshComps.end(), pMesh);
	mMeshComps.erase(iter);
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

inline float Renderer::GetScreenWidth()
{
	return mScreenWidth;
}

inline float Renderer::GetScreenHeight()
{
	return mScreenHeight;
}
