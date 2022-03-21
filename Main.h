#pragma once
// provides functions for opengl
#include "glad/glad.h"
#include "glfw3.h"
#include <iostream>
#include <string>

// source code for loading in shaders
#include "Include/Shader.h"


// source code for light types
#include "Include/PointLight.h"
#include "Include/DirectionLight.h"
#include "Include/Spotlight.h"

// camera class
#include "Include/Camera.h"

// library for loading in images
#include "Include/stb_image.h"

// library for loading in models
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

// matrix operation library
#include "Include/glm/glm.hpp"
#include "Include/glm/gtc/matrix_transform.hpp"
#include "Include/glm/gtc/type_ptr.hpp"

#include "Include/Game.h"

// the resize callback function has the following prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// prototype for ProcessInput
void ProcessInput(GLFWwindow* window);

void mouseCallback(GLFWwindow* window, double xPos, double yPos);

/*

// in -> takes in a value from the previous step (could be reading it in as a vertex attribute, or being passed up from further back in the graphics pipeline)
// out -> gives a useful output for colour or maybe passes it up the graphics pipeline


static const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	vertexColor = vec4(1.0f, aPos.x, 0.0f, 1.0f);"
"}\0";

/*
static const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";
*/

// for red to orangey fragment colors
/*
static const char* fragmentShaderSource = "#version 330 core\n"
"in vec4 vertexColor;"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vertexColor;\n"
"}\0";
*?/

// uniform fragment coloring
static const char* fragmentShaderSource = "#version 330 core\n"
"uniform vec4 vertexColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vertexColor;\n"
"}\0";

static const char* vertexShaderSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 thisColour;\n"
"out vec4 vertexColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	vertexColor = vec4(thisColour, 1.0f);"
"}\0";

// yellow shader
/*
static const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";
*?/

// wonderful multicoloured shader
static const char* fragmentShaderSource2 = "#version 330 core\n"
"in vec4 vertexColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vertexColor;\n"
"}\0";

*/