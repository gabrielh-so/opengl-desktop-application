#include "Include/Model.h"

Model::Model(char* path)
{
	LoadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < Meshes.size(); i++)
	{
		Meshes[i].Draw(shader);
	}
}

void Model::LoadModel(std::string path)
{

	// create the importer
	Assimp::Importer importer;

	// call the omporter's readfile function which does somoething

	// useful options:
	// aiProcess_Triangulate means the importer should convert all primatives into triangles
	// since we're using gldrawtriangles this is the one we want

	// aiProcess_FlipUVs flips the texture coordineates on the y axis where nessesary when processing

	// aiProcess_GenNormals generates normals for each vertex if the model doesn't contain vector normals
	// if the model doesn't contain normals this is nessesary for use of the lighting shader

	// aiProcess_SplitLargeMeshes splits large meshes into smaller meshes if your rendering process has a
	// max no. of vertices allowed

	// aiProcess_OptimizeMeshes joins several meshes into one larger mesh to reduce drawing calls for optimisation

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);


	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	}

	// retrieves the directory path of the given file path
	directory = path.substr(0, path.find_last_of("/"));

	ProcessNode(scene->mRootNode, scene);

}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{

	// the node system provides a parent-child relationship between the meshes
	// while we don't technically need to preserve this relationship for this
	// simple implementation, you may want to edit this function to allow extra
	// control over the relationship between your different mesh objects

	// process all the node's meshes

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {

		// iterate through the list of mesh indexes stored in this node (node->mMeshes[i])
		// them push the corresponding scene mesh to the array

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		Meshes.push_back(ProcessMesh(mesh, scene));
	}


	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}

	// loop through child nodes for them to add to meshes
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// we need to grab select data from the corresponding locations in the scene
	// to collect under a single mesh class

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// define the vertex used to store the information
		Vertex vertex;
		
		// position and normals are straight forward
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;


		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;

		// for texture coordinates, this is different
		// assimp allows up to 8 texture coordinates per vertex
		// but at the moment we only care about 1

		// we should check that the 1st texture coordinate exists before adding it though

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 textureCoordinates;
			textureCoordinates.x = mesh->mTextureCoords[0][i].x;
			textureCoordinates.y = mesh->mTextureCoords[0][i].y;

			vertex.TextureCoordinates = textureCoordinates;
		}
		else
			vertex.TextureCoordinates = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// now we have the vertices and indices of the mesh, which is enough to draw it
	// however we still need to load in textures

	// valid texture indexes will have indexable values, so check if the material index
	// of the mesh is greater than or equal to zero
	if (mesh->mMaterialIndex >= 0)
	{
		// get the correct material index from the scene's collection
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// get a vector of extracted diffuse and specular textures (seperately), and insert them into the mesh's textures vector
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}


	// like nodes that index meshes, meshes only store indexes to textures that are
	// contained within the aiScene object. therefore we need to fetch these indices
	// and load the corresponding texture for each one


	return Mesh(vertices, indices, textures);

}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
{

	// the vector that stores all the textures
	std::vector<Texture> textures;

	// loop over each texture of the material type to add it in
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{

		aiString str;
		material->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < LoadedTextures.size(); j++)
		{
			if (std::strcmp(LoadedTextures[j].Path.data(), str.C_Str()) == 0)
			{
				textures.push_back(LoadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;

			// TextureFromFile will fetch the texture id given to the program by opengl to
			texture.ID = LoadTextureFromFile(str.C_Str(), directory, false);
			texture.Type = typeName;
			texture.Path = str.C_Str();
			textures.push_back(texture);
			LoadedTextures.push_back(texture); // add to loaded textures
		}
	}

	return textures;
}

unsigned int Model::LoadTextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = directory + "/" + std::string(path);

	// let's create a texture - we use an id a lot like buffers and arrays
	unsigned int textureID;
	glGenTextures(1, &textureID);

	// now bind the texture so subsequent texture commands are directed at this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	stbi_set_flip_vertically_on_load(true);

	int textureWidth, textureHeight, textureNrChannels;
	unsigned char* data = stbi_load(filename.c_str(), &textureWidth, &textureHeight, &textureNrChannels, 0);

	if (data)
	{

		GLenum format;

		switch (textureNrChannels)
		{
		case 1:
			format = GL_RED;
			break;

		case 2:
			format = GL_RGB;
			break;

		case 3:
			format = GL_RGBA;
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture | path: " << directory << path << std::endl;
	}

	// now free the image data
	stbi_image_free(data);

	return textureID;
}