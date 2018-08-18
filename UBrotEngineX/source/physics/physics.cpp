#include "../../header/physics/physics.h"

DirectX::XMMATRIX viewMatrix, projMatrix, worldMatrix;

namespace ubrot
{
namespace physics
{

void SetUniforms(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, DirectX::XMMATRIX world)
{
	viewMatrix = view;
	projMatrix = proj;
	worldMatrix = world;
}


int TestIntersection(int mPosX, int mPosY, float xModel, float yModel, float zModel)
{
	auto x = ((2.0f * mPosX) / SCREEN_WIDTH) - 1.0f; // TODO
	auto y = (((2.0f * mPosY) / SCREEN_HEIGHT) - 1.0f) * -1.0f;

	/*
	x = x / self.uniform.proj.x.x;
	y = y / self.uniform.proj.y.y;
	*/
	DirectX::XMVECTOR *det = nullptr;
	auto inverse = DirectX::XMMatrixInverse(det, viewMatrix);
	auto direction = DirectX::XMFLOAT3{
		(x * inverse.r[0].m128_f32[0]) + (y * inverse.r[1].m128_f32[0]) + inverse.r[2].m128_f32[0],
		(x * inverse.r[0].m128_f32[1]) + (y * inverse.r[1].m128_f32[1]) + inverse.r[2].m128_f32[1],
		(x * inverse.r[0].m128_f32[2]) + (y * inverse.r[1].m128_f32[2]) + inverse.r[2].m128_f32[2]
	};

	auto translate = DirectX::XMMatrixTranslation(xModel, yModel, zModel);
	auto worldTranslate = worldMatrix * translate;
	auto inverseWorld = DirectX::XMMatrixInverse(det, worldTranslate);

	/*
	if let Some(inverse) = self.uniform.view.invert() {
		let direction = Vector3{ x: (x * inverse.x.x) + (y * inverse.y.x) + inverse.z.x,
										 y: (x * inverse.x.y) + (y * inverse.y.y) + inverse.z.y,
										 z: (x * inverse.x.z) + (y * inverse.y.z) + inverse.z.z };


		for i in 0..self.field_positions.len() {
			for index in 0..self.field_positions[i].len() {
				let mut world = self.uniform.world.clone();
				// Translate to field's position
				let translation = Matrix4::from_translation(Vector3 {
														x: self.field_positions[i][index].x,
														y: self.field_positions[i][index].y,
														z: self.field_positions[i][index].z
													  });
				world = world * translation;

				let inverse_world = world.invert().unwrap();
				// Calculate ray direction
				let mut ray_direction = inverse_world.transform_vector(direction.normalize());
				ray_direction = ray_direction.normalize();
				// Calculate ray origin
				let ray_origin = inverse_world.transform_point(self.camera);

				// If we intersect return field coordinates in board coordinates
				if GraphicsEngine::ray_intersect(&ray_origin, &ray_direction) {
					return Some(self.map_field_positions(i, index))
				}
			}
		}
	} else {
		println!("Could not invert view matrix");
		return None
	}

	*/

	return 0;
}


/// Maps a field position in the 3D world to a board position
DirectX::XMFLOAT2 MapFieldPositions(int i, int j)
{
	return DirectX::XMFLOAT2(/*(self.field_positions[i][j].x + 4.0f), (4.0f - self.field_positions[i][j].z)*/);
}


/// Ray intersection test with a sphere
bool RayIntersect(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 direction)
{
	// TODO: Plane intersection test?
	auto a = (direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z);
	auto b = ((direction.x * origin.x) + (direction.y * origin.y) + (direction.z * origin.z)) * 2.0f;
	auto c = ((origin.x * origin.x) + (origin.y * origin.y) + (origin.z * origin.z)) - (0.5f * 0.5f);

	auto discriminant = (b*b) - (4.0f * a * c);

	return discriminant > 0.0f;
}

};
};