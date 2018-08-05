///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: camera.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../../header/graphics/camera.h"


Camera::Camera()
{
	m_frameTime = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed = 0.0f;
	m_lookDownSpeed = 0.0f;

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


Camera::Camera(const Camera& other)
{
}


Camera::~Camera()
{
}


void* Camera::operator new(size_t n) throw()
{
	return _aligned_malloc(n, 16);
}


void Camera::operator delete(void * p) throw()
{
	_aligned_free(p);
}


void Camera::SetFrameTime(float time)
{
	m_frameTime = time;
}


void Camera::ResetRotation()
{
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
}


void Camera::TurnLeft(bool keydown)
{
	if (keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if (m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 0.005f;

		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	m_rotationY -= m_leftTurnSpeed;
	if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}

	return;
}


void Camera::TurnRight(bool keydown)
{
	if (keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if (m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime * 0.005f;

		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	m_rotationY += m_rightTurnSpeed;
	if (m_rotationY> 360.0f)
	{
		m_rotationY -= 360.0f;
	}


	return;
}


void Camera::TurnUp(bool keydown)
{
	if (keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.01f;

		if (m_lookUpSpeed > (m_frameTime * 0.15f))
		{
			m_lookUpSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime * 0.005f;

		if (m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	m_rotationX -= m_lookUpSpeed;

	// Sorge dafür, dass die Kamera bei -90 Grad (also wenn sie genau nach oben schaut)
	// anhällt und sich nicht weiter roteriert. Das verhindert "unnatuerliche" Kopfbewegungen
	if (m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}

	return;
}


void Camera::TurnDown(bool keydown)
{
	if (keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.01f;

		if (m_lookDownSpeed > (m_frameTime * 0.15f))
		{
			m_lookDownSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime * 0.005f;

		if (m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	m_rotationX += m_lookDownSpeed;

	// Bound the camera rotation at 90 degrees so we don't get unnatural head movements.
	if (m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}

	return;
}


void Camera::GoRight(float amount)
{
	m_positionX += cosf(m_rotationY * XM_PI / 180.0f) * amount;
	m_positionZ -= sinf(m_rotationY * XM_PI / 180.0f) * amount;
}


void Camera::GoLeft(float amount)
{
	m_positionX -= cosf(m_rotationY * XM_PI / 180.0f) * amount;
	m_positionZ += sinf(m_rotationY * XM_PI / 180.0f) * amount;
}


void Camera::GoBack(float amount)
{
	m_positionX -= sinf(m_rotationY * XM_PI / 180.0f) * amount;
	m_positionZ -= cosf(m_rotationY * XM_PI / 180.0f) * amount;
}


void Camera::GoForward(float amount)
{
	m_positionX += sinf(m_rotationY * XM_PI / 180.0f) * amount;
	m_positionZ += cosf(m_rotationY * XM_PI / 180.0f) * amount;
}


void Camera::GoUp(float amount)
{
	m_positionY += amount;
}


void Camera::GoDown(float amount)
{
	m_positionY -= amount;
}


void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}


void Camera::SetRotation(float x, float y, float z)
{
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;

	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}


XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}


XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}


void Camera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Initialize the up vector and convert it to a XMVECTOR
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	// Initialize the position vector and convert it to a XMVECTOR
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	positionVector = XMLoadFloat3(&position);

	// Initialize the look-at vector and convert it to a XMVECTOR
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// Create a rotation matrix around each axis (yaw, pitch and roll) whose content is in radians
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the look-at and up vectors using the rotation matrix
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the look-at vector to the current camera position
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Create the view matrix from the three vectors
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}


void Camera::RenderReflectionMatrix(float height)
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Initialize the up vector and convert it to a XMVECTOR
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	// Initialize the position vector and convert it to a XMVECTOR and make the y value
	// correspond to the height of the reflecting object
	position.x = m_positionX;
	position.y = -m_positionY + height * 2.0f;
	position.z = m_positionZ;

	positionVector = XMLoadFloat3(&position);

	// Initialize the look-at vector and convert it to a XMVECTOR
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// Create a rotation matrix around each axis (yaw, pitch and roll) whose content is in radians
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(-pitch, yaw, -roll);

	// Transform the look-at and up vectors using the rotation matrix
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the look-at vector to the current camera position
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Create the reflection matrix from the three vectors
	m_reflection_matrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


void Camera::GetReflectionMatrix(XMMATRIX& reflection_matrix)
{
	reflection_matrix = m_reflection_matrix;
	return;
}


void Camera::RenderBaseViewMatrix()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Initialize the up vector and convert it to a XMVECTOR
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	// Initialize the position vector and convert it to a XMVECTOR
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	positionVector = XMLoadFloat3(&position);

	// Initialize the look-at vector and convert it to a XMVECTOR
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// Create a rotation matrix around each axis (yaw, pitch and roll) whose content is in radians
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the look-at and up vectors using the rotation matrix
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the look-at vector to the current camera position
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Create the view matrix from the three vectors
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


void Camera::GetBaseViewMatrix(XMMATRIX& baseViewMatrix)
{
	baseViewMatrix = m_baseViewMatrix;
	return;
}