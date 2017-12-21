#pragma once

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace crank
{

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

	inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_position, m_position + Front, m_up); }
	inline float GetZoom() const { return m_zoom; }
	inline glm::vec3 GetPosition() const { return m_position; }

	void ProcessKeyboardInput(CameraMovement direction, float dt);
	void ProcessMouseInput(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessMouseScrollInput(float yoffset);

	void AddSpeed(float speed);


private:
	void UpdateCameraVectors();

public:
	glm::vec3 Front;

private:
	// Camera attributes
	glm::vec3 m_position;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_worldUp;

	// Eular angles
	float m_pitch, m_yaw;

	// Camera options
	float m_speed;
	float m_sensitivty;
	float m_zoom;
};

} // namespace crank
