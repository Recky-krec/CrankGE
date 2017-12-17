#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <csignal>

#define CE_DEBUG 1

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#if CE_DEBUG == 1
#define LOG(x) std::cout << x << std::endl
#define TOGGLE_BREAKPOINT std::raise(SIGINT);  // probably only compatible with linux
#define ASSERT(x) if(!(x)) TOGGLE_BREAKPOINT;

// note: GLCall will fail in 1 line if statements
#define GLCall(x) GLClearError();\
                  x;\
                  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define TIME(x) std::cout << "----------- Call to: '" << #x << "' -----------" << std::endl;\
                  std::cout << "Starting time: " << glfwGetTime() << "\n";\
                  x;\
                  std::cout << "Ending time: " << glfwGetTime() << "\n";
#elif CE_DEBUG == 0
#define LOG(x)
#define TOGGLE_BREAKPOINT
#define ASSERT(x)
#define GLCall(x) x;
#define TIME(x) x;
#endif