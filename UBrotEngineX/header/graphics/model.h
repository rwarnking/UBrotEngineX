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
#include "vertextypes.h"

namespace ubrot
{
namespace graphics
{
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
	 * @return m_indexCount
	 */
	int GetIndexCount();

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	int m_vertexCount, m_indexCount;
};
};
};

#endif