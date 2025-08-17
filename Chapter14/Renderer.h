#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

#include <SDL3/SDL.h>
#include <GL/glew.h>

#include "Math.h"

struct DirectionalLight
{
	Vector3 direction;
	Vector3 diffuseColor;
	Vector3 specColor;
};

class Game;
class SpriteComponent;
class Mesh;
class MeshComponent;
class Texture;
class Shader;
class VertexArray;
class SkeletalMeshComponent;
class GBuffer;
class PointLightComponent;

class Renderer final
{
public:
	Renderer(Game* const pGame);
	virtual ~Renderer() = default;

	bool Initialize(const float screenWidth, const float screenHeight);

	void Shutdown();
	void UnloadData();

	void Draw();

	void AddSprite(SpriteComponent* const pSprite);
	void RemoveSprite(SpriteComponent* const pSprite);

	void AddMeshComp(MeshComponent* const pMesh);
	void RemoveMeshComp(MeshComponent* const pMesh);

	void AddPointLight(PointLightComponent* const pLight);
	void RemovePointLight(PointLightComponent* const pLight);

	Texture* GetTexture(const std::string& fileName);
	Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view);

	void SetAmbientLight(const Vector3& ambient);

	DirectionalLight& GetDirectionalLight();

	inline float GetScreenWidth();
	inline float GetScreenHeight();

	SDL_Window* GetSDLWindow()
	{
		return mpWindow;
	}

	Vector3 Unproject(const Vector3& screenPoint) const;

	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;

	Texture* GetMirrorTexture()
	{
		return mpMirrorTexture;
	}

	void SetMirrorView(const Matrix4& view)
	{
		mMirrorView = view;
	}

	Vector3 GetAmbientLight() const
	{
		return mAmbientLight;
	}

private:
	std::unordered_map<std::string, Texture*> mTextures;
	std::unordered_map<std::string, Mesh*> mMeshes;

	std::vector<SpriteComponent*> mSprites;
	std::vector<MeshComponent*> mMeshComps;
	std::vector<SkeletalMeshComponent*> mSkeletalMeshes;

	Game* mpGame;

	Shader* mpSpriteShader;
	VertexArray* mpSpriteVerts;

	Shader* mpMeshShader;
	Shader* mpSkinnedShader;

	Matrix4 mView;
	Matrix4 mProjection;

	float mScreenWidth;
	float mScreenHeight;

	Vector3 mAmbientLight;
	DirectionalLight mDirLight;

	SDL_Window* mpWindow;
	SDL_GLContext mContext;

	unsigned int mMirrorBuffer;
	Texture* mpMirrorTexture;
	Matrix4 mMirrorView;

	GBuffer* mpGBuffer;
	Shader* mpGGlobalShader;

	std::vector<PointLightComponent*> mPointLights;
	Shader* mpGPointLightShader;
	Mesh* mpPointLightMesh;

private:
	void setLightUniforms(Shader* const pShader, const Matrix4& view);
	bool createMirrorTarget();
	void draw3DScene(const unsigned int framebuffer, const Matrix4& view, const Matrix4& proj, const float viewPortScale = 1.f, const bool bLit = true);
	void drawFromGBuffer();
};

inline float Renderer::GetScreenWidth()
{
	return mScreenWidth;
}

inline float Renderer::GetScreenHeight()
{
	return mScreenHeight;
}