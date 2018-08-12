#pragma once
///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: defs.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DEFS_H_
#define _DEFS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "settings.h"


namespace ecs
{

// Components
struct CTransform
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
};
struct CModel
{
	std::size_t index;
};
struct CTexture
{
	std::size_t index;
};

using MyComponents = ComponentList<CTransform, CModel, CTexture>;

// Tags
struct TColor {};

using MyTags = TagList<TColor>;

using STransform = Signature<CTransform>;
using SRender = Signature<CModel, CTransform>;
using SRenderColor = Signature<CModel, CTransform, TColor>;
using SRenderTex = Signature<CModel, CTransform, CTexture>;

using MySignatures = SignatureList<STransform, SRender, SRenderColor, SRenderTex>;

using MySettings = Settings<MyComponents, MyTags, MySignatures>;

};

#endif // !_DEFS_H_