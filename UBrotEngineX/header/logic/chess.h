///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: chess.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <array>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "gamelogic.h"
#include "../graphics/camera.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Chess
/// TODO
///////////////////////////////////////////////////////////////////////////////////////////////////
class Chess : public ubrot::logic::GameLogic
{
public:

	Chess(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
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

	static constexpr std::size_t MAX_ENTITIES = 32 + 64;

private:

	// Own stuff and methods
	void RegisterModels();

	void CheckMovement(ubrot::graphics::Camera &camera);

	// Chess logic stuff
	bool IsValidMove(ecs::Manager<AllSettings> &mgr, int from, int to);
	bool IsValidSelection(ecs::Manager<AllSettings> &mgr, int selection);

	using EntityIndex = ecs::EntityIndex;

	enum class Figure : short {
		Bishop,
		King,
		Queen,
		Knight,
		Pawn,
		Rook
	};

	int m_lastIntersect;
};