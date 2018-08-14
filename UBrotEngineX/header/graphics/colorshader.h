///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: colorshader.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _COLORSHADER_H_
#define _COLORSHADER_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include <vector>
#include <wrl\client.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////

namespace ubrot
{
namespace graphics
{
///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: ColorShader
/// This class loads all needed shaders which can be used by calling the \c Render function.
/// The shader loading takes place in the \c InitializeShader functions and happens dynamically
/// at runtime, which is done by parsing the passed file and compiling it.
///////////////////////////////////////////////////////////////////////////////////////////////////
class ColorShader
{
public: // TODO: use a DOD-composition pattern here?
		/**
		* Holds matrices necessary for rendering, which are passed to the shader as a uniform.
		*/
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};


	struct ColorBufferType
	{
		DirectX::XMFLOAT4 color;
	};


public:
	/**
	* A constructor where all the member pointers are initialised to null.
	*/
	ColorShader();
	/**
	* Empty copy contructor
	* @param other to copy
	*/
	ColorShader(const ColorShader& other);
	~ColorShader();

	/**
	* Loads shader files and enables their use for DirectX and the GPU. Also initializes
	* necessary buffers and a description (\ m_layout), which must correspond to the structure
	* of both vertices and the shader.
	* @param device Direct3D device needed to create buffers
	* @param hwnd Window handle used for error messages
	*/
	bool InitializeShader(ID3D11Device *device, HWND hwnd);

	/**
	* Passes the uniform variables to the shader and subsequently calls \c RenderShader
	* @param deviceContext Direct3D device needed to set and map buffers as well as draw calls
	* @param indexCount
	* @param worldMatrix
	* @param viewMatrix
	* @param projectionMatrix
	*/
	bool XM_CALLCONV Render(
		ID3D11DeviceContext *deviceContext,
		DirectX::FXMMATRIX worldMatrix,
		DirectX::CXMMATRIX viewMatrix,
		DirectX::CXMMATRIX projectionMatrix,
		DirectX::XMFLOAT4 color,
		int indexCount
	);

private:
	/**
	* Sets the layout in the input assembler, so that the GPU knows the format of the
	* vertex buffers and then sets the vertex and fragment shader. Then the rendering
	* process is started using \c DrawIndexed
	* @param deviceContext
	* @param indexCount
	*/
	void RenderShader(ID3D11DeviceContext *deviceContext, int indexCount);
	/**
	* Writes the passed error message \p errorMessage to a file \p shaderFilename and
	* notifies the user with a message box.
	* @param errorMessage The error message to write
	* @param hwnd Window handle needed for the message box
	* @param shaderFilename The file to write to
	*/
	void OutputShaderErrorMessage(
		ID3D10Blob *errorMessage,
		HWND hwnd,
		const WCHAR *shaderFilename
	);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_colorBuffer;
};
};
};

#endif