///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: renderer.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../../header/graphics/renderer.h"
#include "../../header/io/assetmanager.h"

using namespace DirectX;

namespace ubrot
{
namespace graphics
{

Renderer::Renderer()
{
	m_direct3d = nullptr;
	m_colorShader = nullptr;
	m_oneColorShader = nullptr;
	m_textureShader = nullptr;
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


	// TODO array das dynamisch nach bedarf geladen wird
	m_oneColorShader = std::make_unique<ColorShader>();
	if (!m_oneColorShader)
	{
		return false;
	}

	m_oneColorShader->InitializeShader(m_direct3d->GetDevice(), hwnd);


	// TODO array das dynamisch nach bedarf geladen wird
	m_textureShader = std::make_unique<TextureShader>();
	if (!m_textureShader)
	{
		return false;
	}

	m_textureShader->InitializeShader(m_direct3d->GetDevice(), hwnd);

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
	auto orthoMatrix = m_direct3d->GetOrthoMatrix();

	//auto& model = scene.GetModels();

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
		static float rot = 0.0f; // TODO rausnehmen
		//rot += 0.0001f;

		auto color = DirectX::XMFLOAT4(
			(float)g_mouseX / (float)SCREEN_WIDTH / 2.0f +
			(float)g_mouseY / (float)SCREEN_HEIGHT / 2.0f, 0, 0, 0);
		mgr.forEntitiesMatching<ecs::SRender>(
			[&](auto index, auto& cModel, auto& cTransform, auto& cColor)
		{
			// Transform the Object.
			worldMatrix = XMMatrixMultiply(
				XMMatrixMultiply(
					XMMatrixScaling(cTransform.scale.x, cTransform.scale.y, cTransform.scale.z),
					XMMatrixRotationRollPitchYaw(cTransform.rotation.x, cTransform.rotation.y + rot, cTransform.rotation.z)),
				XMMatrixTranslation(cTransform.position.x, cTransform.position.y, cTransform.position.z)
			);

			auto& model = assets::GetModel(cModel.index);
			RenderModel<vertices::SimVertex>(m_direct3d->GetDeviceContext(), model);
			result = m_oneColorShader->Render(
				m_direct3d->GetDeviceContext(),
				worldMatrix, viewMatrix, projectionMatrix,
				cColor.color,
				model.indexCount
			);
		});

		// First parameter is the entity index
		mgr.forEntitiesMatching<ecs::SRenderColor>(
			[&](auto index, auto& cModel, auto& cTransform)
		{
			// Transform the Object.
			worldMatrix = XMMatrixMultiply(
				XMMatrixMultiply(
					XMMatrixScaling(cTransform.scale.x, cTransform.scale.y, cTransform.scale.z),
					XMMatrixRotationRollPitchYaw(cTransform.rotation.x, cTransform.rotation.y + rot, cTransform.rotation.z)),
				XMMatrixTranslation(cTransform.position.x, cTransform.position.y, cTransform.position.z)
			);

			auto& model = assets::GetModel(cModel.index);
			RenderModel<vertices::ColVertex>(m_direct3d->GetDeviceContext(), model);
			result = m_colorShader->Render(
				m_direct3d->GetDeviceContext(),
				worldMatrix, viewMatrix, projectionMatrix,
				model.indexCount
			);
		});

		mgr.forEntitiesMatching<ecs::SRenderTex>(
			[&](auto index, auto& cModel, auto& cTransform, auto& cTexture)
		{
			// Transform the Object.
			worldMatrix = XMMatrixMultiply(
				XMMatrixMultiply(
					XMMatrixScaling(cTransform.scale.x, cTransform.scale.y, cTransform.scale.z),
					XMMatrixRotationRollPitchYaw(cTransform.rotation.x, cTransform.rotation.y + rot, cTransform.rotation.z)),
				XMMatrixTranslation(cTransform.position.x, cTransform.position.y, cTransform.position.z)
			);

			auto texture = assets::GetTexture(0);
			auto& model = assets::GetModel(cModel.index);
			RenderModel<vertices::TexVertex>(m_direct3d->GetDeviceContext(), model);
			result = m_textureShader->Render(
				m_direct3d->GetDeviceContext(),
				worldMatrix, viewMatrix, projectionMatrix,
				model.indexCount,
				texture
			);
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
