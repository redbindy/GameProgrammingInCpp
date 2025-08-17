#include "LevelLoader.h"

#include "Renderer.h"
#include "Game.h"
#include "Actor.h"
#include "BallActor.h"
#include "FollowActor.h"
#include "PlaneActor.h"
#include "TargetActor.h"
#include "Component.h"
#include "AudioComponent.h"
#include "BallMove.h"
#include "BoxComponent.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "SkeletalMeshComponent.h"
#include "SpriteComponent.h"
#include "MirrorCamera.h"
#include "PointLightComponent.h"
#include "TargetComponent.h"
#include "FollowCamera.h"

constexpr int LEVEL_VERSION = 1;

std::unordered_map<std::string, ActorFunc> LevelLoader::sActorFactoryMap
{
	{ "Actor", &Actor::Create<Actor> },
	{ "BallActor", &Actor::Create<BallActor> },
	{ "FollowActor", &Actor::Create<FollowActor> },
	{ "PlaneActor", &Actor::Create<PlaneActor> },
	{ "TargetActor", &Actor::Create<TargetActor> },
};

std::unordered_map<std::string, std::pair<int, ComponentFunc>> LevelLoader::sComponentFactoryMap
{
	{ "AudioComponent", { Component::TAudioComponent, &Component::Create<AudioComponent>} },
	{ "BallMove", { Component::TBallMove, &Component::Create<BallMove> } },
	{ "BoxComponent", { Component::TBoxComponent, &Component::Create<BoxComponent> } },
	{ "CameraComponent", { Component::TCameraComponent, &Component::Create<CameraComponent> } },
	{ "FollowCamera", { Component::TFollowCamera, &Component::Create<FollowCamera> } },
	{ "MeshComponent", { Component::TMeshComponent, &Component::Create<MeshComponent> } },
	{ "MoveComponent", { Component::TMoveComponent, &Component::Create<MoveComponent> } },
	{ "SkeletalMeshComponent", { Component::TSkeletalMeshComponent, &Component::Create<SkeletalMeshComponent> } },
	{ "SpriteComponent", { Component::TSpriteComponent, &Component::Create<SpriteComponent> } },
	{ "MirrorCamera", { Component::TMirrorCamera, &Component::Create<MirrorCamera> } },
	{ "PointLightComponent", { Component::TPointLightComponent, &Component::Create<PointLightComponent> }},
	{ "TargetComponent",{ Component::TTargetComponent, &Component::Create<TargetComponent> } },
};

bool LevelLoader::LoadLevel(Game* const pGame, const std::string& fileName)
{
	rapidjson::Document doc;
	if (!LoadJSON(fileName, doc))
	{
		SDL_Log("Failed to load level %s", fileName.c_str());

		return false;
	}

	int version = 0;
	if (!JsonHelper::GetInt(doc, "version", version)
		|| version != LEVEL_VERSION)
	{
		SDL_Log("Incorrect level file version for %s", fileName.c_str());

		return false;
	}

	const rapidjson::Value& globals = doc["globalProperties"];
	if (globals.IsObject())
	{
		loadGlobalProperties(pGame, globals);
	}

	const rapidjson::Value& actors = doc["actors"];
	if (actors.IsArray())
	{
		loadActors(pGame, actors);
	}

	return true;
}

bool LevelLoader::LoadJSON(const std::string& fileName, rapidjson::Document& outDoc)
{
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		SDL_Log("File %s not found", fileName.c_str());

		return false;
	}

	std::ifstream::pos_type fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
	file.read(bytes.data(), static_cast<size_t>(fileSize));

	outDoc.Parse(bytes.data());
	if (!outDoc.IsObject())
	{
		SDL_Log("File %s is not valid JSON", fileName.c_str());

		return false;
	}

	return true;
}

