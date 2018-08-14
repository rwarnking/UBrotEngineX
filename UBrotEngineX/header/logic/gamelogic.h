#pragma once

#include "../ecs/manager.h"
#include "gameobjects.h"

#include <rapidjson/document.h>

namespace ubrot
{

class Scene;
class Tile;
namespace io
{
struct AssetFiles;
};

namespace logic
{

class GameLogic
{
public:

	virtual ~GameLogic() {}

	virtual void HandleComponent(
		ecs::Manager<AllSettings> &mgr,
		ecs::EntityIndex &entity,
		const std::string &name,
		rapidjson::GenericValue<rapidjson::UTF8<char>> &comp
	) = 0;
	virtual void HandleTag(
		ecs::Manager<AllSettings> &mgr,
		ecs::EntityIndex &entity,
		const std::string &tag
	) = 0;

	virtual void RegisterEntities(ubrot::Tile &tile, io::AssetFiles &assetBits) = 0;

	virtual void Process(ubrot::Scene &scene) = 0;

protected:

	// TODO: implement input class/namespace ?
	// Input m_input;
};

};
};