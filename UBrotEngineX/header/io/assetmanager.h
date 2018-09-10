#pragma once

#include "../graphics/vertextypes.h"

namespace ubrot
{
namespace assets
{

enum class Procedural
{
	Plane,
	Triangle,
	Cube,
	Sphere,
	NUMBER
};

void Initialize();

// Model stuff
graphics::vertices::Model& GetModel(int modelIndex);

graphics::vertices::Model& GetProdModel(Procedural pModel);

bool HasModel(int modelIndex);

bool HasProdModel(Procedural pModel);

std::size_t GetNextIndex();

std::size_t GetProdIndex(Procedural pModel);

void AddModel(int modelIndex, graphics::vertices::Model model);

int AddProdModel(Procedural pModel, graphics::vertices::Model model);

// Texture stuff
void AddTexture(int textureIndex, ID3D11ShaderResourceView *texture);

ID3D11ShaderResourceView* GetTexture(int textureIndex);

}
};