#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <csignal>

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#define LOG(x) std::cout << x;

// Probably only compatible with linux
#define TOGGLE_BREAKPOINT std::raise(SIGINT);
#define ASSERT(x) if(!(x)) TOGGLE_BREAKPOINT;

// note: GLCall will fail in 1 line if statements
#define GLCall(x) GLClearError();\
                  x;\
                  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define TIME(x) std::cout << "----------- Call to: '" << #x << "' -----------" << std::endl;\
                  std::cout << "Starting time: " << glfwGetTime() << "\n";\
                  x;\
                  std::cout << "Ending time: " << glfwGetTime() << "\n";