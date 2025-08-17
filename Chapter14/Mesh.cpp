#include "Mesh.h"

#include "Texture.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Shader.h"

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};

	constexpr int BIN_VERSION = 1;
	struct MeshBinHeader
	{
		char signature[4] = { 'G', 'M', 'S', 'H' };

		uint32_t version = BIN_VERSION;

		VertexArray::ELayout layout = VertexArray::ELayout::POS_NORM_TEX;

		uint32_t numTextures = 0;
		uint32_t numVerts = 0;
		uint32_t numIndices = 0;

		AABB box = { Vector3::Zero, Vector3::Zero };

		float radius = 0.f;
		float specPower = 100.f;
	};
}

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
	mFileName = fileName;

	if (LoadBinary(fileName + ".bin", pRenderer))
	{
		return true;
	}

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

	size_t vertSize = 8;
	VertexArray::ELayout layout = VertexArray::ELayout::POS_NORM_TEX;

	std::string vertexFormat = doc["vertexformat"].GetString();
	if (vertexFormat == "PosNormSkinTex")
	{
		layout = VertexArray::ELayout::POS_NORM_SKIN_TEX;
		vertSize = 10;
	}

	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no vertices", fileName.c_str());

		return false;
	}

	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);

	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); ++i)
	{
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray())
		{
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());

			return false;
		}

		const Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		mRadius = Math::Max(mRadius, pos.LengthSq());
		mBox.UpdateMinMax(pos);

		if (layout == VertexArray::ELayout::POS_NORM_TEX)
		{
			Vertex v;
			for (rapidjson::SizeType j = 0; j < vert.Size(); ++j)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.push_back(v);
			}
		}
		else
		{
			Vertex v;
			for (rapidjson::SizeType j = 0; j < 6; ++j)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.push_back(v);
			}

			for (rapidjson::SizeType j = 6; j < 14; j += 4)
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();

				vertices.push_back(v);
			}

			for (rapidjson::SizeType j = 14; j < vert.Size(); ++j)
			{
				v.f = vert[j].GetDouble();

				vertices.push_back(v);
			}
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
		static_cast<unsigned int>(vertices.size() / vertSize),
		layout,
		indices.data(),
		static_cast<unsigned int>(indices.size())
	);

	return true;
}

bool Mesh::LoadBinary(const std::string& fileName, Renderer* const pRenderer)
{
	std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
	if (inFile.is_open())
	{
		MeshBinHeader header;
		inFile.read(reinterpret_cast<char*>(&header), sizeof(header));

		char* sig = header.signature;
		if (sig[0] != 'G'
			|| sig[1] != 'M'
			|| sig[2] != 'S'
			|| sig[3] != 'H'
			|| header.version != BIN_VERSION)
		{
			return false;
		}

		for (uint32_t i = 0; i < header.numTextures; ++i)
		{
			uint16_t nameSize = 0;
			inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));

			char* pTexName = new char[nameSize + 1];
			inFile.read(pTexName, nameSize);

			Texture* pTex = pRenderer->GetTexture(pTexName);
			if (pTex == nullptr)
			{
				pTex = pRenderer->GetTexture("Assets/Default.png");
			}
			mTextures.push_back(pTex);

			delete[] pTexName;
		}

		unsigned int vertexSize = VertexArray::GetVertexSize(header.layout);
		char* pVerts = new char[header.numVerts * vertexSize];
		inFile.read(pVerts, header.numVerts * vertexSize);

		uint32_t* pIndices = new uint32_t[header.numIndices];
		inFile.read(reinterpret_cast<char*>(pIndices), header.numIndices * sizeof(uint32_t));

		mpVertexArray = new VertexArray(pVerts, header.numVerts, header.layout, pIndices, header.numIndices);

		delete[] pVerts;
		delete[] pIndices;

		mBox = header.box;
		mRadius = header.radius;
		mSpecPower = header.specPower;

		return true;
	}

	return false;
}

void Mesh::Unload()
{
	delete mpVertexArray;
	mpVertexArray = nullptr;
}

void Mesh::SaveBinary(
	const std::string& fileName,
	const void* pVerts,
	const uint32_t numVerts,
	const VertexArray::ELayout layout,
	const uint32_t* pIndices,
	const uint32_t numIndices,
	const std::vector<std::string>& textureNames,
	const AABB& box,
	const float radius
)
{
	MeshBinHeader header;
	header.layout = layout;
	header.numTextures = static_cast<unsigned int>(textureNames.size());
	header.numVerts = numVerts;
	header.numIndices = numIndices;
	header.box = box;
	header.radius = radius;

	std::ofstream outFile(fileName, std::ios::out | std::ios::binary);
	if (outFile.is_open())
	{
		outFile.write(reinterpret_cast<char*>(&header), sizeof(header));

		for (const std::string& tex : textureNames)
		{
			uint16_t nameSize = static_cast<uint16_t>(tex.length()) + 1;
			outFile.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));

			outFile.write(tex.c_str(), nameSize - 1);
			outFile.write("\0", 1);
		}

		unsigned int vertexSize = VertexArray::GetVertexSize(layout);

		outFile.write(reinterpret_cast<const char*>(pVerts), numVerts * vertexSize);
		outFile.write(reinterpret_cast<const char*>(pIndices), numIndices * sizeof(uint32_t));
	}
}