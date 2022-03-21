#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "glad/glad.h"
#include "glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {


	// 1. get the vertex and| fragment shader source code for compilation
	// create vertexshader  and then attach the source string
	// compile the shader
	// log success

	// do the same for fragment shader

	// create shaderprogram and attach shaders
	// link the shader program to opengl

	// delete shaders

	// when instructed or fall out of scope, delete the program



	// public member variables
public:

	unsigned int shaderProgramID;

	Shader(const char* vertexShaderLocation, const char* fragmentShaderLocation);

	// private member variables and methods
private:


	// public methods
public:

	// use/activate the shader
	void UseShader();

	// utility uniform functions
	void SetBoolUniform(const std::string& name, bool value) const;
	void SetIntUniform(const std::string& name, int value) const;
	void SetFloatUniform(const std::string& name, float value) const;

	void SetFloatVec3Uniform(const std::string& name, float value1, float value2, float value3) const;
	void SetFloatVec3Uniform(const std::string& name, glm::vec3 value) const;

	void SetFloatVec4Uniform(const std::string& name, float value1, float value2, float value3, float value4) const;
	void SetFloatVec4Uniform(const std::string& name, glm::vec4 value) const;

	void SetFloatMat3Uniform(const std::string& name, glm::mat3 matrix) const;

	void SetFloatMat4Uniform(const std::string& name, glm::mat4 matrix) const;

	void DeleteShaderProgram();

	~Shader();

};