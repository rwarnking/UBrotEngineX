#pragma once

#include "../ecs/settings.h"
#include "../ecs/defs.h"

struct CFieldPos
{
	// Position on a chess board
	int x, y;
};

struct TWhiteFigure {};
struct TBlackFigure {};

using ChessComps = ecs::ComponentList<CFieldPos>;
using ChessTags = ecs::TagList<TWhiteFigure, TBlackFigure>;
using ChessSigns = ecs::SignatureList<>;

//using ChessSettings = ecs::Settings<ChessComps, ChessTags, ChessSigns>;

using AllSettings = ecs::Settings
<
	ecs::MPL::Impl::Concat<ecs::MyComponents, ChessComps>,
	ecs::MPL::Impl::Concat<ecs::MyTags, ChessTags>,
	ecs::MPL::Impl::Concat<ecs::MySignatures, ChessSigns>
>;