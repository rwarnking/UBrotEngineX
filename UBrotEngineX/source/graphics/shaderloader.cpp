///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: shaderloader.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../../header/graphics/shaderloader.h"

using namespace DirectX;

ShaderLoader::ShaderLoader()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_matrixBuffer = nullptr;
}


ShaderLoader::ShaderLoader(const ShaderLoader& other)
{
}


ShaderLoader::~ShaderLoader()
{
}


bool ShaderLoader::InitializeShader(ID3D11Device *device, HWND hwnd)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc; // TODO: not needed for colored


	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// Try to compile the vertex shader code given by the file and print an error if it fails
	result = D3DCompileFromFile(
		L"../UBrotEngineX/shader/color.vs", NULL, NULL, "ColorVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, L"../UBrotEngineX/shader/color.vs");
		}
		else
		{
			MessageBox(hwnd, L"../UBrotEngineX/shader/color.vs", L"Missing Shader File", MB_OK);
		}

		return false;
	}


	// Try to compile the fragment shader code given by the dile and print an error if it fails
	result = D3DCompileFromFile(
		L"../UBrotEngineX/shader/color.fs", NULL, NULL, "ColorFragmentShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, L"../UBrotEngineX/shader/color.fs");
		}
		else
		{
			MessageBox(hwnd, L"../UBrotEngineX/shader/color.fs", L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex- and fragmentshader from the buffers and save them in
	// shader object variables.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Prepare the layout to describe the structure of a vertex. Every attribute translates to
	// a slot whose format and other characteristics have to be set manually.
	// Uses D3D11_APPEND_ALIGNED_ELEMENT to ensure that data blocks are aligned sensibly.
	// The structure here must conform to that in the shader file and the vertex struct.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the input layout and store it in m_layout, using the previously filled description
	result = device->CreateInputLayout(
		polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Free the shader and index buffer's memory
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;


	// Prepare the constand buffers (uniforms) for use on the shader. Since the content of the
	// buffer can change each frame, it has to be dynamic (D3D11_USAGE_DYNAMIC) and the CPU must
	// be allowed to write to it (D3D11_CPU_ACCESS_WRITE).
	D3D11_BUFFER_DESC matrixBufferDesc;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, m_matrixBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


bool XM_CALLCONV ShaderLoader::Render(
	ID3D11DeviceContext* deviceContext,
	FXMMATRIX worldMatrix, CXMMATRIX viewMatrix, CXMMATRIX projectionMatrix,
	int indexCount
)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	XMMATRIX worldMatrixTransposed, viewMatrixTransposed, projectionMatrixTransposed;
	unsigned int bufferNumber;


	MatrixBufferType* data_ptr1;

	// Matrices must be transposed (because DirectX 11)
	// TODO: why ? more explanation ?
	worldMatrixTransposed = XMMatrixTranspose(worldMatrix);
	viewMatrixTransposed = XMMatrixTranspose(viewMatrix);
	projectionMatrixTransposed = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(
		m_matrixBuffer.Get(),
		0, D3D11_MAP_WRITE_DISCARD,
		0, &mappedResource
	);
	if (FAILED(result))
	{
		return false;
	}

	data_ptr1 = (MatrixBufferType*)mappedResource.pData;
	data_ptr1->world = worldMatrixTransposed;
	data_ptr1->view = viewMatrixTransposed;
	data_ptr1->projection = projectionMatrixTransposed;

	deviceContext->Unmap(m_matrixBuffer.Get(), 0);

	// Choose the buffer number to be used in the shader and set the buffer
	bufferNumber = 0;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());


	// Call render to draw
	RenderShader(deviceContext, indexCount);

	return true;
}


void ShaderLoader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(m_layout.Get());

	deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}


void ShaderLoader::OutputShaderErrorMessage(
	ID3D10Blob* errorMessage,
	HWND hwnd,
	const WCHAR* shaderFilename
)
{
	char* compileErrors;
	size_t bufferSize, i;
	std::ofstream fout;


	// Prepare error messages
	compileErrors = (char*)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();

	// Open the file to write to
	fout.open("shader-error.txt");
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}
	fout.close();

	errorMessage->Release();
	errorMessage = nullptr;

	// Open a message box to signal to the user that an error has occurred and can be
	// inspected in the previously written file.
	MessageBox(
		hwnd,
		L"Error compiling shader.  Check shader-error.txt for message.",
		shaderFilename,
		MB_OK
	);

	return;
}
