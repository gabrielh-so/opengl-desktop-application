#pragma once

#include <vector>
#include <string>
#include "Shader.h"
#include "Mesh.h"

#include <unordered_map>

// library for loading in models
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "stb_image.h"

class Model {

public:
	Model(char* path);
	void Draw(Shader& shader);


private:
	std::vector<Mesh> Meshes;
	std::string directory;

	std::vector<Texture> LoadedTextures;

	void LoadModel(std::string path);

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);

	unsigned int LoadTextureFromFile(const char* path, const std::string& directory, bool gamma);

};