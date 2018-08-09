#pragma once

//////////////
// INCLUDES //
//////////////
#include <vector>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "ecs/manager.h"

namespace ubrot
{
namespace loader
{

std::vector<std::string> LoadFilenames(std::string tilename);

bool LoadEntities();

};
};