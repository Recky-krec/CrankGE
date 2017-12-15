#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <iostream>
#include <string>

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
	void create(float width, float height, const char* title);
	
	bool isOpen() const;
	void close();

	void setPosition(int x, int y);
	void setSize(int width, int height);
	void setTitle(const char* title);
	void setIcon(const char* path);
	void setMouseCursorVisible(bool visible = true);
	void setMouseCursorLocked(bool locked = true);

	glm::ivec2 getPosition() const;
	glm::ivec2 getSize() const;
	inline GLFWwindow* getWindowPtr() { return m_window; }
	
	void requestFocus();
	void goFullScreen(Monitor monitor, bool fullscreen = true);

	void setActive(bool active = true) const;
	void swapBuffers();

public:
	static void setHint(int hint, int value);

private:
	void onCreation();

private:
	GLFWwindow* m_window;
	float m_width, m_height;
};

