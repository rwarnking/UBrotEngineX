///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: camera.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <math.h>
#include <directxmath.h>
using namespace DirectX;


///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
/// The camera class represents a virtual camera with a position and rotation. This camera
/// can be used to create different view matrices for rendering.
///////////////////////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:
	/**
	 * A constructor where all the member variables are initialised to be
	 * at the origin of the scene (0, 0, 0).
	 */
	Camera();
	/**
	 * Empty copy contructor
	 * @param other to copy
	 */
	Camera(const Camera& other);
	~Camera();

	/**
	 * Needed for 16 Bit allignement
	 */
	void* operator new(size_t n) throw();
	void operator delete(void * p) throw();

	/**
	 * Set the frame time needed to get smooth camera movement, depending on the FPS.
	 * @param time
	 */
	void SetFrameTime(float time);
	/**
	 * Reset the rotation and turnspeed on all axes to 0.
	 */
	void ResetRotation();

	/**
	 * \defgroup CameraTurn A group of functions that rotate the camera.
	 * This function is called each frame as long as the corresponding key is pressed. It
	 * increases the turn speed until some threshold, dependent on \a m_frameTime, is reached.
	 * When the key is released, the turn speed is iteratively decreased. Afterwards, the
	 * camera's rotation is updated. This produces smooth movement that does not abruptly end
	 * as soon as the corresponding key is released.
	 * @param keydown
	 * @{
	 */
	void TurnLeft(bool);
	void TurnRight(bool);
	void TurnUp(bool);
	void TurnDown(bool);
	/** @} */

	/**
	 * \defgroup CameraMove A Group of functions that move the camera.
	 * Moves the camera \p amount units in the corresponding direction.
	 * @param amount
	 * @{
	 */
	void GoLeft(float amount);
	void GoRight(float amount);
	void GoUp(float amount);
	void GoDown(float amount);
	void GoForward(float amount);
	void GoBack(float amount); // TODO backwards ?
	/** @} */

	/**
	 * Sets the camera position to the given values.
	 * @param x setzt \a m_positionX
	 * @param y setzt \a m_positionY
	 * @param z setzt \a m_positionZ
	 */
	void SetPosition(float x, float y, float z);
	/**
	 * Sets the camera rotation to the given values. The turn speed is always reset.
	 * @param x setzt \a m_rotationX
	 * @param y setzt \a m_rotationY
	 * @param z setzt \a m_rotationZ
	 */
	void SetRotation(float x, float y, float z);

	/**
	 * Returns the current camera position.
	 * @return XMFLOAT3
	 */
	XMFLOAT3 GetPosition();
	/**
	* Returns the current camera rotation.
	 * @return XMFLOAT3
	 */
	XMFLOAT3 GetRotation();

	/**
	 * Computes the view matrix using \c XMMatrixLookAtLH, the camera position and camera
	 * rotation. This function should be called every frame to ensure correctness.
	 */
	void Render();
	/**
	 * Copies the computed view matrix into the parameter \p viewMatrix to be used elsewhere.
	 * param[out] viewMatrix
	 */
	void GetViewMatrix(XMMATRIX& viewMatrix);

	/**
	 * Computes the reflection matrix using \c XMMatrixLookAtLH, the camera position and camera
	 * rotation. This function should be called every frame to ensure correctness.
	 * TODO diese Matrix funktioniert aktuell nur für Objekte, die "liegen"
	 * @param height the height at which the reflecting objects are positioned
	 */
	void RenderReflectionMatrix(float height); // TODO: refactor, doesnt really make sense
	/**
	 * Copies the computed reflection matrix into the parameter \p viewMatrix to be used elsewhere.
	 * param[out] reflection_matrix
	 */
	void GetReflectionMatrix(XMMATRIX& reflectionMatrix);

	/**
	 * Computes the base view matrix using \c XMMatrixLookAtLH, the camera position and camera
	 * rotation. This function should be called once at the beginning of the application and not
	 * updated again afterwards to ensure correctness (e.g. for GUI-rendering).
	 */
	void RenderBaseViewMatrix(); // TODO; make better, less duplicated code
	/**
	 * Copies the computed base view matrix into the parameter \p baseViewMatrix
	 * to be used elsewhere.
	 * param[out] baseViewMatrix
	 */
	void GetBaseViewMatrix(XMMATRIX& baseViewMatrix);

private:
	float m_frameTime;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix, m_baseViewMatrix, m_reflection_matrix;
};

#endif