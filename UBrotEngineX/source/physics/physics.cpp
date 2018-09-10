#include "../../header/physics/physics.h"

#include <d3d11.h>
#include "SimpleMath.h"

#include <limits>

using namespace DirectX;

XMMATRIX viewMatrix, projMatrix, worldMatrix;
XMFLOAT3 camPosition;

namespace ubrot
{
namespace physics
{


void SetCameraPosition(XMFLOAT3 cam)
{
	camPosition = cam;
}


void SetUniforms(XMMATRIX view, XMMATRIX proj, XMMATRIX world)
{
	viewMatrix = view;
	projMatrix = proj;
	worldMatrix = world;
}


/// Ray intersection test with a sphere
bool RayIntersect(XMFLOAT3 origin, XMFLOAT3 direction)
{
	auto radius = 0.5f;
	// TODO: Plane intersection test?
	auto a = (direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z);
	auto b = ((direction.x * origin.x) + (direction.y * origin.y) + (direction.z * origin.z)) * 2.0f;
	auto c = ((origin.x * origin.x) + (origin.y * origin.y) + (origin.z * origin.z)) - (radius * radius);

	auto discriminant = (b*b) - (4.0f * a * c);

	return discriminant >= 0.0f;
}


bool TestIntersection(int mPosX, int mPosY, float xModel, float yModel, float zModel)
{
	auto x = ((2.0f * mPosX) / SCREEN_WIDTH) - 1.0f; // TODO
	auto y = (((2.0f * mPosY) / SCREEN_HEIGHT) - 1.0f) * -1.0f;

	/*
	x = x / self.uniform.proj.x.x;
	y = y / self.uniform.proj.y.y;
	*/
	x = x / projMatrix.r[0].m128_f32[0];
	y = y / projMatrix.r[1].m128_f32[1];

	auto inverse = XMMatrixInverse(nullptr, viewMatrix);
	auto direction = XMFLOAT3{
		(x * inverse.r[0].m128_f32[0]) + (y * inverse.r[1].m128_f32[0]) + inverse.r[2].m128_f32[0],
		(x * inverse.r[0].m128_f32[1]) + (y * inverse.r[1].m128_f32[1]) + inverse.r[2].m128_f32[1],
		(x * inverse.r[0].m128_f32[2]) + (y * inverse.r[1].m128_f32[2]) + inverse.r[2].m128_f32[2]
	};

	auto translate = XMMatrixTranslation(xModel, yModel, zModel);
	auto worldTranslate = XMMatrixMultiply(worldMatrix, translate);
	auto inverseWorld = XMMatrixInverse(nullptr, worldTranslate);

	auto tmp1 = XMLoadFloat3(&direction);
	//auto tmp2 = XMVector3Normalize(tmp1);

	auto rayOrigin = XMVector3TransformCoord(XMLoadFloat3(&camPosition), inverseWorld);
	auto rayDirection = XMVector3TransformNormal(tmp1, inverseWorld);

	rayDirection = XMVector3Normalize(rayDirection);

	//auto ray = sm::Ray(rayOrigin, rayDirection);

	//float dist = 0.f;
	//return ray.Intersects(
	//	BoundingSphere(XMFLOAT3(xModel, yModel, zModel), 0.5f), dist
	//);

	XMFLOAT3 origin, dir;
	XMStoreFloat3(&origin, rayOrigin);
	XMStoreFloat3(&dir, rayDirection);

	//dir = XMFLOAT3(0.0f, -1.0f, 0.0f);
	//origin = XMFLOAT3(2.0f, 5.0f, 2.0f);

	return RayIntersect(origin, dir);
}


};
};