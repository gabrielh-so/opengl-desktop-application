#include "Include/Main.h"

static float deltaTime = 0;
static float mixAmount = 0;
static float camX = 0.0f;
static float camY = 0.0f;
static float camZ = 5.0f;
static float speed = 3.0f;

static float pitch = 0;
static float yaw = -90.0f;

static float windowWidth, windowHeight;

float lastPosX, lastPosY;

Camera myCamera = Camera::Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
	return;
}

void ProcessInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);



	float forwardComponent = 0.0f;
	float rightComponent = 0.0f;
	float upComponent = 0.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

		forwardComponent += 1;

		/*
		camX += speed * direction.x * deltaTime;
		camZ += speed * direction.z * deltaTime;
		*/
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

		forwardComponent -= 1;

		/*
		camX -= speed * direction.x * deltaTime;
		camZ -= speed * direction.z * deltaTime;
		*/
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

		rightComponent += 1.0f;

		/*
		camX += speed * rotatedDirection.x * deltaTime;
		camZ += speed * rotatedDirection.z * deltaTime;
		*/
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

		rightComponent -= 1.0f;

		/*
		camX -= speed * rotatedDirection.z * deltaTime;
		camZ -= speed * rotatedDirection.x * deltaTime;
		*/
	}


	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {

		upComponent += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {

		upComponent -= 1.0f;
	}

	myCamera.ProcessKeyboardMovement(forwardComponent, rightComponent, upComponent, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		mixAmount += deltaTime;
		mixAmount = fmin(mixAmount, 1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		mixAmount -= deltaTime;
		mixAmount = fmax(mixAmount, 0.0f);
	}


	return;
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos) {

	if (firstMouse) {
		lastPosX = xPos;
		lastPosY = yPos;
		firstMouse = false;
	}

	float xoffset = xPos - lastPosX;
	float yoffset = lastPosY - yPos;
	
	float sensitivity = 0.1f;
	
	myCamera.ProcessMouseMovement(xoffset, yoffset, deltaTime, true);

	lastPosX = xPos;
	lastPosY = yPos;

	//glfwSetCursorPos(window, 400, 300);

	return;
}


void AssignLightUniforms() {

}



unsigned int LoadTexture(char const* path) {
	// load in the texture materials

	// let's create a texture - we use an id a lot like buffers and arrays
	unsigned int newTextureID;
	glGenTextures(1, &newTextureID);

	// now bind the texture so subsequent texture commands are directed at this texture
	glBindTexture(GL_TEXTURE_2D, newTextureID);

	// when texture coordinates are specified outside of the range, we can use some options to specify how opengl should behave

	/*
	GL_REPEAT: The default behavior for textures.Repeats the texture image.
	GL_MIRRORED_REPEAT : Same as GL_REPEAT but mirrors the image with each repeat. // (probably looks the best because there won't be any hard line)
	GL_CLAMP_TO_EDGE : Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
	GL_CLAMP_TO_BORDER : Coordinates outside the range are now given a user - specified border color.
	*/

	// Each of the aforementioned options can be set per coordinate axis
	// (s, t (and r if you're using 3D textures) equivalent to x,y,z) with the glTexParameter* function:

	// set the options for 2d textures (first param) // we do 2 function calls for each coordinate of the texture (second parameter), and set the actual option (third parameter)
	// we want to configure this for the s and  t axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// if we want clamp to border, then we should specify the border colour
	// float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// texture filtering

	// open gl needs to know which texture pixel to map the coordinate to - usually this is either a linear (a linear mixture of floats of surrounding pixels)
	// or the closest texel to the texture coordinate (default)
	// GL_NEAREST and GL_LINEAR are the two most important options
	// GL_NEAREST makes individual pixels stand out more because of harsher contrasts with surrounding pixels
	// GL_LINEAR is pretty much the opposite

	// texture filtering can be set for magnifying and minifying operations
	// this means you could use nearest neighbour for minimising operations and linear filtering for upscale textures
	// we specify the options for each coordinate with the glTexParameter* function
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// in a large room with thousand of objects, some very far away, there are going to be objects whose fragment covers a large part of the texture
	// for linear filtering there's going to be a lot of surplus bandwidth and processing for finding the average float value for a very small object
	// however, we can use mipmaps which is a collection of textures with smaller and smaller resolutions that are used for fragments at further and further distances from the camera
	// making mipmaps are cumbersome, but we can use opengl to simplify the process by letting it do it for us!!!

	// we can also filter between mipmap levels to prevent sharp edges when switching between mipmap levels
	/*
	GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
	GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap level and samples that level using linear interpolation.
	GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples the interpolated level via nearest neighbor interpolation.
	GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation.
	*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// A common mistake is to set one of the mipmap filtering options as the magnification filter.This doesn't have any effect since mipmaps are primarily used for when textures get downscaled

	// flips the image vertically before loading because the texture is going to be loaded in upside down otherwise
	stbi_set_flip_vertically_on_load(true);

	// now, load in the texture!
	// let#s use an image loading library
	int newTextureWidth, newTextureTextureHeight, newTextureNrChannels;
	unsigned char* data = stbi_load(path, &newTextureWidth, &newTextureTextureHeight, &newTextureNrChannels, 0);


	if (data)
	{
		// the data has been loaded in correctly!
		// we can now generate the texture using the image data
		// this is done using glTexImage2D
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newTextureWidth, newTextureTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// 1. the sort of texture that we're targeting
		// 2. the mipmap texture level we want to create the texture for (if you want to generate mipmaps manually) - base is 0
		// 3. what sort of format we want to store the data in (we're storing in RGB form)
		// 4. width of the image (given to us by the loading library)
		// 5. height of the image
		// 6. legacy arg that should always be 0
		// 7. the format of the source image (RGB)
		// 8. the datatype of the source image (unsigned byte)
		// 9. finally, the actual image data
		// great!

		// now, automatically generate the mipmap of the targeted texture (specifically a 2d texture so any other texture type will not be targeted)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// now free the image data
	stbi_image_free(data);

	return newTextureID;
}

int main() {
	
	// In the main function we first initialize GLFW with glfwInit, after which we can configure GLFW using glfwWindowHint.
	// the configurations are pretty self-explainatory

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// create and instance of a GLFWWindow, and intialise it with the correct dimensions
	GLFWwindow* window = glfwCreateWindow(800, 600, "box2dDemo", NULL, NULL);
	
	// check that the window creation is successful
	if (window == NULL) {
		std::cout << "failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// switch the context to the window
	glfwMakeContextCurrent(window);

	// initialise glad, which manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function: 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// mouse input initialisation - tell glfw to capture the cursor so we can move it around
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// now the cursor won't be visible and it won't read the window

	/*
	lastPosX = windowWidth / 2.0f;
	lastPosY = windowHeight / 2.0f;
	*/
	lastPosX = 400;
	lastPosY = 300;

	// now the callback is set so that the callback runs whenever the cursor is moved
	glfwSetCursorPosCallback(window, mouseCallback);


	// GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.
	
	/*
	// get the maximum number of vertex attributes we can use. 16 is plenty
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	*/

	






	Shader simplexShader("Shaders/simplexNoise.vs", "Shaders/simplexNoise.fs");

	Shader lightingShader("Shaders/multiplelightingshader.vs", "Shaders/multiplelightingshader.fs");
	Shader lightCubeShader("Shaders/lightCubeShader.vs", "Shaders/lightCubeShader.fs");

	// tell opengl the size of the rendering window
	//glViewport(0, 0, 800, 600);

	// Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D coordinates it processed to coordinates on your screen.
	// For example, a processed point of location (-0.5,0.5) would (as its final transformation) be mapped to (200,450) in screen coordinates.

	// However, the moment a user resizes the window the viewport should be adjusted as well.
	// We can register a callback function on the window that gets called each time the window is resized.
	// the callback function is defined further up in the file

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// There are many callbacks functions we can set to register our own functions.
	// For example, we can make a callback function to process joystick input changes,
	// process error messages etc.We register the callback functions after we've created
	// the window and before the render loop is initiated.

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f),
		glm::vec3(-2.0f,  -5.0f, 15.0f),
		glm::vec3(1.5f, 2.2f, 2.5f),
		glm::vec3(3.8f, 2.0f, 12.3f),
		glm::vec3(-2.4f, 0.4f, 3.5f),
		glm::vec3(1.7f,  -3.0f, 7.5f),
		glm::vec3(-1.3f, 2.0f, 2.5f),
		glm::vec3(-1.5f,  -2.0f, 2.5f),
		glm::vec3(-1.5f,  -0.2f, 1.5f),
		glm::vec3(1.3f,  -1.0f, 1.5f)
	};

	float simplexPlaneVerticies[] =
	{
		// positions

		 0.5f,  0.5f, 0.5f,
		 0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		 0.5f,  0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f
	};
	
	float cubeVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	


	/*
	float furnaceVertices[] = {
		 0.5f,  0.5f, 0.0f,
		 1.0f, 0.0f, 0.0f,
		 2.0f, 2.0f,         // top right texture

		 0.5f, -0.5f, 0.0f,
		 0.0f, 1.0f, 0.0f,
		 2.0f, 0.0f,

		-0.5f, -0.5f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f,

		-0.5f,  0.5f, 0.0f,   // top left
		 0.0f,  1.0f, 0.0f,
		 0.0f,  2.0f

	};
	*/
	
	
	/*
	float furnaceVertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,
		 1.0f, 0.0f, 0.0f,
		 1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,
		 0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f    // top left 
	};
	*/
	
	// specify the indexes of the vertexes that form each triangle

	// create Element Array Buffer - stores indices that opengl uses to decide what to draw
	// With the vertex data defined we'd like to send it as input to the first process of the graphics pipeline: the vertex shader.
	// however, every time we need to draw an object we'll have to repeat this process.
	// these functions are not scalable, so we need a way to store all the state configurations and simply bind this object to restore the state

	// to create a vertex array object the process is similar to the start of the other object

	// initialisation code (done once unless your object frequently changes)
	// create vertex buffer object ID (an array can hold multiple buffers)
	unsigned int VBO, lightVAO;

	// generate a vertex array and link it to VAO
	glGenVertexArrays(1, &lightVAO);

	// generate 1 buffer with the single unsigned int
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(lightVAO);

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

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// now, the gpu knows how to process the data but opengl doesn't know how to interpret the vertex data in memory (we know it as three vec3 coordinates)
	// let's configure opengl to correctly interpret the vertex data

	// how the vertices data is formatted:
	
	// vertex 1 // vertex 2 // vertex 3 //
	// stride 12 bits (4 bytes per component, 3 components)
	// offset 0 (there's no extra data that is shoved in at the start that needs to be skipped)

	// the function for configurating the way vertex data is interpreted is glVertexAttribPointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// and the drawing code in the render loop


	// allows opengl to compare the z coordinate of fragments and then replaces them as required
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);


	////////////////////////////
	// SIMPLEX NOISE FUNCTION //
	////////////////////////////

	unsigned int planeVBO, planeVAO;

	glGenVertexArrays(1, &planeVAO);

	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);


	glBufferData(GL_ARRAY_BUFFER, sizeof(simplexPlaneVerticies), simplexPlaneVerticies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	/////////
	// end //
	/////////



	// load in the textures and
	unsigned int container1Diffuse = LoadTexture("Textures/container.png");
	unsigned int container1Specular = LoadTexture("Textures/container_specular.png");
	unsigned int container1Emission = LoadTexture("Textures/matrix.jpg");


	lightingShader.UseShader();
	lightingShader.SetIntUniform("material.diffuse", 0);
	lightingShader.SetIntUniform("material.specular", 1);
	lightingShader.SetIntUniform("material.emission", 2);






	glm::mat4 perspective;
	perspective = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	// this is where you should reverse to

	//	LIGHT SOURCES - 4 POINT LIGHTS, A DIRECTIONAL LIGHT POINTING STRAIGHT UPWARD AND A SPOTLIGHT SHINING OUT FROM THE POSITION OF THE PLAYER

	// this section defines the position, colour and direction/rotation of lights to be stored within the lighting class. the shader is later passed through these classes
	// to pass the stored information to the shaders to be processed in the fragment shader

	DirectionLight directionLight;
	directionLight.position = glm::vec3(0.0f, -1.0f, 0.0f);
	directionLight.ambient = glm::vec3(0.0f, 0.0f, 0.2f);
	directionLight.diffuse = glm::vec3(0.0f, 0.0f, 0.5f);
	directionLight.specular = glm::vec3(0.0f, 0.0f, 1.0f);

	Spotlight spotlight;
	spotlight.cutoff = glm::cos(glm::radians((35.0f)));
	spotlight.outerCutoff = glm::cos(glm::radians((45.0f)));
	spotlight.ambient = glm::vec3(0.0f, 0.2f, 0.0f);
	spotlight.diffuse = glm::vec3(0.0f, 0.5f, 0.0f);
	spotlight.specular = glm::vec3(0.0f, 1.0f, 0.0f);
	spotlight.constant = 1.0f;
	spotlight.linear = 0.09f;
	spotlight.quadratic = 0.035f;

	PointLight pointLights[4];

	pointLights[0].position = glm::vec3(0.7f, 0.2f, 2.0f);
	pointLights[0].ambient = glm::vec3(0.2f, 0.0f, 0.0f);
	pointLights[0].diffuse = glm::vec3(0.5f, 0.0f, 0.0f);
	pointLights[0].specular = glm::vec3(1.0f, 0.0f, 0.0f);
	pointLights[0].constant = 1.0f;
	pointLights[0].linear = 0.09f;
	pointLights[0].quadratic = 0.035f;

	pointLights[1].position = glm::vec3(2.3f, -3.3f, -4.0f);
	pointLights[1].ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	pointLights[1].diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	pointLights[1].specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLights[1].constant = 1.0f;
	pointLights[1].linear = 0.09f;
	pointLights[1].quadratic = 0.035f;

	pointLights[2].position = glm::vec3(-4.0f, 2.0f, -12.0f);
	pointLights[2].ambient = glm::vec3(0.2f, 0.2f, 0.0f);
	pointLights[2].diffuse = glm::vec3(0.5f, 0.5f, 0.0f);
	pointLights[2].specular = glm::vec3(1.0f, 1.0f, 0.0f);
	pointLights[2].constant = 1.0f;
	pointLights[2].linear = 0.09f;
	pointLights[2].quadratic = 0.035f;

	pointLights[3].position = glm::vec3(0.0f, 0.0f, -3.0f);
	pointLights[3].ambient = glm::vec3(0.0f, 0.2f, 0.2f);
	pointLights[3].diffuse = glm::vec3(0.0f, 0.5f, 0.5f);
	pointLights[3].specular = glm::vec3(0.0f, 1.0f, 1.0f);
	pointLights[3].constant = 1.0f;
	pointLights[3].linear = 0.09f;
	pointLights[3].quadratic = 0.035f;


	///////////////////////////////
	//      the render loop      //
	///////////////////////////////

	// draw in polygon/wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float lastTime = 0;

	// glfwWindowShouldClose polls if glfw has been instructed to close
	while (!glfwWindowShouldClose(window)) {

		float newTime = glfwGetTime();
		deltaTime = newTime - lastTime;
		lastTime = newTime;

		glfwSetWindowTitle(window, ((std::string)"FPS: " + std::to_string(1.0f / deltaTime)).c_str());

		// call to ProcessInput, exits on esc key press
		ProcessInput(window);



		// rotate the camera to the right
		/*
		yaw += deltaTime * 30.0f;
		if (yaw > 180) yaw -= 360;
		*/

		//////////////////////
		// camera movements //
		//////////////////////

		// yaw = 0 -> looking parallel to x vector (perp to y)
		// pitch = 0 -> looking perpendicular to y unit vector


		// rendering commands here
		// 
		// At the start of frame we want to clear the screen.
		// We can clear the screen's color buffer using glClear where we pass in buffer bits to specify which buffer we would like to clear.
		// The possible bits we can set are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT

		// the glClearColor function is a state-setting function and glClear is a state-using function in that it uses the current state to retrieve the clearing color from. 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);


		///////////////////////////////
		//      3d  projections      //
		///////////////////////////////

		/*
		to transform the local coordinates of an object you're going to need to use different sorts of matrices in order to transform between different coordinate spaces
		local space - the coordinate space used by the model that the object coordinates use - (0,0,0) is usually the origin
		this is transformed by the model matrix, which produces the coordinates in world space
		then, we transform the coordinates in the world space by the view matrix, which puts it in view space
		then you do a projection matrix which procuces it in clip space
		then opengl does some stuff to it like making sure that it's outside the clipping area and then it's put on the screen

		*/


		/*
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(5.0f - glfwGetTime(), 0.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		*/
		
		glm::mat4 model;

		
		//model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));

		

		// create the view matrix
		glm::mat4 view;
		view = myCamera.GetViewMatrix();

		// when the model matrix is scaled, the normal will be scaled too, but the scaled normal will not be perpendicular to the face.
		// therefore, do the inverse transposition of the normal to find the true normal for the scaled matrix
		// we do this on the cpu because transpositions are costly for the gpu every time the shaders are run
		// also in view space so need to transform by view matrix too
		glm::mat3 normalMatrix;

		/*
		glBindVertexArray(VAO);
		// insert call to function that draws the triangle here!
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		// second argument is the number of items in the elements list (3 vertices * 2 trangles)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		*/

		/*
		glUseProgram(shaderProgram2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		*/

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		/*
		shader2.UseShader();

		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		*/


		// now bind the texture to assign the texture to the fragment shader sampler automatically
		//glBindTexture(GL_TEXTURE_2D, furnace1Texture);

		spotlight.direction = myCamera.Front;
		spotlight.position = myCamera.Position;

		lightingShader.UseShader();

		// set values for the material struct in the fragment shader
	
		// set the diffuse shader to the first texture (the diffuse texture)
		//lightingShader.SetFloatVec3Uniform("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.SetFloatUniform("material.shininess", 32);

		
		directionLight.AssignShaderValues("directionLight.", lightingShader);
		
		for (int i = 0; i < 4; i++)
		{
			pointLights[i].AssignShaderValues("pointLights[" + std::to_string(i) + "].", lightingShader);
		}
		
		spotlight.AssignShaderValues("spotlight.", lightingShader);


		/*
		NOTE: IN ORDER TO HAVE THE DIRECTIONAL LIGHTING BE AN ABSOLUTE VECTOR AND TO NOT HAVE MY BRAIN EXPLODE, I WILL WORK WITH LIGHTING IN WORLD SPACE FROM HERE ON OUT
		I THINK
		*/
		lightingShader.SetFloatVec3Uniform("viewPosition", myCamera.Position);

		// don't need to pass in view position since in view space view position is always origin
		//lightingShader.SetFloatVec3Uniform("viewPosition", myCamera.Position);

		lightingShader.SetFloatMat4Uniform("view", view);
		lightingShader.SetFloatMat4Uniform("perspective", perspective);

		

		// bind active texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container1Diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, container1Specular);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, container1Emission);

		/*
		// create a matrix which rotates vertices by 90 degrees and scales downward

		glm::mat4 rotationTransformation = glm::mat4(1.0f);
		rotationTransformation = glm::translate(rotationTransformation, glm::vec3(0.25, 0.25, 0.0f));
		rotationTransformation = glm::rotate(rotationTransformation, (float)rotation, glm::vec3(0.0, 0.0, 1.0));
		*/

		int i = 0;

		for (glm::vec3 position : cubePositions) {

			model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
			normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
			lightingShader.SetFloatMat4Uniform("model", model);
			lightingShader.SetFloatMat3Uniform("normalMatrix", normalMatrix);
			i++;

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		lightCubeShader.UseShader();

		lightCubeShader.SetFloatMat4Uniform("view", view);
		lightCubeShader.SetFloatMat4Uniform("perspective", perspective);

		for (int i = 0; i < sizeof(pointLights) / sizeof(PointLight); i++)
		{
			model = glm::mat4(1.0f);

			model = glm::translate(model, pointLights[i].position);
			model = glm::scale(model, glm::vec3(0.2f));
			lightCubeShader.SetFloatMat4Uniform("model", model);

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		simplexShader.UseShader();

		simplexShader.SetFloatUniform("currentTime", newTime / 3.0f);

		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		

		/*

		lightCubeShader.UseShader();

		model = glm::translate(model, lightCubePosition);
		model = glm::scale(model, glm::vec3(0.2f));

		lightCubeShader.SetFloatMat4Uniform("model", model);
		lightCubeShader.SetFloatMat4Uniform("view", view);
		lightCubeShader.SetFloatMat4Uniform("perspective", perspective);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		*/


		// checks if events are triggered like input or resizes, updates the window state,
		// and calls the corresponding functions which we can register as callbacks
		glfwPollEvents();

		// swaps the colour buffer that is displayed to the screen (replaces the previously displayed front buffer with the previously written to back buffer)
		glfwSwapBuffers(window);


		// clears the depth buffer for the next loop
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);


	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);

	lightingShader.DeleteShaderProgram();
	lightCubeShader.DeleteShaderProgram();

	// As soon as we exit the render loop we would like to properly clean / delete all of GLFW's resources that were allocated.
	glfwTerminate();

	return 0;
}