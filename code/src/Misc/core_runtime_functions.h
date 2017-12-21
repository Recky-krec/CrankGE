#pragma once

#include "Window/Window.h"
#include "Graphics/Camera.h"
#include "globals.h"

void process_input(crank::Window* window, float deltatime);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void initialize_glew();
void configure_opengl();