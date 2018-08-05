///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: model.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL_H_
#define _MODEL_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <memory>
#include <wrl\client.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "direct3d.h"


/////////////
// GLOBALS //
/////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Model
/// The model class stores all relevant information related to an 3D entity, like the
/// mesh and possible transformation-information.
/// TODO: rename to entity (!NO!) and make data-oriented
///////////////////////////////////////////////////////////////////////////////////////////////////
class Model
{
public:
	/**
	 * Container for colored vertices with a 3D position an a RGBA-color component
	 */
	struct ColVertex
	{
		/**
		 * Default constructor, where everyting is set to the default values of XMFLOAT
		 */
		ColVertex() :
			position(DirectX::XMFLOAT3()), color(DirectX::XMFLOAT4())
		{
		}

		/**
		 * Constructor that sets both position and color
		 * @param pos
		 * @param col
		 */
		ColVertex(
			DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 col
		) : position(pos), color(col)
		{
		}

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

public:
	/**
	* A constructor where all the member pointers are initialised to null
	*/
	Model();
	/**
	* Copy contructor
	* @param other to copy
	*/
	Model(const Model& other);
	~Model();

	/**
	 * Creates and initializes the vertex and index buffers.
	 * @param device
	 */
	bool InitializeBuffers(ID3D11Device* device);

	/**
	 * Activates the vertex and index buffers for the input assembler of the GPU which enables
	 * this model to be rendered by shaders. This function also sets the topology used to
	 * render the model, which is a triangle list (\c IASetPrimitiveTopology).
	 * TODO: support different rendering methods and topologies
	 * @param deviceContext
	 */
	void Render(ID3D11DeviceContext* deviceContext);

	/**
	 * @return m_indexCount
	 */
	int GetIndexCount();

	/**
	 * \defgroup GetAttributes
	 * Getter for the different properties of a model
	 * @{
	 */
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT3 GetRadius(); // Used for BoundingBox test
	/** @} */

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_radius;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif