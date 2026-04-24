#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <lib/Shader.h>

// movement variables
GLfloat posX = 0.0f;
GLfloat posY = 0.0f;
GLfloat moveSpeed = 0.001f;
GLfloat limit = 0.8f;  // limits so that the shape do not move outside the screen

void FrameBufferSizeCallback(GLFWwindow* givenWindow, int givenWidth, int givenHeight)
{
	glViewport(0, 0, givenWidth, givenHeight);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// handle movement  WASD
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && posY < limit)
		posY += moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && posY > -limit)
		posY -= moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && posX < limit)
		posX += moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && posX > -limit) 
		posX -= moveSpeed;
}

int main()
{
  //initialize GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw!" << std::endl;
		return -1;
	}
//create window 
	GLFWwindow* window = glfwCreateWindow(800, 600, "AM: 59065 - Ergasia 2 - Exercise 2", NULL, NULL);
	if (!window) {
		std::cout << "Failed to initialize the window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

  //initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize glew!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// vertices of hexagon centered at (0,0)
	GLfloat hexagonVertices[] = {
		-0.15f,  0.15f, 0.0f,
		 0.0f,  0.3f, 0.0f,
		 0.15f,  0.15f, 0.0f,
		 0.15f, -0.15f, 0.0f,
		 0.0f, -0.3f, 0.0f,
		-0.15f, -0.15f, 0.0f
	};
//create VBOS,VAOS
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
//handle the VAOS,VBOS
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexagonVertices), hexagonVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
//load shaders
	Shader shaderProgram("res/Shaders/VertexShader_12.txt", "res/Shaders/FragmentShader_12.txt");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();

		// update Uniform for movement
		GLint offsetLoc = glGetUniformLocation(shaderProgram.ID, "offset");
		glUniform2f(offsetLoc, posX, posY);

		glBindVertexArray(VAO);
		glDrawArrays(GL_POLYGON, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
//cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
