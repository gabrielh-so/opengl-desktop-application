#pragma once

#include <string>
#include <vector>
#include "Include/Shader.h"
#include "Include/glm/glm.hpp"

// library for loading in models
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextureCoordinates;
};

struct Texture {
	unsigned int ID;
	std::string Type;
	std::string Path;
};

class Mesh {

public:

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture> Textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<Texture> textures);

	void Draw(Shader &shader);

private:

	unsigned int VBO, VAO, EBO;

	void SetupMesh();

};