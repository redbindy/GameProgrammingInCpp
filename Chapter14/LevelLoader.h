#pragma once

#include <string>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <SDL3/SDL_log.h>
#include <functional>
#include <unordered_map>

#include "Math.h"

class Game;
class Actor;
class Component;

using ActorFunc = std::function<Actor* (Game* const, const rapidjson::Value&)>;
using ComponentFunc = std::function<Component* (Actor* const, const rapidjson::Value&)>;

class LevelLoader final
{
public:
	static bool LoadLevel(Game* const pGame, const std::string& fileName);
	static bool LoadJSON(const std::string& fileName, rapidjson::Document& outDoc);
	static void SaveLevel(Game* const pGame, const std::string& fileName);

protected:
	static std::unordered_map<std::string, ActorFunc> sActorFactoryMap;
	static std::unordered_map<std::string, std::pair<int, ComponentFunc>> sComponentFactoryMap;

protected:
	static void loadGlobalProperties(Game* const pGame, const rapidjson::Value& inObject);
	static void loadActors(Game* const pGame, const rapidjson::Value& inArray);
	static void loadComponents(Actor* const pActor, const rapidjson::Value& inArray);

	static void saveGlobalProperties(rapidjson::Document::AllocatorType& alloc, Game* const pGame, rapidjson::Value& inObject);
	static void saveActors(rapidjson::Document::AllocatorType& alloc, Game* const pGame, rapidjson::Value& inArray);
	static void saveComponents(rapidjson::Document::AllocatorType& alloc, const Actor* const pActor, rapidjson::Value& inArray);
};

class JsonHelper final
{
public:
	static bool GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
	static bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
	static bool GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outStr);
	static bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
	static bool GetVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector);
	static bool GetQuaternion(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat);

	static void AddInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, int value);
	static void AddFloat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, float value);
	static void AddString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const std::string& value);
	static void AddBool(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, bool value);
	static void AddVector3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Vector3& value);
	static void AddQuaternion(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObject, const char* name, const Quaternion& value);
};