///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: model.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../../header/graphics/model.h"

#include <vector>

using namespace DirectX;

namespace ubrot
{
namespace graphics
{

Model::Model()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_vertexCount = 0;
	m_indexCount = 0;
}


Model::Model(const Model& other)
{
	m_vertexBuffer = other.m_vertexBuffer;
	m_indexBuffer = other.m_indexBuffer;
	m_vertexCount = other.m_vertexCount;
	m_indexCount = other.m_indexCount;
}


Model::~Model()
{
}


int Model::GetIndexCount()
{
	return m_indexCount;
}

}
}
