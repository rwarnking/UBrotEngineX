///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: scene.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "..\header\scene.h"


Scene::Scene()
{
	m_model = nullptr;
	m_camera = nullptr;
}


Scene::Scene(const Scene &other)
{
}


Scene::~Scene()
{
}


bool Scene::Initialize(ID3D11Device* device)
{
	// Create a camera and place it at the origin.
	m_camera = std::make_unique<Camera>();
	if (!m_camera)
	{
		return false;
	}

	m_camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_camera->RenderBaseViewMatrix();

	// Load all currently visible tiles and all currently visible entity models of those tiles
	m_model = std::make_unique<Model>();
	m_model->InitializeBuffers(device);

	return true;
}


Camera& Scene::GetCamera()
{
	return *m_camera;
}


Model& Scene::GetModel()
{
	return *m_model;
}