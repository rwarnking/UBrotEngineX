///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: renderer.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERER_H_
#define _RENDERER_H_


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <memory>
//#include <DirectXCollision.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "direct3d.h"
#include "camera.h" // TODO: refactor
#include "../scene.h"
#include "shaderloader.h"


namespace ubrot
{
/////////////
// GLOBALS //
/////////////
const bool VSYNC_ENABLED = false;
const bool FULL_SCREEN = false;
extern float SCREEN_DEPTH;
extern float SCREEN_NEAR;

namespace graphics
{
///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Renderer
/// The renderer is used to initiate rendering of all entities in the scene and supports
/// the following entity types:
///		- Colored
/// TODO: add additional entity types
///////////////////////////////////////////////////////////////////////////////////////////////////
class Renderer
{
public:
	/**
	 * A constructor where all the member pointers are initialised to null.
	 */
	Renderer();
	/**
	 * Empty copy contructor
	 * @param other to copy
	 */
	Renderer(const Renderer &other);
	~Renderer();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	/**
	 * Shuts down by calling shut down for every member (where possible) and freeing allocated
	 * memory.
	 */
	void Shutdown();
	/**
	 * Prepares the backbuffer and subsequently calls \c RenderScene.
	 * @param scene
	 */
	bool Process(Scene &scene);

	/**
	 * Getter for the Direct3D Object
	 */
	Direct3D& GetD3D(); // TODO: do we really want this? maybe the renderer should initialize
						// the scene, but then there are logic problems ...

private:
	/**
	 * Iterates over all tiles and all entities of the scence and then renders them accordingly.
	 * To inrease performance, only entities in the field of view are rendered (frustum culling).
	 * @param scene The scene to render
	 * @param camera The camera to use for rendering (holds necessary matrices)
	 */
	bool RenderScene(Scene &scene, Camera &camera);

private:
	std::unique_ptr<Direct3D> m_direct3d;
	std::unique_ptr<ShaderLoader> m_colorShader;
};
};
};

#endif