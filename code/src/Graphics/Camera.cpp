#include "Camera.h"

glm::mat4 my_lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up);

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	mSpeed(SPEED),
	mSensitivty(SENSITIVTY),
	mZoom(ZOOM)

{
	mPosition = position;
	mWorldUp = up;
	mPitch = pitch;
	mYaw = yaw;
	updateCameraVectors();
}

void Camera::processKeyboardInput(CameraMovement direction, float dt)
{
	// glm::vec3 fpsFront(mFront.x , 0, mFront.z); // replace mFront with this one

	if (direction == FORWARD)
		mPosition += mFront * mSpeed * dt;
	if (direction == BACKWARD)
		mPosition -= mFront * mSpeed * dt;
	if (direction == LEFT)
		mPosition -= mRight * mSpeed * dt;
	if (direction == RIGHT)
		mPosition += mRight * mSpeed * dt;
	
	if (direction == UP)
		mPosition += mWorldUp * mSpeed * dt;
	if (direction == DOWN)
		mPosition -= mWorldUp * mSpeed * dt;
}

void Camera::processMouseInput(float xoffset, float yoffset, bool constrainPitch)
{
	mPitch += yoffset * mSensitivty; // moving the mouse up or down
	mYaw += xoffset * mSensitivty; // moving the mouse left or right

	if (constrainPitch)
	{
		if (mPitch>89.0f)
			mPitch = 89.0f;
		if (mPitch<-89.0f)
			mPitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseScrollInput(float yoffset)
{
	if (mZoom >= 1.0f && mZoom <= 45.0f)
		mZoom -= yoffset;
	if (mZoom <= 1.0f)
		mZoom = 1.0f;
	if (mZoom >= 45.0f)
		mZoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
	front.y = sin(glm::radians(mPitch));
	front.z = cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));
	mFront = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));

}

void Camera::addSpeed(float speed)
{
    mSpeed += speed;
}
