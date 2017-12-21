#pragma once

#include "GL/glew.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "assimp/types.h"

#include "Shader.h"

#include <string>
#include <vector>

namespace crank
{

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
	void Draw(const crank::Shader &shader);

public:
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture> Textures;

private:
	unsigned int m_vao, m_vbo, m_ebo;
	void SetupMesh();
};

} // namespace crank
