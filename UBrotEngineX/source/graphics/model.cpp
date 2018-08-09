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
	m_position = XMFLOAT3(10.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_radius = XMFLOAT3(1.0f, 1.0f, 1.0);

	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_vertexCount = 0;
	m_indexCount = 0;
}


Model::Model(const Model& other)
{
	m_position = other.m_position;
	m_rotation = other.m_rotation;
	m_scale = other.m_scale;
	m_radius = other.m_radius;

	m_vertexBuffer = other.m_vertexBuffer;
	m_indexBuffer = other.m_indexBuffer;
	m_vertexCount = other.m_vertexCount;
	m_indexCount = other.m_indexCount;
}


Model::~Model()
{
}


bool Model::InitializeBuffers(ID3D11Device* device)
{
	using namespace std;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	const int vertexCount = 3;
	const int indexCount = 3;

	m_vertexCount = vertexCount;
	m_indexCount = indexCount;

	// Allocate temporary arrays for vertex and index data
	//ColVertex* vertices = nullptr;
	auto vertices = new ColVertex[vertexCount];
	auto indices = new unsigned long[indexCount];

	// Fill the vertex array (triangle)
	// Bottom left
	vertices[0] = ColVertex(
		XMFLOAT3(-1.0f, -1.0f, 0.0f),
		XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
	);

	// Top middle
	vertices[1] = ColVertex(
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
	);

	// Bottom right
	vertices[2] = ColVertex(
		XMFLOAT3(1.0f, -1.0f, 0.0f),
		XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
	);

	// Fill the index array
	indices[0] = 0;  // Bottom left
	indices[1] = 1;  // Top middle
	indices[2] = 2;  // Bottom right


	// Initialize a static vertex buffer description
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ColVertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Initialize the subresource structure and pass the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer and store it in m_vertexBuffer

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Initialize a static index buffer description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Initialize the subresource structure and pass the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer and store it in m_indexBuffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Free allocated memory of the temporary data arrays, since they are no longer needed
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}


void Model::Render(ID3D11DeviceContext* device_context)
{
	unsigned int stride;
	unsigned int offset;


	stride = sizeof(ColVertex);
	offset = 0;

	// Pass the vertex buffer to the input assembler
	device_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Pass the inbdex buffer to the input assembler
	device_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the rendering topology (triangle list)
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


int Model::GetIndexCount()
{
	return m_indexCount;
}


XMFLOAT3 Model::GetRotation()
{
	return m_rotation;
}


XMFLOAT3 Model::GetPosition()
{
	return m_position;
}


XMFLOAT3 Model::GetScale()
{
	return m_scale;
}


XMFLOAT3 Model::GetRadius()
{
	return m_radius;
}

}
}