void LevelLoader::SaveLevel(Game* const pGame, const std::string& fileName)
{
	rapidjson::Document doc;
	doc.SetObject();

	JsonHelper::AddInt(doc.GetAllocator(), doc, "version", LEVEL_VERSION);

	rapidjson::Value globals(rapidjson::kObjectType);

	saveGlobalProperties(doc.GetAllocator(), pGame, globals);
	doc.AddMember("globalProperties", globals, doc.GetAllocator());

	rapidjson::Value actors(rapidjson::kArrayType);

	saveActors(doc.GetAllocator(), pGame, actors);
	doc.AddMember("actors", actors, doc.GetAllocator());

	rapidjson::StringBuffer buffer;

	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	const char* output = buffer.GetString();

	std::ofstream outFile(fileName);
	if (outFile.is_open())
	{
		outFile << output;
	}
}

void LevelLoader::loadGlobalProperties(Game* const pGame, const rapidjson::Value& inObject)
{
	Vector3 ambient;

	Renderer* const pRenderer = pGame->GetRenderer();

	if (JsonHelper::GetVector3(inObject, "ambientLight", ambient))
	{
		pRenderer->SetAmbientLight(ambient);
	}

	const rapidjson::Value& dirObj = inObject["directionalLight"];
	if (dirObj.IsObject())
	{
		DirectionalLight& light = pRenderer->GetDirectionalLight();

		JsonHelper::GetVector3(dirObj, "direction", light.direction);
		JsonHelper::GetVector3(dirObj, "color", light.diffuseColor);
	}
}

void LevelLoader::loadActors(Game* const pGame, const rapidjson::Value& inArray)
{
	for (rapidjson::SizeType i = 0; i < inArray.Size(); ++i)
	{
		const rapidjson::Value& actorObj = inArray[i];
		if (actorObj.IsObject())
		{
			std::string type;
			if (JsonHelper::GetString(actorObj, "type", type))
			{
				auto iter = sActorFactoryMap.find(type);
				if (iter != sActorFactoryMap.end())
				{
					Actor* pActor = iter->second(pGame, actorObj["properties"]);

					if (actorObj.HasMember("components"))
					{
						const rapidjson::Value& components = actorObj["components"];
						if (components.IsArray())
						{
							loadComponents(pActor, components);
						}
					}
				}
			}
			else
			{
				SDL_Log("Unknown actor type %s", type.c_str());
			}
		}
	}
}

void LevelLoader::loadComponents(Actor* const pActor, const rapidjson::Value& inArray)
{
	for (rapidjson::SizeType i = 0; i < inArray.Size(); ++i)
	{
		const rapidjson::Value& compObj = inArray[i];
		if (compObj.IsObject())
		{
			std::string type;
			if (JsonHelper::GetString(compObj, "type", type))
			{
				auto iter = sComponentFactoryMap.find(type);
				if (iter != sComponentFactoryMap.end())
				{
					Component::TypeID tId = static_cast<Component::TypeID>(iter->second.first);

					Component* pComp = pActor->GetComponentOfType(tId);
					if (pComp == nullptr)
					{
						pComp = iter->second.second(pActor, compObj["properties"]);
					}
					else
					{
						pComp->LoadProperties(compObj["properties"]);
					}
				}
			}
			else
			{
				SDL_Log("Unknown component type %s", type.c_str());
			}
		}
	}
}

void LevelLoader::saveGlobalProperties(rapidjson::Document::AllocatorType& alloc, Game* const pGame, rapidjson::Value& inObject)
{
	Renderer* const pRenderer = pGame->GetRenderer();
	JsonHelper::AddVector3(alloc, inObject, "ambientLight", pRenderer->GetAmbientLight());

	DirectionalLight& dirLight = pRenderer->GetDirectionalLight();

	rapidjson::Value dirObj(rapidjson::kObjectType);

	JsonHelper::AddVector3(alloc, dirObj, "direction", dirLight.direction);
	JsonHelper::AddVector3(alloc, dirObj, "color", dirLight.diffuseColor);

	inObject.AddMember("directionalLight", dirObj, alloc);
}

