///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: tile.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "..\header\tile.h"

namespace ubrot
{

Tile::Tile()
{
	//m_model = std::vector<graphics::Model>();
}


bool Tile::Initialize(ID3D11Device* device, io::AssetFiles &bits, logic::GameLogic *logic)
{
	// Load all currently visible tiles and all currently visible entity models of those tiles
	/*const auto size = 40;
	m_model.reserve(size);
	for (auto i = 0; i < size; i++)
	{
		m_model.emplace_back();
		m_model.back().InitializeBuffers(device);
	}*/

	/*
	std::vector<std::vector<unsigned long>> tmp = std::vector<std::vector<unsigned long>>();
	for (auto i = 0; i < size; i++)
	{
		std::vector<unsigned long> x = std::vector<unsigned long>();
		x.push_back(0);
		x.push_back(1);
		x.push_back(2);
		tmp.push_back(x);
	}

	std::vector<unsigned long> tmp2 = std::vector<unsigned long>();
	for (auto i = 0; i < size; i++)
	{
		tmp2.push_back(0);
		tmp2.push_back(1);
		tmp2.push_back(2);
	}*/

	return io::LoadEntities(logic, m_mgr, bits);
}


void Tile::Refresh()
{
	m_mgr.refresh();
}


ecs::Manager<AllSettings>& Tile::GetManager()
{
	return m_mgr;
}



}