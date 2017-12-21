#include "ResourceManager.h"
#include "stb/stb_image.h"

namespace crank
{

// Instantiate static variables
std::map<std::string, crank::Shader> ResourceManager::Shaders;
//std::map<std::string, Texture2D> ResourceManager::Textures;


crank::Shader ResourceManager::LoadShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, std::string name)
{
    Shaders[name] = loadShaderFromFile(vertexPath, fragmentPath, geometryPath);
    return Shaders[name];
}

crank::Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

/*Texture2D ResourceManager::LoadTexture(const char* file, std::string name)
{
    Textures[name] = loadTextureFromFile(file);
    return Textures[name];
}

Texture2D ResourceManager::loadTextureFromFile(const char* file)
{
    if(!file) std::cout << "passed nullptr at ResourceManager::LoadTexture" << std::endl;
    Texture2D result;

    // Load image
    int width, height, nrComponents;
    unsigned char* data = stbi_load(file, &width, &height, &nrComponents, 0);

    if(nrComponents == 1) {
        result.internal_format = GL_RED;
        result.image_format    = GL_RED;
    } else if(nrComponents == 3) {
        result.internal_format = GL_RGB;
        result.image_format    = GL_RGB;
    } else if (nrComponents == 4) {
        result.internal_format = GL_RGBA;
        result.image_format    = GL_RGBA;
    }

    result.generate(width, height, data);
    stbi_image_free(data);
    return result;
}


Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}*/


crank::Shader ResourceManager::loadShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    std::string vertexCode, fragmentCode, geometryCode;
    try
    {
        vertexCode = crank::readFile(vertexPath);
        fragmentCode = crank::readFile(fragmentPath);

        if (geometryPath != nullptr)
            geometryCode = crank::readFile(geometryPath);
    }
    catch (std::exception& e)
    {
        //crank::Log::ReportingLevel(crank::Log::ERROR);
        LOGE << "[ResourceManager]: Failed to read shader files" << e.what() <<  std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    // 2. Now create shader object from source code
    crank::Shader shader;
    shader.Compile(vShaderCode, fShaderCode, geometryPath != nullptr ? gShaderCode : nullptr);
    return shader;
}

void ResourceManager::Clear()
{
    for(auto it : Shaders)
        glDeleteShader(it.second.Id);

    /*for(auto it : Textures)
        glDeleteTextures(1, &it.second.Id);*/
}

} // namespace crank