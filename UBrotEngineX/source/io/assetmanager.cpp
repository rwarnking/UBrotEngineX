#include "../../header/io/assetmanager.h"
#include "../../header/io/assetloader.h"

#include <vector>
#include <array>


namespace ubrot
{
namespace assets
{

namespace gv = graphics::vertices;

// TODO: 2 vectors or map?
std::vector<gv::Model> models;
std::vector<int> indices;
// Procedural model indices
std::array<int, (int)Procedural::NUMBER> prods;

// Textures
std::vector<ID3D11ShaderResourceView*> textures;

void Initialize()
{
	prods.fill(-1);
}


gv::Model& GetModel(int modelIndex)
{
	if (modelIndex < 0)
	{
		return models[-(modelIndex+1)];
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


bool HasModel(int modelIndex)
{
	return indices[modelIndex] >= 0;
}


bool HasProdModel(Procedural pModel)
{
	return prods[(int)pModel] >= 0;
}


std::size_t GetNextIndex()
{
	return models.size();
}


std::size_t GetProdIndex(Procedural pModel)
{
	return prods[(int)pModel];
}


void AddModel(int modelIndex, gv::Model model)
{
	if (modelIndex >= indices.size())
	{
		indices.resize(modelIndex+5, -1);
	}
	indices[modelIndex] = (int)models.size();
	models.push_back(std::move(model));
}


int AddProdModel(Procedural pModel, gv::Model model)
{
	if (prods[(int)pModel] < 0)
	{
		prods[(int)pModel] = (int)models.size();
		models.push_back(std::move(model));
		return (int)models.size() - 1;
	}
	return -1;
}


void AddTexture(int textureIndex, ID3D11ShaderResourceView *texture)
{
	if (textures.empty())
	{
		textures.push_back(texture);
	}
	else
	{
		textures[0] = texture;
	}
}


ID3D11ShaderResourceView * GetTexture(int textureIndex)
{
	return textures[0];
}

};
};