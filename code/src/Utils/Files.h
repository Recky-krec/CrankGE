#pragma once

#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include "Utils/Log.h"

#define GLEW_STATIC
#include "GL/glew.h"
#include "stb/stb_image.h"
namespace crank
{
std::string readFile(const std::string& path);
unsigned int loadTexture(const std::string& path);

} // namespace crank
