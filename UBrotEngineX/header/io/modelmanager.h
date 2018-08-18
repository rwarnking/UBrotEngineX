#pragma once

#include "../graphics/vertextypes.h"

namespace ubrot
{
namespace models
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

graphics::vertices::Model& GetModel(std::size_t modelIndex, bool procedural = false);

graphics::vertices::Model& GetProdModel(Procedural pModel);

bool HasModel(std::size_t modelIndex);

bool HasProdModel(Procedural pModel);

std::size_t GetNextIndex();

std::size_t GetProdIndex(Procedural pModel);

void AddModel(std::size_t modelIndex, graphics::vertices::Model model);

int AddProdModel(Procedural pModel, graphics::vertices::Model model);

}
};