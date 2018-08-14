#include "../../header/io/loader.h"
#include "../../header/io/assetloader.h"

#include "../../header/logic/gameobjects.h"

#include <algorithm>
#include <fstream>
#include <limits>

#include <sstream>
#include <string>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

#define DIR_ROOT "data/"
// Asset paths
#define DIR_ASSETS "assets/"
#define DIR_MODELS "models/"
#define DIR_AUDIO "audio/"
#define DIR_TEX "textures/"
// Scene paths
#define DIR_SCENES "scenes/"
#define DIR_TILES "tiles/"
// Scene file names
#define FILE_META "scene.txt"
#define FILE_MODELS "models.txt"
#define FILE_TEX "textures.txt"
#define FILE_AUDIO "audio.txt"

namespace ubrot
{
namespace io
{

namespace rj = rapidjson;
namespace gv = graphics::vertices;

inline std::string GetPathScenes(int sceneID, const char* last)
{
	std::stringstream ss;
	ss << DIR_ROOT << DIR_SCENES << std::to_string(sceneID) << '/' << last;
	return ss.str();
}

inline std::string GetFileTile(int sceneID, int tileID)
{
	std::stringstream ss;
	ss << DIR_ROOT << DIR_SCENES << std::to_string(sceneID);
	ss << '/' << DIR_TILES << std::to_string(tileID) << ".json";
	return ss.str();
}

inline std::string GetPathAssets(const char* last)
{
	std::stringstream ss;
	ss << DIR_ROOT << DIR_ASSETS << last;
	return ss.str();
}


inline std::string GetFileAssets(const char* dir, const char* file)
{
	std::stringstream ss;
	ss << DIR_ROOT << DIR_ASSETS << dir << file;
	return ss.str();
}


template <class T>
void ParseComponents(
	logic::GameLogic *logic,
	ecs::Manager<AllSettings> &mgr,
	ecs::EntityIndex &entity,
	const rj::GenericObject<false, T> &comps,
	AssetFiles &bits
)
{
	for (auto& c : comps)
	{
		if (c.name == "transform")
		{
			auto& trans(mgr.addComponent<ecs::CTransform>(entity, ecs::CTransform()));
			auto tmp = c.value["pos"].GetArray();
			trans.position = DirectX::XMFLOAT3(
				tmp[0].GetFloat(),
				tmp[1].GetFloat(),
				tmp[2].GetFloat()
			);
			tmp = c.value["rot"].GetArray();
			trans.rotation = DirectX::XMFLOAT3(
				tmp[0].GetFloat(),
				tmp[1].GetFloat(),
				tmp[2].GetFloat()
			);
			tmp = c.value["scale"].GetArray();
			trans.scale = DirectX::XMFLOAT3(
				tmp[0].GetFloat(),
				tmp[1].GetFloat(),
				tmp[2].GetFloat()
			);
		}
		if (c.name == "model")
		{
			auto& model(mgr.addComponent<ecs::CModel>(entity).index);
			model = c.value.GetInt();

			// TODO: make this look prettier?
			//		 make logger evaluate a condition, and if that does not hold then
			//		 shut down the program properly and print an error message (somewhere)
			assert(model < bits.modelFiles.size() && "Model count in scene meta is too small");

			bits.modelFiles[model] = true;
		}
		else
		{
			// TODO: let game logic handle other components
			logic->HandleComponent(mgr, entity, c.name.GetString(), c.value);
		}
	}
}

template <class T>
void ParseTags(
	logic::GameLogic *logic,
	ecs::Manager<AllSettings> &mgr,
	ecs::EntityIndex &entity,
	const rj::GenericArray<false, T> &tags
)
{
	for (auto& t : tags)
	{
		if (t == "color")
		{
			mgr.addTag<ecs::TColor>(entity);
		}
		else
		{
			// TODO: let game logic handle other components
			logic->HandleTag(mgr, entity, t.GetString());
		}
	}
}

SceneMeta LoadSceneMeta(int sceneID)
{
	auto meta = SceneMeta();
	std::ifstream fin(GetPathScenes(sceneID, FILE_META));

	if (!(fin >> meta.tiles >> meta.audio >> meta.models >> meta.textures))
	{
		// TODO
	}

	return meta;
}

bool LoadEntities(logic::GameLogic *logic, ecs::Manager<AllSettings> &mgr, AssetFiles &bits)
{
	FILE *fp;
	fopen_s(&fp, GetFileTile(0, 0).c_str(), "rb");

	char readBuffer[65536];
	rj::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	rj::Document d;
	d.ParseStream(is);

	const auto entities = d["entities"].GetArray();
	for (auto i(0u); i < entities.Size(); ++i)
	{
		auto e(mgr.createIndex());
		const auto comps = entities[i]["components"].GetObjectW();
		ParseComponents(logic, mgr, e, comps, bits);
		const auto tags = entities[i]["tags"].GetArray();
		ParseTags(logic, mgr, e, tags);
	}

	return true;
}


std::vector<gv::Model> LoadModels(BitVec &modelBits, ID3D11Device *device, int sceneID)
{
	auto models = std::vector<gv::Model>(modelBits.size());
	auto tmp = std::vector<gv::Model>();

	auto tuple = ecs::MPL::Impl::TypeTuple<gv::VertexList>();

	std::ifstream fin(GetPathScenes(sceneID, FILE_MODELS));
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		// TODO: logger error and shutdown
	}
	for (auto i(0u); i < modelBits.size(); i++)
	{
		char name[256];
		std::size_t vertexIndex;
		if (!(fin >> name >> vertexIndex))
		{
			// TODO: handle reading error
		}

		if (!modelBits[i])
		{
			continue;
		}

		auto load = [&](auto t) {
			ubrot::io::LoadModel<ECS_TYPE(t)>(device, GetFileAssets(DIR_MODELS, name), models[i]);
		};
		ecs::MPL::VisitAt(tuple, vertexIndex, load);

	}

	fin.close();

	return models;
}

};
};