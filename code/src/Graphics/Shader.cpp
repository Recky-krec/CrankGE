#include "Shader.h"

namespace crank
{

Shader& Shader::Enable()
{
    glUseProgram(this->Id);
    return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    unsigned int vertex, fragment, geometry;

    vertex   = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex, 1, &vertexSource, NULL);
    glShaderSource(fragment, 1, &fragmentSource, NULL);

    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    if(geometrySource != nullptr)
    {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometrySource, NULL);
        glCompileShader(geometry);
        CheckCompileErrors(geometry, "GEOMETRY");
    }

    this->Id = glCreateProgram();
    glAttachShader(this->Id, vertex);
    glAttachShader(this->Id, fragment);

    if(geometrySource != nullptr)
        glAttachShader(this->Id, geometry);

    glLinkProgram(this->Id);
    CheckCompileErrors(this->Id, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if(geometrySource != nullptr)
        glDeleteShader(geometry);
}

void Shader::CheckCompileErrors(int object, std::string type)
{
    int success;
    char infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            LOGEL << "[SHADER]:";
            LOGE << "Compile-time error: Type: " << type;
            LOGE << infoLog;
            LOGE << "-- --------------------------------------------------- -- ";
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);

            LOGEL << "[SHADER]:";
            LOGE << "Link-time error: Type: " << type;
            LOGE << infoLog;
            LOGE << "-- --------------------------------------------------- -- ";

        }
    }
}

void Shader::SetFloat (const char* name, float value, bool useShader)
{
    if (useShader)
        this->Enable();

    glUniform1f(glGetUniformLocation(this->Id, name), value);
}

void Shader::SetInteger (const char* name, int value, bool useShader)
{
    if (useShader)
        this->Enable();

    glUniform1i(glGetUniformLocation(this->Id, name), value);
}

void Shader::SetVector2f (const char* name, float x, float y, bool useShader)
{
    if (useShader)
        this->Enable();

    glUniform2f(glGetUniformLocation(this->Id, name), x, y);
}

void Shader::SetVector2f (const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
        this->Enable();

    glUniform2f(glGetUniformLocation(this->Id, name), value.x, value.y);
}

void Shader::SetVector3f (const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->Enable();

    glUniform3f(glGetUniformLocation(this->Id, name), x, y, z);
}

void Shader::SetVector3f (const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
        this->Enable();

    glUniform3f(glGetUniformLocation(this->Id, name), value.x, value.y, value.z);
}

void Shader::SetVector4f (const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->Enable();

    glUniform4f(glGetUniformLocation(this->Id, name), x, y, z, w);
}

void Shader::SetVector4f (const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader)
        this->Enable();

    glUniform4f(glGetUniformLocation(this->Id, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4  (const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
        this->Enable();

    glUniformMatrix4fv(glGetUniformLocation(this->Id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

} // namespace crank
