#pragma once

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

static const float PITCH = 0.0f;
static const float YAW = -90.0f;
static const float SPEED = 2.5f;
static const float SENSITIVTY = 0.1f;
static const float ZOOM = 45.0f;

class Camera
{
public:
	enum CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	inline glm::mat4 getViewMatrix() const { return glm::lookAt(mPosition, mPosition + mFront, mUp); }
	inline float getZoom() const { return mZoom; }
	inline glm::vec3 getPosition() const { return mPosition; }

	void processKeyboardInput(CameraMovement direction, float dt);
	void processMouseInput(float xoffset, float yoffset, bool constrainPitch = true);
	void processMouseScrollInput(float yoffset);

	void addSpeed(float speed);


private:
	void updateCameraVectors();

public:
	glm::vec3 mFront;

private:
	// Camera attributes
	glm::vec3 mPosition;
	glm::vec3 mRight;
	glm::vec3 mUp;
	glm::vec3 mWorldUp;

	// Eular angles
	float mPitch, mYaw;

	// Camera options
	float mSpeed;
	float mSensitivty;
	float mZoom;
};