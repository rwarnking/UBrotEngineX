#pragma once

#include "../../header/graphics/vertextypes.h"

#include <d3d11.h>
#include <string>


namespace ubrot
{
namespace io
{

namespace gv = graphics::vertices;

template <class T>
bool LoadModel(ID3D11Device* device, std::string filename, gv::Model &model);

template <class T>
bool LoadModelFromOBJ(
	std::string filename,
	gv::Model &model,
	T *&vertices,
	unsigned long *&indices
);

bool ReadFileCounts(
	std::string filename,
	int &vertexCount,
	int &textureCount,
	int &normalCount,
	int &faceCount
);

template <class T>
bool LoadData(
	std::string filename,
	int vertexCount,
	int textureCount,
	int normalCount,
	int faceCount,
	T* &vertices2,
	unsigned long* &indices,
	int tex_num
);

/**
* Creates and initializes the vertex and index buffers.
* @param device
*/
template <class T>
bool InitializeBuffers(
	ID3D11Device* device,
	gv::Model &model,
	T *&vertices,
	unsigned long *&indices
);

};
};