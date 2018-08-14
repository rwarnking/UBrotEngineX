#pragma once

#include "gamelogic.h"

class Chess : public ubrot::logic::GameLogic
{
public:

	~Chess() {}

	void HandleComponent(
		ecs::Manager<AllSettings> &mgr,
		ecs::EntityIndex &entity,
		const std::string &name,
		rapidjson::GenericValue<rapidjson::UTF8<char>> &comp
	) override;
	void HandleTag(
		ecs::Manager<AllSettings> &mgr,
		ecs::EntityIndex &entity,
		const std::string &tag
	) override;


	void RegisterEntities(ubrot::Tile &tile, ubrot::io::AssetFiles &assetBits) override;
	void Process(ubrot::Scene &scene) override;

	static constexpr std::size_t MAX_ENTITIES = 32;

private:

	// Own stuff and methods
	using EntityHandle = ecs::Manager<ecs::MySettings>::Handle;

	enum class Figure : short {
		Bishop,
		King,
		Queen,
		Knight,
		Pawn,
		Rook
	};

	std::size_t FieldIndex(char one, std::size_t two);

	// TODO: template parameter
	EntityHandle m_lastIntersect;
};
