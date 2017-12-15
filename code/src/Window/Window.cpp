#include "Window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


// Callbacks signatures
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window()
	: m_width(0),
	  m_height(0)
{
}

Window::Window(float width, float height, const char* title)
	: m_window(glfwCreateWindow(width, height, title, NULL, NULL)),
	  m_width(width),
	  m_height(height)
{
	onCreation();
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

void Window::create(float width, float height, const char* title)
{
	m_width = width;
	m_height = height;
	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	onCreation();
}

glm::ivec2 Window::getPosition() const
{
	glm::ivec2 result;
	glfwGetWindowPos(m_window, &result.x, &result.y);
	return result;
}

bool Window::isOpen() const
{
	return !glfwWindowShouldClose(m_window);
}

void Window::setActive(bool active) const
{
	/* Make the window's context current */
	if(active)
	{
		glfwMakeContextCurrent(m_window);
	} else {
		glfwMakeContextCurrent(NULL);
	}
}

void Window::swapBuffers()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(m_window);
}

void Window::close()
{
	glfwSetWindowShouldClose(m_window, true);
}

void Window::setPosition(int x, int y)
{
	glfwSetWindowPos(m_window, x, y);
}

glm::ivec2 Window::getSize() const
{
	glm::ivec2 result;
	glfwGetWindowSize(m_window, &result.x, &result.y);
	return result;
}

void Window::setSize(int width, int height)
{
	glfwSetWindowSize(m_window, width, height);
}

void Window::setTitle(const char* title)
{
	glfwSetWindowTitle(m_window, title);
}

void Window::setMouseCursorVisible(bool visible)
{
	if(!visible)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	} else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Window::setMouseCursorLocked(bool locked)
{
	if(locked)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Window::setIcon(const char* path)
{
	GLFWimage icon;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data) { std::cout << "Failed to load image at: " << path << std::endl; }

	icon.pixels = data;
	icon.width = width;
	icon.height = height;

	glfwSetWindowIcon(m_window, 1, &icon);
}

void Window::requestFocus()
{
	glfwFocusWindow(m_window);
}

void Window::onCreation()
{
	// Checks if window initialized correctly
	if (!m_window)
	{
		std::cout << "Failed to create GLFW Window and Context, terminating..." << std::endl;
		glfwTerminate();
	}

	// Sets the callbacks
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
}


void Window::setHint(int hint, int value)
{
	glfwWindowHint(hint, value);
}

void Window::goFullScreen(Monitor monitor, bool fullscreen)
{
	// Gets all monitors to the array
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);

	// +1 because enum class Monitor starts from 0
	if(count < (int)monitor+1)
	{
		std::cout << "Unavailable monitor number: " << (int)monitor+1 << ". You currently have " << count << " monitor/s detected." << std::endl;
		std::cout << "Terminating..." << std::endl;
		glfwTerminate();
	}
	
	// Gets monitor video mode
	GLFWmonitor* selectedMonitor = monitors[(int)monitor];
	const GLFWvidmode* mode = glfwGetVideoMode(selectedMonitor);

	if(fullscreen)
	{
		glfwSetWindowMonitor(m_window, selectedMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	} else {
		// By default, selecting goFullScreen with false will retrieve the screen in the primary monitor
		glfwSetWindowMonitor(m_window, NULL, 0, 0, m_width, m_height, mode->refreshRate);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}