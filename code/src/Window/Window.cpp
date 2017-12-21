#include "Window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace crank
{

// Callbacks signatures
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window()
	: m_width(0),
	  m_height(0)
{}

Window::Window(float width, float height, const char* title)
	: m_window(glfwCreateWindow(width, height, title, NULL, NULL)),
	  m_width(width),
	  m_height(height)
{
	OnCreation();
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

void Window::Create(float width, float height, const char* title)
{
	m_width = width;
	m_height = height;
	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	OnCreation();
}

glm::ivec2 Window::GetPosition() const
{
	glm::ivec2 result;
	glfwGetWindowPos(m_window, &result.x, &result.y);
	return result;
}

bool Window::IsOpen() const
{
	return !glfwWindowShouldClose(m_window);
}

void Window::SetActive(bool active) const
{
	/* Make the window's context current */
	if(active)
	{
		glfwMakeContextCurrent(m_window);
	} else {
		glfwMakeContextCurrent(NULL);
	}
}

void Window::SwapBuffers()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(m_window);
}

void Window::Close()
{
	glfwSetWindowShouldClose(m_window, true);
}

void Window::SetPosition(int x, int y)
{
	glfwSetWindowPos(m_window, x, y);
}

glm::ivec2 Window::GetSize() const
{
	glm::ivec2 result;
	glfwGetWindowSize(m_window, &result.x, &result.y);
	return result;
}

void Window::SetSize(int width, int height)
{
	glfwSetWindowSize(m_window, width, height);
}

void Window::SetTitle(const char* title)
{
	glfwSetWindowTitle(m_window, title);
}

void Window::SetMouseCursorVisible(bool visible)
{
	if(!visible)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	} else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Window::SetMouseCursorLocked(bool locked)
{
	if(locked)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Window::SetIcon(const char* path)
{
	GLFWimage icon;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data)
	{
		//crank::Log::ReportingLevel(crank::Log::ERROR);
		LOGWL << "[WINDOW]:";
		LOGW  << "Failed to load image at: " << path;
	}

	icon.pixels = data;
	icon.width = width;
	icon.height = height;

	glfwSetWindowIcon(m_window, 1, &icon);
}

void Window::RequestFocus()
{
	glfwFocusWindow(m_window);
}

void Window::OnCreation()
{
	// Checks if window initialized correctly
	if (!m_window)
	{
		//crank::Log::ReportingLevel(crank::Log::ERROR);
		LOGEL << "[WINDOW]:";
		LOGE << "Failed to create GLFW Window and Context, terminating...";
		glfwTerminate();
	}

	// Sets the callbacks
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
}


void Window::SetHint(int hint, int value)
{
	glfwWindowHint(hint, value);
}

void Window::GoFullScreen(Monitor monitor, bool fullscreen)
{
	// Gets all monitors to the array
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);

	// +1 because enum class Monitor starts from 0
	if(count < (int)monitor+1)
	{
		//crank::Log::ReportingLevel(crank::Log::ERROR);
		LOGEL << "[WINDOW]:";
		LOGE << "Unavailable monitor number: " << (int)monitor+1 << ". You currently have " << count << " monitor/s detected.";
		LOGE << "Terminating...";
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

} // namespace crank
