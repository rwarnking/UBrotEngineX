#include "../../header/io/assetloader.h"

namespace ubrot
{
namespace io
{

//using namespace std;
namespace gv = graphics::vertices;
namespace dx = DirectX;

template <class T>
bool InitializeBuffers(ID3D11Device* device, std::string filename, gv::Model &model)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	const int vertexCount = 3;
	const int indexCount = 3;

	model.vertexCount = vertexCount;
	model.indexCount = indexCount;

	// Allocate temporary arrays for vertex and index data
	//ColVertex* vertices = nullptr;
	auto vertices = new T[vertexCount];
	auto indices = new unsigned long[indexCount];

	// Fill the vertex array (triangle)
	// Bottom left
	vertices[0] = gv::ColVertex(
		dx::XMFLOAT3(-1.0f, -1.0f, 0.0f),
		dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
	);

	// Top middle
	vertices[1] = gv::ColVertex(
		dx::XMFLOAT3(0.0f, 1.0f, 0.0f),
		dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
	);

	// Bottom right
	vertices[2] = gv::ColVertex(
		dx::XMFLOAT3(1.0f, -1.0f, 0.0f),
		dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
	);

	// Fill the index array
	indices[0] = 0;  // Bottom left
	indices[1] = 1;  // Top middle
	indices[2] = 2;  // Bottom right


					 // Initialize a static vertex buffer description
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(gv::ColVertex) * model.vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Initialize the subresource structure and pass the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer and store it in m_vertexBuffer

	result = device->CreateBuffer(
		&vertexBufferDesc,
		&vertexData,
		model.vertexBuffer.GetAddressOf()
	);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize a static index buffer description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * model.indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Initialize the subresource structure and pass the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer and store it in m_indexBuffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, model.indexBuffer.GetAddressOf());
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

template bool InitializeBuffers<gv::ColVertex>(ID3D11Device* device, std::string filename, gv::Model &model);

};
};