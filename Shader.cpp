#include "Include/Shader.h"

Shader::Shader(const char* vertexShaderLocation, const char* fragmentShaderLocation) {
	// create input file streams and string streams
	std::ifstream vertexInputStream, fragmentInputStream;

	std::stringstream vertexStringStream, fragmentStringStream;

	// open the stream
	vertexInputStream.open(vertexShaderLocation);
	// read remaining stream buffer into string stream
	vertexStringStream << vertexInputStream.rdbuf();

	// close the stream after reading the buffer
	vertexInputStream.close();

	// same again for the fragment file
	fragmentInputStream.open(fragmentShaderLocation);
	fragmentStringStream << fragmentInputStream.rdbuf();

	// close the stream after reading the buffer
	fragmentInputStream.close();

	// get the string from the string stream and then read it as a const char*
	std::string vertexShaderSourceString = vertexStringStream.str();
	std::string fragmentShaderSourceString = fragmentStringStream.str();

	const char* vertexShaderSource = vertexShaderSourceString.c_str();
	const char* fragmentShaderSource = fragmentShaderSourceString.c_str();

	// values used for logging success
	int success;
	char infoLog[512];


	// create vertex shader, link and compile
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//A shader program object is the final linked version of multiple shaders combined.
	// To use the recently compiled shaders we have to link them to a shader program object and then activate this shader program when rendering objects.
	// The activated shader program's shaders will be used when we issue render calls.
	shaderProgramID = glCreateProgram();

	// attach and link shaders to program
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);

	// link program to opengl
	glLinkProgram(shaderProgramID);

	// get the status osmethgnwnjkrghbwseiktbkdhfehbl
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);

	// check for shader program linking success
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// and delete the shaders after linking them to the program as they are no longer needed

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::UseShader() {
	glUseProgram(shaderProgramID);
}

void Shader::SetBoolUniform(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}
void Shader::SetIntUniform(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}
void Shader::SetFloatUniform(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::SetFloatVec3Uniform(const std::string& name, float value1, float value2, float value3) const {
	int uniformLocation = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform3f(uniformLocation, value1, value2, value3);
}

void Shader::SetFloatVec3Uniform(const std::string& name, glm::vec3 value) const {
	int uniformLocation = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform3f(uniformLocation, value.x, value.y, value.z);
}

void Shader::SetFloatVec4Uniform(const std::string& name, float value1, float value2, float value3, float value4) const {
	int uniformLocation = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform4f(uniformLocation, value1, value2, value3, value4);
}

void Shader::SetFloatVec4Uniform(const std::string& name, glm::vec4 value) const {
	int uniformLocation = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);
}

void Shader::SetFloatMat3Uniform(const std::string& name, glm::mat3 matrix) const {

	unsigned int matrixUniformLocation = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniformMatrix3fv(matrixUniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetFloatMat4Uniform(const std::string& name, glm::mat4 matrix) const {

	unsigned int matrixUniformLocation = glGetUniformLocation(shaderProgramID, name.c_str());
	glUniformMatrix4fv(matrixUniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::DeleteShaderProgram() {
	glDeleteProgram(shaderProgramID);
}

Shader::~Shader() {
	DeleteShaderProgram();
}