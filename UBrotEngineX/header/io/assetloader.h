///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: assetloader.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <string>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../../header/graphics/vertextypes.h"
#include "modelmanager.h"

namespace ubrot
{
namespace io
{
///////////////////////////////////////////////////////////////////////////////////////////////////
/// TODO
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace gv = graphics::vertices;


void SetDevice(ID3D11Device* device);


template <class T>
bool LoadModel(
	std::string filename,
	gv::Model &model,
	models::Procedural pModel = models::Procedural::NUMBER
);

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
	gv::Model &model,
	T *&vertices,
	unsigned long *&indices
);

};
};