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
	bool result;


	// Get the nessessary matrices from the camera and the direct3d object.
	auto viewMatrix = camera.GetViewMatrix();
	auto worldMatrix = m_direct3d->GetWorldMatrix();
	auto projectionMatrix = m_direct3d->GetProjectionMatrix();

	auto& model = scene.GetModels();

	// Render all entities in the scene
	for (auto& tile : scene.GetTiles())
	{
		/*
		for (auto& model : tile.GetModels())
		{
			model.Render(m_direct3d->GetDeviceContext());
			result = m_colorShader->Render(
				m_direct3d->GetDeviceContext(),
				worldMatrix, viewMatrix, projectionMatrix,
				model.GetIndexCount()
			);
		}*/

		// On the first iteration, systems are not applied to entities because
		// the entity size of the manager is only updated at the end of the frame,
		// and we only iterate over _size_ entities,

		auto& mgr = tile.GetManager();
		/*
		mgr.forEntitiesMatching<ecs::STransform>(
			[&](auto index, auto& cTransform)
		{
			// Transform the Object.
			worldMatrix = XMMatrixMultiply(
				XMMatrixMultiply(
					XMMatrixScaling(cTransform.scale.x, cTransform.scale.y, cTransform.scale.z),
					XMMatrixRotationRollPitchYaw(cTransform.rotation.x, cTransform.rotation.y, cTransform.rotation.z)),
				XMMatrixTranslation(cTransform.position.x, cTransform.position.y, cTransform.position.z)
			);
		});*/

		// First parameter is the entity index
		mgr.forEntitiesMatching<ecs::SRenderColor>(
			[&](auto index, auto& cModel, auto& cTransform)
		{
			// Transform the Object.
			worldMatrix = XMMatrixMultiply(
				XMMatrixMultiply(
					XMMatrixScaling(cTransform.scale.x, cTransform.scale.y, cTransform.scale.z),
					XMMatrixRotationRollPitchYaw(cTransform.rotation.x, cTransform.rotation.y, cTransform.rotation.z)),
				XMMatrixTranslation(cTransform.position.x, cTransform.position.y, cTransform.position.z)
			);

			RenderModel<vertices::ColVertex>(m_direct3d->GetDeviceContext(), model[cModel.index]);
			result = m_colorShader->Render(m_direct3d->GetDeviceContext(),
				worldMatrix, viewMatrix, projectionMatrix, model[cModel.index].indexCount);
		});

		tile.Refresh();

	}


	return result;
}


template <class T>
void Renderer::RenderModel(ID3D11DeviceContext* device_context, vertices::Model &model)
{
	unsigned int stride;
	unsigned int offset;


	stride = sizeof(T);
	offset = 0;

	// Pass the vertex buffer to the input assembler
	device_context->IASetVertexBuffers(0, 1, model.vertexBuffer.GetAddressOf(), &stride, &offset);

	// Pass the inbdex buffer to the input assembler
	device_context->IASetIndexBuffer(model.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the rendering topology (triangle list)
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


Direct3D& Renderer::GetD3D()
{
	return *m_direct3d;
}

}
}
