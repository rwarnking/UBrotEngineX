///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: renderer.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../../header/graphics/renderer.h"

using namespace DirectX;

namespace ubrot
{
namespace graphics
{

Renderer::Renderer()
{
	m_direct3d = nullptr;
	m_colorShader = nullptr;
}


Renderer::Renderer(const Renderer& other)
{
}


Renderer::~Renderer()
{
}


bool Renderer::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create and initialize the Direct3D object.
	m_direct3d = std::make_unique<Direct3D>();
	if (!m_direct3d)
	{
		return false;
	}

	result = m_direct3d->Initialize(
		hwnd,
		SCREEN_DEPTH, SCREEN_NEAR,
		screenWidth, screenHeight,
		VSYNC_ENABLED, FULL_SCREEN);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}


	// Create and initialize the color shader.
	m_colorShader = std::make_unique<ShaderLoader>();
	if (!m_colorShader)
	{
		return false;
	}

	m_colorShader->InitializeShader(m_direct3d->GetDevice(), hwnd);

	return true;
}


void Renderer::Shutdown()
{
	if (m_direct3d)
	{
		m_direct3d->Shutdown();
	}

	return;
}


bool Renderer::Process(Scene &scene)
{
	bool result;


	Camera &camera = scene.GetCamera(); // TODO
	camera.Render();

	// Clear the buffers
	m_direct3d->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

	result = RenderScene(scene, camera);

	// Present the rendered scene to the screen.
	m_direct3d->EndScene();

	return true;
}


bool Renderer::RenderScene(Scene &scene, Camera &camera)
{
	XMMATRIX /*, baseViewMatrix, */viewMatrix, projectionMatrix, worldMatrix;//, orthoMatrix;
	bool result;


	// Get the nessessary matrices from the camera and the direct3d object.
	camera.GetViewMatrix(viewMatrix);
	m_direct3d->GetWorldMatrix(worldMatrix);
	m_direct3d->GetProjectionMatrix(projectionMatrix);

	// Render all entities in the scene
	scene.GetModel().Render(m_direct3d->GetDeviceContext());
	result = m_colorShader->Render(
		m_direct3d->GetDeviceContext(),
		worldMatrix, viewMatrix, projectionMatrix,
		scene.GetModel().GetIndexCount()
	);

	return result;
}


Direct3D& Renderer::GetD3D()
{
	return *m_direct3d;
}

}
}
