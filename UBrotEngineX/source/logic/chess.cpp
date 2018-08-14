#include "../../header/logic/chess.h"

#include "../../header/scene.h"

void Chess::HandleComponent(
	ecs::Manager<AllSettings>& mgr,
	ecs::EntityIndex & entity,
	const std::string &name,
	rapidjson::GenericValue<rapidjson::UTF8<char>> &comp
)
{
}

void Chess::HandleTag(
	ecs::Manager<AllSettings>& mgr,
	ecs::EntityIndex & entity,
	const std::string &tag)
{
}

void Chess::RegisterEntities(ubrot::Tile &tile, ubrot::io::AssetFiles &assetBits)
{
	auto& mgr = tile.GetManager();

	auto boardWidth = 8;
	auto fieldSize = 1.f;
	auto startX = -((boardWidth / 2) * fieldSize - fieldSize / 2.f);
	auto startZ = startX;

	auto colorWhite = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	auto colorBlack = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	// Pawns
	for (auto i = 0; i < boardWidth*2; i++)
	{
		auto p(mgr.createIndex());

		// Components
		auto& trans(mgr.addComponent<ecs::CTransform>(p, ecs::CTransform()));
		trans.position = DirectX::XMFLOAT3(
			startX + (i % boardWidth) * fieldSize,
			0.f,
			i < boardWidth ? startZ+fieldSize : -startZ-fieldSize
		);
		trans.rotation = DirectX::XMFLOAT3();
		trans.scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);

		auto& pos(mgr.addComponent<CFieldPos>(p, CFieldPos()));
		pos.x = i < boardWidth ? i+1 : (i+1) % boardWidth;
		pos.y = i < boardWidth ? 1 : boardWidth;

		auto& model(mgr.addComponent<ecs::CModel>(p).index);
		model = (std::size_t)Figure::Pawn;

		assetBits.modelFiles[model] = true;

		// Tags
		auto& color(mgr.addComponent<ecs::CColor>(p).color);
		if (i < boardWidth)
		{
			color = colorWhite;
			mgr.addTag<TWhiteFigure>(p);
		}
		else
		{
			color = colorBlack;
			mgr.addTag<TBlackFigure>(p);
		}
	}
	// Rooks
	for (auto i = 0; i < 4; i++)
	{
		auto r(mgr.createIndex());

		auto& trans(mgr.addComponent<ecs::CTransform>(r, ecs::CTransform()));
		trans.position = DirectX::XMFLOAT3(
			i < 2 ? startX : -startX,
			0.f,
			i % 2 == 0 ? startZ : -startZ
		);
		trans.rotation = DirectX::XMFLOAT3();
		trans.scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);

		auto& pos(mgr.addComponent<CFieldPos>(r, CFieldPos()));
		pos.x = i < 2 ? 1 : boardWidth;
		pos.y = i % 2 == 0 ? 1 : boardWidth;

		auto& model(mgr.addComponent<ecs::CModel>(r).index);
		model = (std::size_t)Figure::Rook;

		assetBits.modelFiles[model] = true;

		auto& color(mgr.addComponent<ecs::CColor>(r).color);
		if (i % 2 == 0)
		{
			color = colorWhite;
			mgr.addTag<TWhiteFigure>(r);
		}
		else
		{
			color = colorBlack;
			mgr.addTag<TBlackFigure>(r);
		}
	}
	// Knights
	for (auto i = 0; i < 4; i++)
	{
		auto k(mgr.createIndex());

		auto& trans(mgr.addComponent<ecs::CTransform>(k, ecs::CTransform()));
		trans.position = DirectX::XMFLOAT3(
			i < 2 ? startX + fieldSize : -startX - fieldSize,
			0.f,
			i % 2 == 0 ? startZ : -startZ
		);
		trans.rotation = DirectX::XMFLOAT3();
		trans.scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);

		auto& pos(mgr.addComponent<CFieldPos>(k, CFieldPos()));
		pos.x = i < 2 ? 2 : boardWidth-1;
		pos.y = i % 2 == 0 ? 1 : boardWidth;

		auto& model(mgr.addComponent<ecs::CModel>(k).index);
		model = (std::size_t)Figure::Knight;

		assetBits.modelFiles[model] = true;

		auto& color(mgr.addComponent<ecs::CColor>(k).color);
		if (i % 2 == 0)
		{
			color = colorWhite;
			mgr.addTag<TWhiteFigure>(k);
		}
		else
		{
			color = colorBlack;
			mgr.addTag<TBlackFigure>(k);
		}
	}
	// Bishops
	for (auto i = 0; i < 4; i++)
	{
		auto b(mgr.createIndex());

		auto& trans(mgr.addComponent<ecs::CTransform>(b, ecs::CTransform()));
		trans.position = DirectX::XMFLOAT3(
			i < 2 ? startX + 2*fieldSize : -startX - 2*fieldSize,
			0.f,
			i % 2 == 0 ? startZ : -startZ
		);
		trans.rotation = DirectX::XMFLOAT3();
		trans.scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);

		auto& pos(mgr.addComponent<CFieldPos>(b, CFieldPos()));
		pos.x = i < 2 ? 3 : boardWidth - 2;
		pos.y = i % 2 == 0 ? 1 : boardWidth;

		auto& model(mgr.addComponent<ecs::CModel>(b).index);
		model = (std::size_t)Figure::Bishop;

		assetBits.modelFiles[model] = true;

		auto& color(mgr.addComponent<ecs::CColor>(b).color);
		if (i % 2 == 0)
		{
			color = colorWhite;
			mgr.addTag<TWhiteFigure>(b);
		}
		else
		{
			color = colorBlack;
			mgr.addTag<TBlackFigure>(b);
		}
	}
	// Queens
	for (auto i = 0; i < 2; i++)
	{
		auto q(mgr.createIndex());

		auto& trans(mgr.addComponent<ecs::CTransform>(q, ecs::CTransform()));
		trans.position = DirectX::XMFLOAT3(
			startX + 3*fieldSize,
			0.f,
			i == 0 ? startZ : -startZ
		);
		trans.rotation = DirectX::XMFLOAT3();
		trans.scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);

		auto& pos(mgr.addComponent<CFieldPos>(q, CFieldPos()));
		pos.x = 4;
		pos.y = i == 0 ? 1 : boardWidth;

		auto& model(mgr.addComponent<ecs::CModel>(q).index);
		model = (std::size_t)Figure::Queen;

		assetBits.modelFiles[model] = true;

		auto& color(mgr.addComponent<ecs::CColor>(q).color);
		if (i == 0)
		{
			color = colorWhite;
			mgr.addTag<TWhiteFigure>(q);
		}
		else
		{
			color = colorBlack;
			mgr.addTag<TBlackFigure>(q);
		}
	}
	// Kings
	for (auto i = 0; i < 2; i++)
	{
		auto k(mgr.createIndex());

		auto& trans(mgr.addComponent<ecs::CTransform>(k, ecs::CTransform()));
		trans.position = DirectX::XMFLOAT3(
			startX + 4*fieldSize,
			0.f,
			i == 0 ? startZ : -startZ
		);
		trans.rotation = DirectX::XMFLOAT3();
		trans.scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);

		auto& pos(mgr.addComponent<CFieldPos>(k, CFieldPos()));
		pos.x = 5;
		pos.y = i == 0 ? 1 : boardWidth;

		auto& model(mgr.addComponent<ecs::CModel>(k).index);
		model = (std::size_t)Figure::King;

		assetBits.modelFiles[model] = true;

		auto& color(mgr.addComponent<ecs::CColor>(k).color);
		if (i == 0)
		{
			color = colorWhite;
			mgr.addTag<TWhiteFigure>(k);
		}
		else
		{
			color = colorBlack;
			mgr.addTag<TBlackFigure>(k);
		}
	}
}


void Chess::Process(ubrot::Scene &scene)
{
	// Check if a mouse button was pressed

	// Check intersection

}

std::size_t Chess::FieldIndex(char one, std::size_t two)
{
	return (int)(one - 'a') * 8 + two;
}

