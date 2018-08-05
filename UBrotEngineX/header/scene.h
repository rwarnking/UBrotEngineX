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
#include "graphics/model.h"

/////////////
// GLOBALS //
/////////////


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
	bool Initialize(ID3D11Device* device);

	/**
	 * Returns this scene's main camera.
	 * @return reference to m_camera
	 */
	Camera& GetCamera();
	/**
	 * TODO: remove this in the future/adjust for tiles etc.
	 * @return reference to m_model
	 */
	Model& GetModel();

private:
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Model> m_model;
};

#endif