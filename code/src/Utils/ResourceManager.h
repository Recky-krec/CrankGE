#pragma once

#include <map>
#include <string>
#include <fstream>

#define GLEW_STATIC
#include "GL/glew.h"

#include "Graphics/Shader.h"
//#include "Graphics/Texture2D.h"
#include "Utils/FileUtils.h"

class ResourceManager
{
public:
    ResourceManager() = delete;

public:
    static std::map<std::string, Shader> Shaders;
    //static std::map<std::string, Texture2D> Textures;

public:
    static Shader LoadShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, std::string name);
    static Shader GetShader(std::string name);
    //static Texture2D LoadTexture(const char* file, std::string name);
    //static Texture2D GetTexture(std::string name);

    static void Clear();

private:
    static Shader loadShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    //static Texture2D loadTextureFromFile(const char* file);
};
