#include "Shader.h"

Shader& Shader::enable()
{
    glUseProgram(this->ID);
    return *this;
}

void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    unsigned int vertex, fragment, geometry;

    vertex   = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex, 1, &vertexSource, NULL);
    glShaderSource(fragment, 1, &fragmentSource, NULL);

    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    if(geometrySource != nullptr)
    {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometrySource, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);

    if(geometrySource != nullptr)
        glAttachShader(this->ID, geometry);

    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if(geometrySource != nullptr)
        glDeleteShader(geometry);
}

void Shader::checkCompileErrors(int object, std::string type)
{
    int success;
    char infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "ERROR::Shader: Compile-time error: Type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
}

void Shader::SetFloat (const char* name, float value, bool useShader)
{
    if (useShader)
        this->enable();

    glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetInteger (const char* name, int value, bool useShader)
{
    if (useShader)
        this->enable();

    glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetVector2f (const char* name, float x, float y, bool useShader)
{
    if (useShader)
        this->enable();

    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::SetVector2f (const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
        this->enable();

    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::SetVector3f (const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->enable();

    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::SetVector3f (const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
        this->enable();

    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void Shader::SetVector4f (const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->enable();

    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}

void Shader::SetVector4f (const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader)
        this->enable();

    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4  (const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
        this->enable();

    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}