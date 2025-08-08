#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <GL/glew.h>

#include "Math.h"

class Game;
class SpriteComponent;
class Texture;
class Mesh;
class Shader;
class VertexArray;
class MeshComponent;

struct DirectionalLight
{
	Vector3 direction;
	Vector3 diffuse;
	Vector3 specColor;
};

class Renderer final
{
public:
	Renderer(Game* const pGame);
	virtual ~Renderer() = default;

	bool Initialize(const float screenWidth, const float screenHeight);
	void Shutdown();

	void Draw();

	void AddSprite(SpriteComponent* const pSprite);
	void RemoveSprite(SpriteComponent* const pSprite);

	Texture* GetTexture(const std::string& fileName);
	Mesh* GetMesh(const std::string& fileName);

	void AddMeshComp(MeshComponent* const pMeshComp);
	void RemoveMeshComp(MeshComponent* const pMeshComp);

	void SetLightUniforms(Shader* const pShader);

	inline void SetViewMatrix(const Matrix4& view);
	inline void SetAmbientLight(const Vector3& ambient);
	inline DirectionalLight& GetDirectionalLight();

private:
	Game* mpGame;

	float mScreenWidth;
	float mScreenHeight;

	SDL_Window* mpWindow;
	SDL_GLContext mContext;

	std::unordered_map<std::string, Texture*> mTextures;
	std::unordered_map<std::string, Mesh*> mMeshes;

	std::vector<SpriteComponent*> mSprites;

	Shader* mpSpriteShader;
	VertexArray* mpSpriteVerts;

	Shader* mpMeshShader;
	std::vector<MeshComponent*> mMeshComps;

	Matrix4 mView;
	Matrix4 mProjection;

	Vector3 mAmbientLight;
	DirectionalLight mDirLight;
};

inline void Renderer::SetViewMatrix(const Matrix4& view)
{
	mView = view;
}

inline void Renderer::SetAmbientLight(const Vector3& ambient)
{
	mAmbientLight = ambient;
}

inline DirectionalLight& Renderer::GetDirectionalLight()
{
	return mDirLight;
}