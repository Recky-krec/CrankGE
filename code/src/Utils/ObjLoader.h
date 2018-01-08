#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "GL/glew.h"

#include "Graphics/Shader.h"
#include "Graphics/Mesh.h"
#include "Utils/Log.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace crank
{

class ObjLoader
{
public:
    ObjLoader(const std::string& path);
    void Draw(const crank::Shader &shader);

private:
    void OnConstruction(const std::string& path);
    void LoadBuffers(const std::string& path);
    void SortBuffers();
    void SetupMesh();

private:
    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_texCoords;
    std::vector<std::string> m_faceInfo;

    std::vector<Vertex> m_vertices;

private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;

};

} // namespace crank
