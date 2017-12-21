#include "Camera.h"

namespace crank
{

glm::mat4 my_lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up);

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_speed(SPEED),
	m_sensitivty(SENSITIVTY),
	m_zoom(ZOOM)

{
	m_position = position;
	m_worldUp = up;
	m_pitch = pitch;
	m_yaw = yaw;
    UpdateCameraVectors();
}

void Camera::ProcessKeyboardInput(CameraMovement direction, float dt)
{
	// glm::vec3 fpsFront(Front.x , 0, Front.z); // replace Front with this one

	if (direction == FORWARD)
		m_position += Front * m_speed * dt;
	if (direction == BACKWARD)
		m_position -= Front * m_speed * dt;
	if (direction == LEFT)
		m_position -= m_right * m_speed * dt;
	if (direction == RIGHT)
		m_position += m_right * m_speed * dt;
	
	if (direction == UP)
		m_position += m_worldUp * m_speed * dt;
	if (direction == DOWN)
		m_position -= m_worldUp * m_speed * dt;
}

void Camera::ProcessMouseInput(float xoffset, float yoffset, bool constrainPitch)
{
	m_pitch += yoffset * m_sensitivty; // moving the mouse up or down
	m_yaw += xoffset * m_sensitivty; // moving the mouse left or right

	if (constrainPitch)
	{
		if (m_pitch>89.0f)
			m_pitch = 89.0f;
		if (m_pitch<-89.0f)
			m_pitch = -89.0f;
	}

    UpdateCameraVectors();
}

void Camera::ProcessMouseScrollInput(float yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= yoffset;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::UpdateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	front.y = sin(glm::radians(m_pitch));
	front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	Front = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(Front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, Front));

}

void Camera::AddSpeed(float speed)
{
    m_speed += speed;
}

} // namespace crank
