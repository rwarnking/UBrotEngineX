///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: tile.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _TILE_H_
#define _TILE_H_


//////////////
// INCLUDES //
//////////////
#include <vector>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "graphics/model.h"
#include "ecs/manager.h"
//#include "entityloader.h"


namespace ubrot
{
///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Tile
/// TODO
///////////////////////////////////////////////////////////////////////////////////////////////////
class Tile
{
public:

	Tile();
	//Tile(const Tile &other);
	//Tile(Tile &&other);

	/**
	* Loads all entities of a tile
	* @param device
	* @return result \c true if successfull
	*/
	bool Initialize(ID3D11Device* device);

	void Refresh();

	/**
	* TODO: remove this in the future/adjust for tiles etc.
	* @return reference to m_model
	*/
	std::vector<graphics::Model>& GetModels();

	ecs::Manager<ecs::MySettings>& GetManager();

private:
	// entitiy vector
	std::vector<graphics::Model> m_model;
	ecs::Manager<ecs::MySettings> m_mgr; // TODO unique pointer

	// component vectors
};
};

#endif // !_TILE_H_
