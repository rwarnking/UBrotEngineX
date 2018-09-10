///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: gamelogic.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../ecs/manager.h"
#include "gameobjects.h"
#include "input.h"


//////////////
// INCLUDES //
//////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: GameLogic
/// TODO
///////////////////////////////////////////////////////////////////////////////////////////////////
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
	std::unique_ptr<Input> m_input = std::make_unique<Input>(); // Hier oder in der erbenden klasse ?

};

};
};