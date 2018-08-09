///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: defs.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DEFS_H_
#define _DEFS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "settings.h"

namespace ecs
{

struct CTransform
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
};
struct CColor
{
	DirectX::XMFLOAT4 color;
};
struct CModel
{
	std::size_t index;
};
struct CTexture
{
	std::size_t index;
};

using MyComponents = ComponentList<CTransform, CColor, CModel, CTexture>;

using MyTags = TagList<>;

using STransform = Signature<CTransform>;
using SRenderColor = Signature<CModel, CTransform, CColor>;
using SRenderTex = Signature<CModel, CTransform, CTexture>;

using MySignatures = SignatureList<SRenderColor, SRenderTex>;

using MySettings = Settings<MyComponents, MyTags, MySignatures>;

};

#endif // !_DEFS_H_