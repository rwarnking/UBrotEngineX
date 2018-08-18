///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: gameobjects.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../ecs/settings.h"
#include "../ecs/defs.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
/// TODO
///////////////////////////////////////////////////////////////////////////////////////////////////
struct CFieldPos
{
	// Position on a chess board
	int x, y;
};

struct TWhiteFigure {};
struct TBlackFigure {};

using SCheckIntersect = ecs::Signature<ecs::CTransform, CFieldPos>;

using ChessComps = ecs::ComponentList<CFieldPos>;
using ChessTags = ecs::TagList<TWhiteFigure, TBlackFigure>;
using ChessSigns = ecs::SignatureList<SCheckIntersect>;

//using ChessSettings = ecs::Settings<ChessComps, ChessTags, ChessSigns>;

using AllSettings = ecs::Settings
<
	ecs::MPL::Impl::Concat<ecs::MyComponents, ChessComps>,
	ecs::MPL::Impl::Concat<ecs::MyTags, ChessTags>,
	ecs::MPL::Impl::Concat<ecs::MySignatures, ChessSigns>
>;