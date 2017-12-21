#pragma once

#include <iostream>
#include <string>

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Utils/Log.h"

namespace crank
{

class Shader
{
public:
    unsigned int Id;

public:
    Shader() = default;
    void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
    Shader& Enable();

    void SetFloat    (const char* name, float value, bool useShader = false);
    void SetInteger  (const char* name, int value, bool useShader = false);
    void SetVector2f (const char* name, float x, float y, bool useShader = false);
    void SetVector2f (const char* name, const glm::vec2& value, bool useShader = false);
    void SetVector3f (const char* name, float x, float y, float z, bool useShader = false);
    void SetVector3f (const char* name, const glm::vec3& value, bool useShader = false);
    void SetVector4f (const char* name, float x, float y, float z, float w, bool useShader = false);
    void SetVector4f (const char* name, const glm::vec4& value, bool useShader = false);
    void SetMatrix4  (const char* name, const glm::mat4& matrix, bool useShader = false);

private:
    void CheckCompileErrors(int object, std::string type);
};

} // namespace crank