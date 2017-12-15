#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <iostream>
#include <vector>

#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "Utils/FileUtils.h"

class Model
{
public:
	Model(const char* path);
	void draw(const Shader& shader);

private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;
};

