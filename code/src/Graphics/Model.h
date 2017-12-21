#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <iostream>
#include <vector>

#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "Utils/Files.h"
#include "Utils/Log.h"

namespace crank
{

class Model
{
public:
	Model(const char* path);
	void Draw(const crank::Shader &shader);

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	crank::Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

private:
    std::vector<Texture> m_loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    std::vector<crank::Mesh> m_meshes;
	std::vector<Texture> m_texturesLoaded;
	std::string m_directory;
};

} // namespace crank
