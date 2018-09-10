///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: physics.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <DirectXMath.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////


namespace ubrot
{

/////////////
// GLOBALS //
/////////////
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

namespace physics
{

void SetCameraPosition(DirectX::XMFLOAT3 cam);

void SetUniforms(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMMATRIX world);

/// Get the field in board coordinates that the player selected using a ray intersection test
bool TestIntersection(int mPosX, int mPosY, float xModel, float yModel, float zModel);

};
};