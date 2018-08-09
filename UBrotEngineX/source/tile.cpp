///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: tile.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "..\header\tile.h"

namespace ubrot
{

Tile::Tile()
{
	m_model = std::vector<graphics::Model>();
}


//Tile::Tile(const Tile &other)
//{
//	m_model = std::vector<std::unique_ptr<graphics::Model>>(other.m_model);
//	//m_model = std::make_unique<graphics::Model>(*(other.m_model));
//}
//
//Tile::Tile(Tile && other): m_model(std::move(other.m_model)) {}


bool Tile::Initialize(ID3D11Device* device)
{
	// Load all currently visible tiles and all currently visible entity models of those tiles
	const auto size = 40;
	m_model.reserve(size);
	for (auto i = 0; i < size; i++)
	{
		m_model.emplace_back();
		m_model.back().InitializeBuffers(device);
	}

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

	for (auto i(0u); i < size; ++i)
	{
		auto e(m_mgr.createHandle());

		/*auto setRndVec3([](auto& mVec, auto mX)
		{
			mVec{ mX,mX,mX };
		});*/

		auto& pos(m_mgr.addComponent<ecs::CTransform>(e, ecs::CTransform()).position);
		pos = DirectX::XMFLOAT3(i-size/2.0f, 0.0f, 0.0f);

		auto& model(m_mgr.addComponent<ecs::CModel>(e).index);
		model = i;

		auto& color(m_mgr.addComponent<ecs::CColor>(e).color);	// TODO das brauch eigentlich nur
		color = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);			// ein Tag sein, weil die Color
																// ja im Buffer steht pro vertex
	}

	return true;
}

void Tile::Refresh()
{
	m_mgr.refresh();
}


std::vector<graphics::Model>& Tile::GetModels()
{
	return m_model;
}

ecs::Manager<ecs::MySettings>& Tile::GetManager()
{
	return m_mgr;
}



}