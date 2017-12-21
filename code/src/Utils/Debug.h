#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Utils/Log.h"
#include <iostream>
#include <csignal>

#define CE_DEBUG 1

void glClearError();
bool glLogCall(const char* function, const char* file, int line);

#if CE_DEBUG == 1
#define TOGGLE_BREAKPOINT std::raise(SIGINT);  // probably only compatible with linux
#define ASSERT(x) if(!(x)) TOGGLE_BREAKPOINT;

// note: GLCALL will fail in 1 line if statements
#define GLCALL(x) glClearError();\
                  x;\
                  ASSERT(glLogCall(#x, __FILE__, __LINE__))

#define TIME(x)   LOGIL <<"[TIME]:";\
                  LOGI  << "Starting time: " << glfwGetTime();\
                  LOGI  << "Call to: '" << #x << "'";\
                  x;\
                  LOGI  << "Ending time: " << glfwGetTime();

#elif CE_DEBUG == 0
#define LOG(x)
#define TOGGLE_BREAKPOINT
#define ASSERT(x)
#define GLCALL(x) x;
#define TIME(x) x;
#endif