void LevelLoader::saveActors(rapidjson::Document::AllocatorType& alloc, Game* const pGame, rapidjson::Value& inArray)
{
	const std::vector<Actor*> actors = pGame->GetActors();
	for (const Actor* const pActor : actors)
	{
		rapidjson::Value obj(rapidjson::kObjectType);

		JsonHelper::AddString(alloc, obj, "type", Actor::TypeNames[pActor->GetType()]);

		rapidjson::Value props(rapidjson::kObjectType);

		pActor->SaveProperties(alloc, props);

		obj.AddMember("properties", props, alloc);

		rapidjson::Value components(rapidjson::kArrayType);
		saveComponents(alloc, pActor, components);

		obj.AddMember("components", components, alloc);

		inArray.PushBack(obj, alloc);
	}
}

void LevelLoader::saveComponents(rapidjson::Document::AllocatorType& alloc, const Actor* const pActor, rapidjson::Value& inArray)
{
	const std::vector<Component*> components = pActor->GetComponents();
	for (const Component* const p : components)
	{
		rapidjson::Value obj(rapidjson::kObjectType);

		JsonHelper::AddString(alloc, obj, "type", Component::TypeNames[p->GetType()]);

		rapidjson::Value props(rapidjson::kObjectType);

		p->SaveProperties(alloc, props);

		obj.AddMember("properties", props, alloc);

		inArray.PushBack(obj, alloc);
	}
}

bool JsonHelper::GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt)
{
	auto iter = inObject.FindMember(inProperty);
	if (iter == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = iter->value;
	if (!property.IsInt())
	{
		return false;
	}

	outInt = property.GetInt();

	return true;
}

bool JsonHelper::GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsDouble())
	{
		return false;
	}

	outFloat = property.GetDouble();

	return true;
}

bool JsonHelper::GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsString())
	{
		return false;
	}

	outStr = property.GetString();

	return true;
}

bool JsonHelper::GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsBool())
	{
		return false;
	}

	outBool = property.GetBool();

	return true;
}

bool JsonHelper::GetVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;
	if (!property.IsArray() || property.Size() != 3)
	{
		return false;
	}

	for (rapidjson::SizeType i = 0; i < 3; i++)
	{
		if (!property[i].IsDouble())
		{
			return false;
		}
	}

	outVector.X = property[0].GetDouble();
	outVector.Y = property[1].GetDouble();
	outVector.Z = property[2].GetDouble();

	return true;
}

bool JsonHelper::GetQuaternion(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat)
{
	auto itr = inObject.FindMember(inProperty);
	if (itr == inObject.MemberEnd())
	{
		return false;
	}

	auto& property = itr->value;

	for (rapidjson::SizeType i = 0; i < 4; i++)
	{
		if (!property[i].IsDouble())
		{
			return false;
		}
	}

	outQuat.X = property[0].GetDouble();
	outQuat.Y = property[1].GetDouble();
	outQuat.Z = property[2].GetDouble();
	outQuat.W = property[3].GetDouble();

	return true;
}

void JsonHelper::AddInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, int value)
{
	rapidjson::Value v(value);

	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddFloat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, float value)
{
	rapidjson::Value v(value);
	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const std::string& value)
{
	rapidjson::Value v;
	v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);

	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddBool(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, bool value)
{
	rapidjson::Value v(value);

	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddVector3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Vector3& value)
{
	rapidjson::Value v(rapidjson::kArrayType);

	v.PushBack(rapidjson::Value(value.X).Move(), alloc);
	v.PushBack(rapidjson::Value(value.Y).Move(), alloc);
	v.PushBack(rapidjson::Value(value.Z).Move(), alloc);

	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}

void JsonHelper::AddQuaternion(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Quaternion& value)
{
	rapidjson::Value v(rapidjson::kArrayType);

	v.PushBack(rapidjson::Value(value.X).Move(), alloc);
	v.PushBack(rapidjson::Value(value.Y).Move(), alloc);
	v.PushBack(rapidjson::Value(value.Z).Move(), alloc);
	v.PushBack(rapidjson::Value(value.W).Move(), alloc);

	inObject.AddMember(rapidjson::StringRef(name), v, alloc);
}
