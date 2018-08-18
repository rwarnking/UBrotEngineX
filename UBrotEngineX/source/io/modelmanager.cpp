#include "../../header/io/modelmanager.h"
#include "../../header/io/assetloader.h"

#include <vector>
#include <array>

namespace ubrot
{
namespace models
{

namespace gv = graphics::vertices;

// TODO: 2 vectors or map?
std::vector<gv::Model> models;
std::vector<int> indices;
// Procedural models already loaded
std::array<int, (std::size_t)Procedural::NUMBER> prods;


void Initialize()
{
	prods.fill(-1);
}


gv::Model& GetModel(std::size_t modelIndex, bool procedural)
{
	assert(modelIndex >= 0 && modelIndex <= indices.size());

	if (procedural)
	{
		return models[modelIndex];
	}
	else
	{
		return models[indices[modelIndex]];
	}
}


gv::Model& GetProdModel(Procedural pModel)
{
	if (prods[(std::size_t)pModel] >= 0)
	{
		return models[prods[(std::size_t)pModel]];
	}
	else
	{
		auto model = gv::Model();
		// TODO: other vertex types?
		io::LoadModel<gv::SimVertex>("", model, pModel);
		auto index = AddProdModel(pModel, model);

		return models[index];
	}
}


bool HasModel(std::size_t modelIndex)
{
	return indices[modelIndex] >= 0;
}


bool HasProdModel(Procedural pModel)
{
	return prods[(std::size_t)pModel] >= 0;
}


std::size_t GetNextIndex()
{
	return indices.size();
}


std::size_t GetProdIndex(Procedural pModel)
{
	return prods[(std::size_t)pModel];
}


void AddModel(std::size_t modelIndex, gv::Model model)
{
	if (modelIndex >= indices.size())
	{
		indices.resize(modelIndex+5, -1);
	}
	indices[modelIndex] = models.size();
	models.push_back(std::move(model));
}


int AddProdModel(Procedural pModel, gv::Model model)
{
	if (prods[(std::size_t)pModel] < 0)
	{
		prods[(std::size_t)pModel] = models.size();
		models.push_back(std::move(model));
		return (int)models.size() - 1;
	}
	return -1;
}

};
};