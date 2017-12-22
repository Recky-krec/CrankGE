#pragma once

#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include "Utils/Log.h"
#include <vector>
#include <cmath>

#define GLEW_STATIC
#include "GL/glew.h"
#include "stb/stb_image.h"
namespace crank
{
std::string readFile(const std::string& path);
void copyFileByte(const std::string& src, const std::string& destination);
void copyFileBlock(const std::string& src, const std::string& destination);
template<typename T>
char* asBytes(T& var);

unsigned int loadTexture(const std::string& path);




} // namespace crank
