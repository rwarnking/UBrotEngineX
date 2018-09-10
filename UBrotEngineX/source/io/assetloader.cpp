#include "../../header/io/assetloader.h"

#include "DDSTextureLoader.h"
#include <wrl/client.h>

#include <fstream>
#include <vector>

namespace ubrot
{
namespace io
{

namespace dx = DirectX;


ID3D11Device *d3device;


void SetDevice(ID3D11Device * device)
{
	d3device = device;
}


ID3D11ShaderResourceView * LoadTexture(std::string filename)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

	// Be careful to specify a format when creating the dds file!
	auto result = DirectX::CreateDDSTextureFromFile(
		d3device,
		L"D:/07_Projekte/UBrotEngineX/UBrotEngineX/data/assets/textures/Pebbles.dds",
		nullptr,
		srv.GetAddressOf()
	);

	if (FAILED(result))
	{
		//throw std::runtime_error("");
	}

	return srv.Detach();
}


template <class T>
void GenerateCube(gv::Model &model, T* &vertices, unsigned long* &indices)
{
	// TODO:
}


template <class T>
void GenerateTriangle(gv::Model &model, T* &vertices, unsigned long* &indices)
{
	const int vertexCount = 3;
	const int indexCount = 3;

	model.vertexCount = vertexCount;
	model.indexCount = indexCount;

	// Allocate temporary arrays for vertex and index data
	//ColVertex* vertices = nullptr;
	vertices = new T[vertexCount];
	indices = new unsigned long[indexCount];

	// Fill the vertex array (triangle)
	// Bottom left
	gv::Create(vertices[0],
		dx::XMFLOAT3(-1.0f, -1.0f, 0.0f),
		dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		dx::XMFLOAT2(),
		dx::XMFLOAT3(),
		dx::XMFLOAT3(),
		dx::XMFLOAT3()
	);

	// Top middle
	gv::Create(vertices[1],
		dx::XMFLOAT3(0.0f, 1.0f, 0.0f),
		dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		dx::XMFLOAT2(),
		dx::XMFLOAT3(),
		dx::XMFLOAT3(),
		dx::XMFLOAT3()
	);

	// Bottom right
	gv::Create(vertices[2],
		dx::XMFLOAT3(1.0f, -1.0f, 0.0f),
		dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		dx::XMFLOAT2(),
		dx::XMFLOAT3(),
		dx::XMFLOAT3(),
		dx::XMFLOAT3()
	);

	// Fill the index array
	indices[0] = 0;  // Bottom left
	indices[1] = 1;  // Top middle
	indices[2] = 2;  // Bottom right
}


template <class T>
void GeneratePlane(gv::Model &model, T* &vertices, unsigned long* &indices)
{
	const int vertexCount = 4 * 2;
	const int indexCount = 6;

	model.vertexCount = vertexCount;
	model.indexCount = indexCount;

	// Allocate temporary arrays for vertex and index data
	//ColVertex* vertices = nullptr;
	vertices = new T[vertexCount];
	indices = new unsigned long[indexCount];

	// Fill the vertex array (triangle)
	// Bottom left
	gv::Create(vertices[0],
		dx::XMFLOAT3(-0.5f, -0.5f, 0.f),
		dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		dx::XMFLOAT2(), dx::XMFLOAT3(),
		dx::XMFLOAT3(), dx::XMFLOAT3()
	);

	// Top left
	gv::Create(vertices[1],
		dx::XMFLOAT3(-0.5f, 0.5f, 0.f),
		dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		dx::XMFLOAT2(), dx::XMFLOAT3(),
		dx::XMFLOAT3(), dx::XMFLOAT3()
	);

	// Bottom right
	gv::Create(vertices[2],
		dx::XMFLOAT3(0.5f, -0.5f, 0.f),
		dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		dx::XMFLOAT2(), dx::XMFLOAT3(),
		dx::XMFLOAT3(), dx::XMFLOAT3()
	);

	// Top right
	gv::Create(vertices[3],
		dx::XMFLOAT3(0.5f, 0.5f, 0.f),
		dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		dx::XMFLOAT2(), dx::XMFLOAT3(),
		dx::XMFLOAT3(), dx::XMFLOAT3()
	);

	// Fill the index array
	indices[0] = 0;  // Bottom left
	indices[1] = 1;  // Top left
	indices[2] = 2;  // Bottom right
	indices[3] = 3;  // Bottom right
	indices[4] = 2;  // Top left
	indices[5] = 1;  // Top right

}


template <class T>
bool LoadModel(std::string filename, gv::Model &model, assets::Procedural pModel)
{
	// Vertices array
	T *vertices;
	// Indices array
	unsigned long *indices;

	if (pModel == assets::Procedural::NUMBER) // TODO
	{
		if (!LoadModelFromOBJ<T>(filename, model, vertices, indices))
		{
			return false; // TODO
		}
	}
	else
	{
		switch (pModel)
		{
			case assets::Procedural::Cube: GenerateCube<T>(model, vertices, indices); break;
			case assets::Procedural::Plane: GeneratePlane<T>(model, vertices, indices); break;
			default: GenerateTriangle<T>(model, vertices, indices); break;
		}
	}
	return InitializeBuffers(model, vertices, indices);
}


template <class T>
bool LoadModelFromOBJ(
	std::string filename,
	gv::Model &model,
	T *&vertices,
	unsigned long *&indices
)
{
	int vertexCount, textureCount, normalCount, faceCount;

	// Read in the number of vertices, tex coords, normals, and faces so that the data
	// can be initialized with the exact sizes needed.
	if (!ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount))
	{
		return false;
	}

