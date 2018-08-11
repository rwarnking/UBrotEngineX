#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

namespace ubrot
{
namespace graphics
{
namespace vertices
{

struct Model
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	int vertexCount, indexCount;
};

/**
* Container for colored vertices with a 3D position an a RGBA-color component
*/
struct ColVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;

	/**
	* Default constructor, where everyting is set to the default values of XMFLOAT
	*/
	ColVertex() :
		position(DirectX::XMFLOAT3()),
		color(DirectX::XMFLOAT4()) {}

	/**
	* Constructor that sets both position and color
	* @param pos
	* @param col
	*/
	ColVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 col) : position(pos), color(col) {}
};

};
};
};