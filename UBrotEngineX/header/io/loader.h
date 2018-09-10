///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: loader.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LOADER_H_
#define _LOADER_H_


//////////////
// INCLUDES //
//////////////
#include <vector>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../ecs/manager.h"
#include "../../header/logic/gamelogic.h"
#include "../graphics/vertextypes.h"


namespace ubrot
{

// Forward declarations
namespace logic
{
class GameLogic;
};
namespace io
{
///////////////////////////////////////////////////////////////////////////////////////////////////
/// TODO
///////////////////////////////////////////////////////////////////////////////////////////////////

struct SceneMeta
{
	int tiles;
	int audio;
	int models;
	int textures;
};

using BitVec = std::vector<bool>;

struct AssetFiles
{
	BitVec audioFiles;
	BitVec modelFiles;
	BitVec textureFiles;

	AssetFiles() : AssetFiles(0) {}

	AssetFiles(const int size) :
		audioFiles(BitVec(size, false)),
		modelFiles(BitVec(size, false)),
		textureFiles(BitVec(size, false)) {}

	AssetFiles(const SceneMeta &meta) :
		audioFiles(BitVec(meta.audio, false)),
		modelFiles(BitVec(meta.models, false)),
		textureFiles(BitVec(meta.textures, false))
	{

	}

	//void Add(const AssetFiles &other) noexcept
	//{
	//	auto orFunc = [](auto& vec1, const auto&vec2) {
	//		assert(vec1.size() == vec2.size());
	//		for (auto i(0u); i < vec1.size(); i++)
	//		{
	//			vec1[i] = vec1[i] | vec2[i];
	//		}
	//	};
	//	orFunc(audioFiles, other.audioFiles);
	//	orFunc(modelFiles, other.modelFiles);
	//	orFunc(textureFiles, other.textureFiles);
	//}
};

SceneMeta LoadSceneMeta(int sceneID=0);

// läd entitys eines tiles aus File
bool LoadEntities(logic::GameLogic *logic, ecs::Manager<AllSettings> &mgr, AssetFiles &bits);

bool LoadTextures();

// TODO sceneID
bool LoadModels(BitVec &modelBits, int sceneID = 0);

// Load model (where index is known)
bool LoadModel(
	std::string filename,
	std::size_t vertexIndex,
	int modelIndex
);


};
};
#endif // !_LOADER_H_