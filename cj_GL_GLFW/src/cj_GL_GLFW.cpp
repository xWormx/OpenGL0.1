#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include "..\glad\glad.c"
#include "..\GLFW\glfw3.h"

#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"
#include "..\glm\gtc\type_ptr.hpp"

#include "Game.cpp"
#include "Input.cpp"

void setViewportToWindowSize(GLFWwindow *window);
void createContextAndLoadGLAD(GLFWwindow *window);

unsigned int createAndCompileShader(char *filePath, GLenum shadertype);
unsigned int linkShaderProgram2(unsigned int shader1, unsigned int shader2);

GLFWwindow* setupGLFWandGLAD(int windowWidth, int windowHeight, char* appName);

int main()
{

	GLFWwindow *window = {};
	window = setupGLFWandGLAD(800, 600, "cj_GL_GLFW");

	unsigned int vertexShader = createAndCompileShader("vertexShader.glsl", GL_VERTEX_SHADER);
	unsigned int fragmentShader = createAndCompileShader("fragmentShader.glsl", GL_FRAGMENT_SHADER);

	unsigned int shaderProgram = linkShaderProgram2(vertexShader, fragmentShader);


	float vertices[] =
	{
		-1.0f, -1.0f, 0.0f, 	1.0f, 0.0f, 0.0f, 1.0f,		 0.0f,  0.0f,
		-1.0f,  1.0f, 0.0f, 	0.0f, 1.0f, 0.0f, 1.0f,		 0.0f,  1.0f,
		 1.0f,  1.0f, 0.0f, 	0.0f, 0.0f, 1.0f, 1.0f,		 1.0f,  1.0f,
		 1.0f, -1.0f, 0.0f, 	0.8f, 0.3f, 1.0f, 1.0f,		 1.0f,  0.0f
	};

	unsigned int indices[]=
	{
		0, 1, 3,
		1, 2, 3,
	};

	/* Vertex buffer object, has unique ID corresponding to the buffer
	   glGenBuffer(GLsizei n, GLuint *buffers);
	   Returns n buffer object names in buffers */

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/* !!IMPORTANT!! to bind the vertex array object first, then bind and set vBuffers
	   and configure attributes */
	glBindVertexArray(VAO);

	/* Bind the buffer object to GL_ARRAY_BUFFER target
	   glBindBuffer(GLenum target, GLuint buffer); */
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy the vertex data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);




	// TEXTURE LOADING
	unsigned int texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderCol[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderCol);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width = 0;
	int height = 0;
	int nrChannels = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char *texData = stbi_load("myPng.png", &width, &height, &nrChannels, 0);
	
	if(texData)
	{
		// FILFORMAT KAN VARA PNG = GL_BGRA, JPG = GL_RGB 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load myPng.png\n");
	}

	// free image data
	stbi_image_free(texData);

	float player1MoveX = 0;
	float player1MoveY = 0;
	
	float player2MoveX = 0;
	float player2MoveY = 0;

	unsigned int vertxUniColorLocation = glGetUniformLocation(shaderProgram, "uniformCol");
	unsigned int vertxUniPosLocation = glGetUniformLocation(shaderProgram, "uniPos");
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

	float vector[] = { 1.0f, 0.0f, 0.0f, 0.0f };

	Brick playerbrick = {};
	matrix_makeIdentity(playerbrick.matrix);
	playerbrick.setX(0.0f);
	playerbrick.setY(-0.9f);
	playerbrick.setWidth(0.125f);
	playerbrick.setHeight(0.025f);




	Brick bricks[7];
	for (int i = 0; i < 7; i++)
	{
		matrix_makeIdentity(bricks[i].matrix);
		bricks[i].setY(0.8f);
		bricks[i].setX(-0.95f + (i * 0.20f));
		bricks[i].setHeight( 0.025f);
		bricks[i].setWidth(0.08f); 
	}

	Brick ball;
	matrix_makeIdentity(ball.matrix);
	ball.setX(0.0f);
	ball.setY(0.0f);
	ball.setWidth(0.015f);
	ball.setHeight(0.015f);
	
	float ballIncX = 0.01f;
	float ballIncY = 0.02f;
	
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while(!glfwWindowShouldClose(window))
	{
		setViewportToWindowSize(window);
		checkInput(window);
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);
	
		// Use the resulting shader program which every shader and rendering call will use
		glUseProgram(shaderProgram);

		processInput(&playerbrick);

		for (int i = 0; i < 7; i++)
		{
			if(playerbrick.getY() + playerbrick.getHeight() >= bricks[i].getY())
			{
					printf("COLLISION\n");
					playerbrick.setY((bricks[0].getY() - playerbrick.getHeight()) - 1.0f);
			}	
		}

		if(ball.getX() + ball.getWidth() >= 1.0f)
		{
				ballIncX = -0.01f;
		}
		if(ball.getX() <= -1.0f)
		{
				ballIncX = 0.01f;
		} 
		if(ball.getY() + ball.getHeight() >= 1.0f)
		{
				ballIncY = -0.02f;
		}
		if( (  		ball.getY() <= (playerbrick.getY() + playerbrick.getHeight())) &&
				 (( (ball.getX() + ball.getWidth()) >= playerbrick.getX())		  && 
				 ( (ball.getX() <= (playerbrick.getX() + playerbrick.getWidth()))))
			   )
		{
				ballIncY = 0.02f;
		}

		ball.moveX(ballIncX);
		ball.moveY(ballIncY);



		

		glUniformMatrix4fv(transformLoc, 1, GL_TRUE, &playerbrick.matrix[0][0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		for (int i = 0; i < 7; i++)
		{
			glUniformMatrix4fv(transformLoc, 1, GL_TRUE, &bricks[i].matrix[0][0]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glUniformMatrix4fv(transformLoc, 1, GL_TRUE, &ball.matrix[0][0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();


	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}


void setViewportToWindowSize(GLFWwindow* window)
{
	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);

	glViewport(0, 0, width, height);
}


unsigned int createAndCompileShader(char *filePath, GLenum shadertype)
{
	FILE *file = fopen(filePath, "r"); 

	if(!file)
	{
		printf("Can't open file\n");
	}

	int nChars = 512; 
	char fileBuffer[512] = {};
	char *shaderBuffer = (char*)malloc(512);
	strcpy(shaderBuffer, "");
	
	while(fgets(fileBuffer, nChars, file) != 0)
	{
			strcat(shaderBuffer, fileBuffer);
	}

	printf("length of shader: %d\n", (int)strlen(shaderBuffer));

	fclose(file);
	free(shaderBuffer);
	
	
	// Create a shader object and store it as an ID generated by 
	// glCreateShader();
	unsigned int shader = glCreateShader(shadertype);

	// Attach the shader source code to the shader object and compile the shader
	
	glShaderSource(shader, 1, &shaderBuffer, 0);
	glCompileShader(shader);


	// Checking for shader compilation errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shader, 512, 0, infoLog);
		printf("ERROR-SHADER-VERTEX-COMPILATION_FAILED\n%s", infoLog);
	}

	return shader;
}

unsigned int linkShaderProgram2(unsigned int shader1, unsigned int shader2)
{

	// Create a shader program object which is the final linked verions of multiple shaders combined
	unsigned int shaderProgram = glCreateProgram();

	// Attach the shaders to the program and link them
	glAttachShader(shaderProgram, shader1);
	glAttachShader(shaderProgram, shader2);

	glLinkProgram(shaderProgram);

	int success = 0;
	char *infoLog = {};
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, 0, infoLog);
		printf("ERROR-SHADER-PROGRAM-LINK_FAILED\n%s", infoLog);
	}

	// Delete the shaders objects after they're linked togheter
	glDeleteShader(shader1);
	glDeleteShader(shader2);

	return shaderProgram;
}

void createContextAndLoadGLAD(GLFWwindow *window)
{
	if(!window)
	{
		MessageBox(0, "window fail", 0, 0);
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);


	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		MessageBox(0, "GLAD loader fail", 0, 0);
	}
	
	char *version = (char*)glGetString(GL_VERSION);
	printf("%s\n", version);

	int nvertexAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nvertexAttributes);
	printf("Maximum nr of vertex attribs supported: %d\n", nvertexAttributes);
}

GLFWwindow* setupGLFWandGLAD(int windowWidth, int windowHeight, char* appName)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, appName, 0, 0);

	if(!window)
	{
		MessageBox(0, "window fail", 0, 0);
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);


	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		MessageBox(0, "GLAD loader fail", 0, 0);
	}
	
	char *version = (char*)glGetString(GL_VERSION);
	printf("%s\n", version);

	int nvertexAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nvertexAttributes);
	printf("Maximum nr of vertex attribs supported: %d\n", nvertexAttributes);

		return window;
}

