///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: scene.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENE_H_
#define _SCENE_H_


//////////////
// INCLUDES //
//////////////
#include <memory>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "graphics/camera.h"
#include "logic/gamelogic.h"
//#include "graphics/model.h"
#include "tile.h"
//#include "entityloader.h"


namespace ubrot
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Scene
/// The scene class manages all components of a renderable scene, including entities and cameras.
/// TODO: keep up-to-date
///////////////////////////////////////////////////////////////////////////////////////////////////
class Scene
{
public:

	/**
	 * A constructor where all the member pointers are initialised to null.
	 */
	Scene();
	/**
	 * TODO fill dat
	 * Empty copy contructor
	 * @param other to copy
	 */
	Scene(const Scene& other);
	~Scene();

	/**
	 * Loads all components of a scene, like the cameras and entities.
	* @param device
	* @return result \c true if successfull
	*/
	bool Initialize(ID3D11Device* device, logic::GameLogic *logic);

	/**
	 * Returns this scene's main camera.
	 * @return reference to m_camera
	 */
	graphics::Camera& GetCamera();
	/**
	 * TODO: check if compatible with DOD
	 * @return reference to tile
	 */
	std::vector<Tile>& GetTiles();

	/**
	* TODO: remove this in the future/adjust for tiles etc.
	* @return reference to m_model
	*/
	std::vector<graphics::vertices::Model>& GetModels();

private:
	std::unique_ptr<graphics::Camera> m_camera;
	// Holds meta information (counts) TODO: unique ptr
	io::SceneMeta m_meta;
	// Holds all tiles of the scene
	std::vector<Tile> m_tiles;
	// Holds all models needed to render the scene
	std::vector<graphics::vertices::Model> m_models;

};
};

#endif