#include "Mesh.h"

#include "Texture.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Shader.h"

constexpr size_t VERT_SIZE = 8;

Mesh::Mesh()
	: mTextures()
	, mpVertexArray(nullptr)
	, mShaderName()
	, mRadius(0.f)
	, mSpecPower(100.f)
	, mBox(Vector3::Infinity, Vector3::NegInfinity)
{
}

bool Mesh::Load(const std::string& fileName, Renderer* const pRenderer)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Mesh %s", fileName.c_str());

		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();

	std::string contents = fileStream.str();

	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Mesh %s is not valid json", fileName.c_str());

		return false;
	}

	const int version = doc["version"].GetInt();
	if (version != 1)
	{
		SDL_Log("Mesh %s not version 1", fileName.c_str());

		return false;
	}

	mShaderName = doc["shader"].GetString();
	mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		SDL_Log("Mesh %s has no textures, there should be at least one", fileName.c_str());

		return false;
	}

	for (rapidjson::SizeType i = 0; i < textures.Size(); ++i)
	{
		std::string texName = textures[i].GetString();

		Texture* pTex = pRenderer->GetTexture(texName);
		if (pTex == nullptr)
		{
			pTex = pRenderer->GetTexture("Assets/Default.png");
		}
		mTextures.push_back(pTex);
	}

	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no vertices", fileName.c_str());

		return false;
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * VERT_SIZE);

	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); ++i)
	{
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != VERT_SIZE)
		{
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());

			return false;
		}

		const Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		mRadius = Math::Max(mRadius, pos.LengthSq());
		mBox.UpdateMinMax(pos);

		for (rapidjson::SizeType i = 0; i < vert.Size(); ++i)
		{
			vertices.push_back(static_cast<float>(vert[i].GetDouble()));
		}
	}

	mRadius = Math::Sqrt(mRadius);

	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName.c_str());

		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);

	for (rapidjson::SizeType i = 0; i < indJson.Size(); ++i)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName.c_str());

			return false;
		}

		indices.push_back(ind[0].GetUint());
		indices.push_back(ind[1].GetUint());
		indices.push_back(ind[2].GetUint());
	}

	mpVertexArray = new VertexArray(
		vertices.data(),
		static_cast<unsigned int>(vertices.size() / VERT_SIZE),
		indices.data(),
		static_cast<unsigned int>(indices.size())
	);

	return true;
}

void Mesh::Unload()
{
	delete mpVertexArray;
	mpVertexArray = nullptr;
}
