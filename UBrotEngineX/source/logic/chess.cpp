#include "../../header/logic/chess.h"

#include "../../header/scene.h"
#include "../../header/physics/physics.h"
#include "../../header/io/assetmanager.h"

namespace gv = ubrot::graphics::vertices;

const auto FieldModel = ubrot::assets::Procedural::Plane;

// TODO: ueberarbeiten ?
Chess::Chess(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	m_input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
}


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
	RegisterModels();

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
		model = (int)Figure::Pawn;

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
		model = (int)Figure::Rook;

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
		model = (int)Figure::Knight;

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
		model = (int)Figure::Bishop;

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
		model = (int)Figure::Queen;

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
		model = (int)Figure::King;

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

	auto fieldModelIndex = ubrot::assets::GetProdIndex(FieldModel);
	// Fields
	for (auto i = 0; i < boardWidth; i++)
	{
		for (auto j = 0; j < boardWidth; j++)
		{
			auto f(mgr.createIndex());

			// Components
			auto& trans(mgr.addComponent<ecs::CTransform>(f, ecs::CTransform()));
			trans.position = DirectX::XMFLOAT3(
				startX + i * fieldSize,
				-0.05f,
				startZ + j * fieldSize
			);
			trans.rotation = DirectX::XMFLOAT3(DirectX::XM_PI/2 , 0.f, 0.f); // TODO: degree
			trans.scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);

			auto& pos(mgr.addComponent<CFieldPos>(f, CFieldPos()));
			pos.x = i < boardWidth ? i + 1 : (i + 1) % boardWidth;
			pos.y = i < boardWidth ? 1 : boardWidth;

			auto& model(mgr.addComponent<ecs::CModel>(f).index);
			model = -1 * (int)fieldModelIndex - 1;
			// mgr.addTag<ecs::TProcedural>(f); // indicate that this is not a "regular" model

			auto& color(mgr.addComponent<ecs::CColor>(f).color);
			if ((i + j) % 2 == 1)
			{
				color = DirectX::XMFLOAT4(0.f, 0.545f, 0.545f, 1.f); //colorWhite;
				mgr.addTag<TWhiteFigure>(f);
			}
			else
			{
				color = DirectX::XMFLOAT4(0.737f, 0.561f, 0.561f, 1.f); //colorBlack;
				mgr.addTag<TBlackFigure>(f);
			}
		}
	}
}


void Chess::Process(ubrot::Scene &scene)
{
	if (!m_input->Frame())
	{
		throw std::runtime_error("BLAHFDREIW");
	}

	auto& cam = scene.GetCamera();

	CheckMovement(cam);

	// Check if a mouse button was pressed
	if (m_input->IsLeftMouseButtonDown())
	{
		int mPosX = 0; // TODO als rückgabe ?
		int mPosY = 0;
		m_input->GetMouseLocation(mPosX, mPosY);
		//mPosX = 960;
		//mPosY = 540;

		int intersect = -1;
		auto& mgr = scene.GetTiles()[0].GetManager(); // TODO
		// For all entities that are either a figure or a field
		// perform an intresection test
		mgr.forEntitiesMatching<SCheckIntersect>(
			[&](auto index, auto &cTrans, auto& cPos)
		{
			auto tmp = ubrot::physics::TestIntersection(
				mPosX,
				mPosY,
				cTrans.position.x,
				cTrans.position.y,
				cTrans.position.z
			);

			if (tmp) {
				intersect = (int)index;
			}
		});

		if (intersect >= 0)
		{
			//ubrot::g_mouseX = 1920;
			//ubrot::g_mouseY = 1080;
			if (m_lastIntersect >= 0)
			{
				auto& color = mgr.getComponent<ecs::CColor>((EntityIndex)intersect);
				color.color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

				if (IsValidMove(mgr, m_lastIntersect, intersect))
				{
					// Perform the move if the clicked figure/field represents a valid move
				}
				else if (IsValidSelection(mgr, intersect))
				{
					// Set selection to just clicked figure/field if it was valid
				}
				else
				{
					// Reset selection if it was all invalid
					m_lastIntersect = -1;
				}
			}
			else if (true || IsValidSelection(mgr, intersect)) // TODO: !
			{
				auto& color = mgr.getComponent<ecs::CColor>((EntityIndex)intersect);
				color.color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f);
				// If we did not select anything previously and this is a valid selection, save it
				m_lastIntersect = intersect;
			}
		}
	}

	if (m_input->IsRightMouseButtonDown())
	{
		ubrot::g_mouseX = 0;
		ubrot::g_mouseY = 0;
	}

}


void Chess::RegisterModels()
{
	ubrot::assets::GetProdModel(FieldModel);
}


void Chess::CheckMovement(ubrot::graphics::Camera & camera)
{
	auto moveSpeed = 0.01f;

	if (m_input->IsKeyDown(DIK_LEFTARROW))
		camera.GoLeft(moveSpeed);
	if (m_input->IsKeyDown(DIK_RIGHTARROW))
		camera.GoRight(moveSpeed);

	if (m_input->IsKeyDown(DIK_E))
		camera.GoDown(moveSpeed);
	if (m_input->IsKeyDown(DIK_Q))
		camera.GoUp(moveSpeed);

	if (m_input->IsKeyDown(DIK_W))
		camera.GoForward(moveSpeed);
	if (m_input->IsKeyDown(DIK_S))
		camera.GoBack(moveSpeed);

	camera.TurnLeft(m_input->IsKeyDown(DIK_A));
	camera.TurnRight(m_input->IsKeyDown(DIK_D));

	camera.TurnDown(m_input->IsKeyDown(DIK_DOWNARROW));
	camera.TurnUp(m_input->IsKeyDown(DIK_UPARROW));
}


bool Chess::IsValidMove(ecs::Manager<AllSettings> &mgr, int from, int to)
{
	// TODO: check if the move is valid
	auto& pos1 = mgr.getComponent<CFieldPos>((ecs::EntityIndex)from);
	auto& pos2 = mgr.getComponent<CFieldPos>((ecs::EntityIndex)to);

	// Check if pos2 is a figure and if so, if it has the correct color (opposite)

	// Check what figure pos1 is and if pos2 is a valid move

	return false;
}


bool Chess::IsValidSelection(ecs::Manager<AllSettings> &mgr, int selection)
{
	return false;
}

