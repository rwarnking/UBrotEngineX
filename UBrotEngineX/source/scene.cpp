///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: scene.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "..\header\scene.h"
#include "..\..\mpl\MPL\MPL.hpp"

#include <tuple>
#include <bitset>
#include <type_traits>
#include <algorithm> // TODO

namespace ubrot
{

Scene::Scene()
{
	m_camera = nullptr;
	m_tiles = std::vector<Tile>();
}


Scene::Scene(const Scene &other)
{
}


Scene::~Scene()
{
}


bool Scene::Initialize(ID3D11Device* device, logic::GameLogic *logic)
{
	// Create a camera and place it at the origin.
	m_camera = std::make_unique<graphics::Camera>();
	if (!m_camera)
	{
		return false;
	}

	m_camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_camera->SetRotation(0.0f, 0.0f, 0.0f);
	m_camera->SetFrameTime(0.2f); // TODO
	m_camera->RenderBaseViewMatrix();

	// TODO: see if initialization should happen in a constructor
	// TODO: reserve vector space according to number of tiles

	m_meta = io::LoadSceneMeta();

	// Load all currently visible tiles and all currently visible entity models of those tiles

	// 36917 ms
	auto assetBits = io::AssetFiles(m_meta);

	// Iterate over all tiles of the scene
	for (auto i = 0; i < m_meta.tiles; i++)
	{
		// Check if tile is visible

		// Add tile and load its asset requirements
		m_tiles.emplace_back();
		if (!m_tiles.back().Initialize(device, assetBits, logic))
			return false;
	}
	// Lade assets TODO
	logic->RegisterEntities(m_tiles[0], assetBits);

	io::LoadModels(assetBits.modelFiles);
	io::LoadTextures();

	return true;
}


graphics::Camera& Scene::GetCamera()
{
	return *m_camera;
}


std::vector<Tile>& Scene::GetTiles()
{
	return m_tiles;
}


std::vector<graphics::vertices::Model>& Scene::GetModels()
{
	return m_models;
}

}
