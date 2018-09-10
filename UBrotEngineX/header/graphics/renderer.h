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
#include "colorshader.h"
#include "textureshader.h"


namespace ubrot
{
/////////////
// GLOBALS //
/////////////
const bool VSYNC_ENABLED = false;
const bool FULL_SCREEN = !false;
extern float SCREEN_DEPTH;
extern float SCREEN_NEAR;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern int g_mouseX;
extern int g_mouseY;

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

	/**
	* Activates the vertex and index buffers for the input assembler of the GPU which enables
	* this model to be rendered by shaders. This function also sets the topology used to
	* render the model, which is a triangle list (\c IASetPrimitiveTopology).
	* TODO: support different rendering methods and topologies
	* @param deviceContext
	*/
	template <class T>
	void RenderModel(ID3D11DeviceContext* deviceContext, vertices::Model &model);

private:
	std::unique_ptr<Direct3D> m_direct3d;
	std::unique_ptr<ShaderLoader> m_colorShader;
	std::unique_ptr<ColorShader> m_oneColorShader;
	std::unique_ptr<TextureShader> m_textureShader;
};
};
};

#endif