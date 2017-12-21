#include "Model.h"

namespace crank
{

Model::Model(const char* path)
{
    LoadModel(path);
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if(!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		LOGEL << "[MODEL]:";
		LOGE << importer.GetErrorString();
		return;
	}
	
	// retrieve the m_directory path of the filepath
	m_directory = path.substr(0, path.find_last_of('/'));
	
	// process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
	// process all the node's m_meshes (if any)
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains Indices to index the actual objects in the scene.
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}

	// then do the same for each of its children
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
        ProcessNode(node->mChildren[i], scene);
	}
}

crank::Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	// data to fill
	std::vector<Vertex> vertices;
	vertices.reserve(256);
	
	std::vector<unsigned int> indices;
	indices.reserve(1024);

	std::vector<Texture> textures;
	textures.reserve(64);

	// Walk through each of the mesh's Vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		
		// vertex position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = std::move(vector);

		// vertex normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = std::move(vector);

		// vertex texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = std::move(vec);
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		// tangent
		if(mesh->mTangents != nullptr) // if there are vertex tangents, read them
		{							   // i explicitly asked to calculate tangents when importing so there will be
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = std::move(vector);
		}
		
		// bitangent
		if(mesh->mBitangents != nullptr) // if there are vertex bitangents, read them
		{
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = std::move(vector);
		}

		vertices.push_back(vertex);
	}
	
	// for every face (faces are the same as triangles)
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// for every triangle
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0) // if the mesh actually contains a material
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		std::vector<Texture> diffuseMaps = m_loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		std::vector<Texture> specularMaps = m_loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		std::vector<Texture> normalMaps = m_loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		std::vector<Texture> heightMaps = m_loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return crank::Mesh(vertices, indices, textures);
}

//checks all material textures of a given type and loads the textures if they're not loaded yet
std::vector<Texture> Model::m_loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString filename;
		mat->GetTexture(type, i, &filename); // getting file name
		
		bool skip = false;
		for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
		{
			if(std::strcmp(m_texturesLoaded[j].path.C_Str(), filename.C_Str()) == 0)
			{
				textures.push_back(m_texturesLoaded[j]);
				skip = true;
				break;
			}
		}

		if(!skip) // if texture hasn't been loaded already, load it
		{
			Texture texture;

			// Choose accordingly
			texture.id = crank::loadTexture(m_directory + '/' + std::string(filename.C_Str()));
			//texture.id = crank::loadTexture(m_directory + std::string(filename.C_Str()));

			texture.type = typeName;
			texture.path = filename;
			textures.push_back(texture);
			m_texturesLoaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

void Model::Draw(const crank::Shader &shader)
{
	for (int i = 0; i < m_meshes.size(); i++)
        m_meshes[i].Draw(shader);
}

}
