#pragma once

#include "../../header/graphics/vertextypes.h"

#include <d3d11.h>
#include <string>


namespace ubrot
{
namespace io
{

/**
* Creates and initializes the vertex and index buffers.
* @param device
*/
template <class T>
bool InitializeBuffers(ID3D11Device* device, std::string filename, graphics::vertices::Model &model);

};
};