	auto result = LoadData<T>(
		filename,vertexCount, textureCount, normalCount, faceCount, vertices, indices
	);
	if (!result)
	{
		return false;
	}
	model.vertexCount = faceCount * 3;
	model.indexCount = faceCount * 3;

	return true;
}


bool ReadFileCounts(
	std::string filename,
	int &vertexCount,
	int &textureCount,
	int &normalCount,
	int &faceCount
)
{
	std::ifstream fin;
	char input;

	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail())
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex,
		// the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') {
				vertexCount++;
			}
			if (input == 't') {
				textureCount++;
			}
			if (input == 'n') {
				normalCount++;
			}
		}
		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') {
				faceCount++;
			}
		}
		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}
		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();

	return true;
}


template <class T>
bool LoadData(
	std::string filename,
	int vertexCount,
	int textureCount,
	int normalCount,
	int faceCount,
	T* &vertices2,
	unsigned long* &indices
)
{
	gv::Vertex *vertices, *texcoords, *normals;
	gv::Face *faces;
	std::ifstream fin;

	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;

	// Initialize the four data structures.
	vertices = new gv::Vertex[vertexCount];
	if (!vertices)
	{
		return NULL;
	}

	texcoords = new gv::Vertex[textureCount];
	if (!texcoords)
	{
		return NULL;
	}

	normals = new gv::Vertex[normalCount];
	if (!normals)
	{
		return NULL;
	}

	faces = new gv::Face[faceCount];
	if (!faces)
	{
		return NULL;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false; // TODO
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since
	//			  Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x;
				fin >> vertices[vertexIndex].y;
				fin >> vertices[vertexIndex].z;
				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * - 1.f;
				vertexIndex++;
			}
			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;
				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.f - texcoords[texcoordIndex].y;
				texcoords[texcoordIndex].z = 0.f;
				texcoordIndex++;
			}
			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x;
				fin >> normals[normalIndex].y;
				fin >> normals[normalIndex].z;
				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * - 1.f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it from right hand system
				// to a left hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2;
				fin >> faces[faceIndex].nIndex3 >> faces[faceIndex].vIndex2 >> input2;
				fin >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2;
				fin >> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1;
				fin >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	dx::XMFLOAT3 min = dx::XMFLOAT3(0.0f, 0.0f, 0.0f);
	dx::XMFLOAT3 max = dx::XMFLOAT3(0.0f, 0.0f, 0.0f);

	// Create the vertex array.
	vertices2 = new T[faceCount * 3];

	// Create the index array.
	indices = new unsigned long[faceCount * 3];

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0, j = 0; i < faceIndex; i++, j += 3)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		gv::Create(vertices2[j],
			dx::XMFLOAT3(vertices[vIndex].x, vertices[vIndex].y, vertices[vIndex].z),
			dx::XMFLOAT4(0.f, 0.f, (float)i / faceIndex, 1.f),
			dx::XMFLOAT2(texcoords[tIndex].x, texcoords[tIndex].y),
			dx::XMFLOAT3(normals[nIndex].x, normals[nIndex].y, normals[nIndex].z),
			dx::XMFLOAT3(),
			dx::XMFLOAT3()
		);

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		gv::Create(vertices2[j + 1],
			dx::XMFLOAT3(vertices[vIndex].x, vertices[vIndex].y, vertices[vIndex].z),
			dx::XMFLOAT4(0.f, 0.f, (float)i / faceIndex, 1.f),
			dx::XMFLOAT2(texcoords[tIndex].x, texcoords[tIndex].y),
			dx::XMFLOAT3(normals[nIndex].x, normals[nIndex].y, normals[nIndex].z),
			dx::XMFLOAT3(),
			dx::XMFLOAT3()
		);

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		gv::Create(vertices2[j + 2],
			dx::XMFLOAT3(vertices[vIndex].x, vertices[vIndex].y, vertices[vIndex].z),
			dx::XMFLOAT4(0.f, 0.f, (float)i / faceIndex, 1.f),
			dx::XMFLOAT2(texcoords[tIndex].x, texcoords[tIndex].y),
			dx::XMFLOAT3(normals[nIndex].x, normals[nIndex].y, normals[nIndex].z),
			dx::XMFLOAT3(),
			dx::XMFLOAT3()
		);

		indices[j] = j;
		indices[j + 1] = j + 1;
		indices[j + 2] = j + 2;

		for (int k = 0; k < 3; k++)
		{
			if (vertices2[j + k].position.x < min.x)
				min.x = vertices2[j + k].position.x;
			if (vertices2[j + k].position.x > max.x)
				max.x = vertices2[j + k].position.x;

			if (vertices2[j + k].position.y < min.y)
				min.y = vertices2[j + k].position.y;
			if (vertices2[j + k].position.y > max.y)
				max.y = vertices2[j + k].position.y;

			if (vertices2[j + k].position.z < min.z)
				min.z = vertices2[j + k].position.z;
			if (vertices2[j + k].position.z > max.z)
				max.z = vertices2[j + k].position.z;
		}
	}

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = nullptr;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = nullptr;
	}
	if (normals)
	{
		delete[] normals;
		normals = nullptr;
	}
	if (faces)
	{
		delete[] faces;
		faces = nullptr;
	}

	return true;
}


template <class T>
bool InitializeBuffers(
	gv::Model &model,
	T *&vertices,
	unsigned long *&indices
)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Initialize a static vertex buffer description
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(T) * model.vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Initialize the subresource structure and pass the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer and store it in m_vertexBuffer

	result = d3device->CreateBuffer(
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
	result = d3device->CreateBuffer(&indexBufferDesc, &indexData, model.indexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Free allocated memory of the temporary data arrays, since they are no longer needed
	delete[] vertices;
	delete[] indices;

	return true;
}



template bool
LoadModel<gv::SimVertex>(std::string fn, gv::Model &model, assets::Procedural pModel);
template bool
LoadModel<gv::ColVertex>(std::string fn, gv::Model &model, assets::Procedural pModel);
template bool
LoadModel<gv::TexVertex>(std::string fn, gv::Model &model, assets::Procedural pModel);
template bool
LoadModel<gv::LigVertex>(std::string fn, gv::Model &model, assets::Procedural pModel);
template bool
LoadModel<gv::NomVertex>(std::string fn, gv::Model &model, assets::Procedural pModel);
template bool
LoadModel<gv::TesVertex>(std::string fn, gv::Model &model, assets::Procedural pModel);

};
};