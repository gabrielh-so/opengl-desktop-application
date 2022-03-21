#include "Include/Mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<Texture> textures) {

	Vertices = vertices;
	Indices = indicies;
	Textures = textures;

	SetupMesh();
}

void Mesh::SetupMesh() {

	// initialisation code (done once unless your object frequently changes)
	// create vertex buffer object ID (an array can hold multiple buffers)
	unsigned int VBO, VAO, EBO;

	// generate a vertex array and link it to VAO
	glGenVertexArrays(1, &VAO);

	// generate 1 buffer with the single unsigned int
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	// bind the type of the vertex buffer (an array buffer)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// so now any buffer calls we make on the glarraybuffer target wil be used to configure the currently bound buffer, which is vbo
	//Then we can make a call to the glBufferData function that copies the previously defined vertex data into the buffer's memory

	// glBufferData is a function specifically targeted to copy user - defined data into the currently bound buffer
	// Its first argument is the type of the buffer we want to copy data into: the vertex buffer object currently bound to the GL_ARRAY_BUFFER target.
	// second argument is the size of the data to pass through
	// the third is the data we want to send
	// the fourth determines the way the gpu should handle the data:

	/// <summary>
	/// GL_STREAM_DRAW: the data is set only onceand used by the GPU at most a few times.
	/// GL_STATIC_DRAW : the data is set only onceand used many times.
	/// GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
	/// </summary>
	/// <returns></returns>

	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	// Also, just like the VBO we want to place those calls between a bind and an unbind call, although this time we specify GL_ELEMENT_ARRAY_BUFFER as the buffer type.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	// now, the gpu knows how to process the data but opengl doesn't know how to interpret the vertex data in memory (we know it as three vec3 coordinates)
	// let's configure opengl to correctly interpret the vertex data

	// how the vertices data is formatted:

	// vertex 1 // vertex 2 // vertex 3 //
	// stride 12 bits (4 bytes per component, 3 components)
	// offset 0 (there's no extra data that is shoved in at the start that needs to be skipped)

	// the function for configurating the way vertex data is interpreted is glVertexAttribPointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// the first param is the vertex attribute we want to configure - in the shader source code, "layout (location = 0)"
	// this sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0
	// the second is the size of the vertex attribute - vertices contains vec3 position data, therefore size is 3
	// third argument specifies type of data - a vec* in glsl consists of floating point values, therefore GL_FLOAT
	// the fourth argument specifies if the data should be normalised. no, so GL_FALSE
	// the fifth is the stride, which tells us the difference between vertex attributes - 4 bytes per component, 3 components therefore stride is 12 bytes
	// the last parameter is the offset starting position. since data sharts in the very first location, this should be 0 (cast to generic void*). this has more use for more advanced concepts later

	// so now we've said how the vertex data should be interpreted, we can also enable the vertex attribute by giving glEnableVertexAttribArray the vertex attrubute location as it's arguement
	glEnableVertexAttribArray(0);


	// for the offset of each struct, we can use a preprocessor directive to get the integer offset of an element in the struct
	// (we can therefore avoid hardcoding the size of values for the index offsets)

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, Normal) );
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, TextureCoordinates) );
	glEnableVertexAttribArray(2);


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	
}

void Mesh::Draw(Shader &shader)
{

	
	unsigned int diffuseCount = 1;
	unsigned int specularCount = 1;

	for (unsigned int i = 0; i < Textures.size(); i++)
	{

		// activate the texture for this index
		// (enums are syntatic sugar for integer values heheheh)
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = Textures[i].Type;
		
		if (name == "texture_diffuse")
			number = std::to_string(diffuseCount++);

		else if (name == "texture_specular")
			number = std::to_string(specularCount++);

		// set the gl texture number to the correct uniform
		shader.SetFloatUniform(("material." + name + number).c_str(), i);

		// bind the texture id to the 
		glBindTexture(GL_TEXTURE_2D, Textures[i].ID);

	}

	// reset the texture bind to the expected value for future usage
	glActiveTexture(GL_TEXTURE0);

	// actually draw the mesh

	// bind the correct array
	glBindVertexArray(VAO);

	// draw the array
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	// reset the bound array to the expected value (NULL)
	glBindVertexArray(0);

}