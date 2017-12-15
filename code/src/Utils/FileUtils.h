#pragma once

#include <iostream>
#include <string>
#include <fstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "stb/stb_image.h"

std::string read_file(const std::string& path);
unsigned int load_texture(const std::string& path);
