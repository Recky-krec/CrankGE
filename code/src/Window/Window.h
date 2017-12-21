#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <iostream>
#include <string>
#include "Utils/Log.h"

namespace crank
{

enum class Monitor
{
	Primary = 0,
	Secondary = 1,
	Tertiary = 2
};

class Window
{
public:
	Window();
	Window(float width, float height, const char* title);
	~Window();
	void Create(float width, float height, const char* title);
	
	bool IsOpen() const;
	void Close();

	void SetPosition(int x, int y);
	void SetSize(int width, int height);
	void SetTitle(const char* title);
	void SetIcon(const char* path);
	void SetMouseCursorVisible(bool visible = true);
	void SetMouseCursorLocked(bool locked = true);

	glm::ivec2 GetPosition() const;
	glm::ivec2 GetSize() const;
	inline GLFWwindow* GetWindowPtr() const { return m_window; }
	
	void RequestFocus();
	void GoFullScreen(Monitor monitor, bool fullscreen = true);

	void SetActive(bool active = true) const;
	void SwapBuffers();

public:
	static void SetHint(int hint, int value);

private:
	void OnCreation();

private:
	GLFWwindow* m_window;
	float m_width, m_height;
};

} // namespace crank

