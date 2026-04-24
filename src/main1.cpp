#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <lib/Shader.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


//εξαγωνο 
GLfloat R = 1.0f;
GLfloat G = 0.5f;
GLfloat B = 0.2f;

//Δεκαγωνο 
GLfloat R1 = 0.2f;
GLfloat G1 = 0.8f;
GLfloat B1 = 0.5f;

GLboolean isWireframe = GL_FALSE;
GLboolean w_CurrentFrameState = GL_FALSE; //state of current frame 
GLboolean w_LastFrameState = GL_FALSE; // state of last frame 

void FrameBufferSizeCallback(GLFWwindow* givenWindow, int givenWidth, int givenHeight)
{
	glViewport(0, 0, givenWidth, givenHeight);
}


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		isWireframe = !isWireframe;

		if (isWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
			
		}
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
}
}
int main()
{
	// Inititlaize GLFW 
	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw!" << std::endl;
		return -1;
	}

	//make window 
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL ERGASIA2", NULL, NULL);
	
	if (!window)
	{
		std::cout << "Failed to initialize the window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Set the callback function
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	//glfwSetKeyCallback(window, KeyCallback);

	//initialize GLEW 
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize glew!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//set up IMGUI 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//create the Vertices 
	GLfloat hexagonVertices[] = {
	-0.7f,0.4f,0.0f,
	-0.5f ,0.6f,0.0f,
	-0.3f,0.4f,0.0f,
	-0.3f,0.1f,0.0f,
	-0.5f,-0.1f,0.0f,
	-0.7f,0.1f,0.0f
	};

	GLfloat decagonVertices[] = {
	0.50f,  0.80f, 0.0f,  // Πάνω κέντρο
	 0.68f,  0.72f, 0.0f,
	 0.78f,  0.55f, 0.0f,
	 0.78f,  0.35f, 0.0f,
	 0.68f,  0.18f, 0.0f,
	 0.50f,  0.10f, 0.0f,  // Κάτω κέντρο
	 0.32f,  0.18f, 0.0f,
	 0.22f,  0.35f, 0.0f,
	 0.22f,  0.55f, 0.0f,
	 0.32f,  0.72f, 0.0f
	};


	//create VBOS and VAOs
	GLuint VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);


	//setup of hexagon 
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexagonVertices), hexagonVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	//set up decagon 
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(decagonVertices), decagonVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	Shader shaderProgram("res/Shaders/VertexShader_11.glsl", "res/Shaders/FragmentShader_11.glsl");


	//initialize window run 
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		w_CurrentFrameState= (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);


		if (w_CurrentFrameState == GL_TRUE && w_LastFrameState == GL_FALSE)
		{
			isWireframe = !isWireframe; 

			if (isWireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		w_LastFrameState = w_CurrentFrameState;
		//set up Imgui Frame 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//attach the 6 variables R,G,B,R1,G1,B1 in IMGUI 

		ImGui::Begin("color controls for 59065");
		ImGui::SliderFloat("R", &R, 0.0f, 1.0f);
		ImGui::SliderFloat("G", &G, 0.0f, 1.0f);
		ImGui::SliderFloat("B", &B, 0.0f, 1.0f);
		ImGui::Separator();

		ImGui::SliderFloat("R1", &R1, 0.0f, 1.0f);
		ImGui::SliderFloat("G1", &G1, 0.0f, 1.0f);
		ImGui::SliderFloat("B1", &B1, 0.0f, 1.0f);
		ImGui::End();


		//rendering 
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		//adjust the color of the haxagon,decagon
		shaderProgram.use();
		GLint colorLoc = glGetUniformLocation(shaderProgram.ID, "color1");

		glUniform3f(colorLoc, R, G, B);
		//draw hexagon
		glBindVertexArray(VAOs[0]); 
		glDrawArrays(GL_POLYGON, 0, 6);


		glUniform3f(colorLoc, R1, G1, B1);
		//draw decagon
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_POLYGON, 0, 10);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
