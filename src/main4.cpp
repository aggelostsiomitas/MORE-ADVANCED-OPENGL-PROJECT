#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<string>
#include <lib/Shader2.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera_2.h"



//Lightning variables
bool pointLightsActive = true;
bool dirLightsActive = true;
int attenuationChoice = 1; 
float lightMovingRadius = 5.0f;

//data for attenuation
struct Attenuation {
	float constant, linear, quadratic;
};

Attenuation attenOptions[] = {
	{1.0f, 0.09f, 0.032f},   
	{1.0f, 0.07f, 0.017f},   
	{1.0f, 0.045f, 0.0075f},
	{1.0f, 0.027f, 0.0028f}, 
	{1.0f, 0.014f, 0.0007f}  
};
//call Camer.h to handle Camera_2.h movement
Camera myCamera(glm::vec3(0.0f, 0.0f, 10.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;


void FrameBufferSizeCallback(GLFWwindow* givenWindow, int givenWidth, int givenHeight)
{
	glViewport(0, 0, givenWidth, givenHeight);
}

void ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Cehck for  Shift key for speed
	bool shiftPressed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	//handle movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		myCamera.ProcessKeyboard(FORWARD, deltaTime, shiftPressed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		myCamera.ProcessKeyboard(BACKWARD, deltaTime, shiftPressed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		myCamera.ProcessKeyboard(LEFT, deltaTime, shiftPressed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		myCamera.ProcessKeyboard(RIGHT, deltaTime, shiftPressed);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		myCamera.ProcessKeyboard(UP, deltaTime, shiftPressed);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		myCamera.ProcessKeyboard(DOWN, deltaTime, shiftPressed);
	}
}

int main()
{
	//initialize glfw
	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw!" << std::endl;
		return -1;
	}

	//initialize the window 
	GLFWwindow* window = glfwCreateWindow(800, 600, "AM: 59065 - Ergasia 2 - Exercise 3", NULL, NULL);
	if (!window) {
		std::cout << "Failed to initialize the window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	//initialize glew 
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize glew!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	//set up Imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//initialize shaders
	Shader lightingShader("res/Shaders_2/VertexShader_21.txt", "res/Shaders_2/FragmentShader_21.txt");
	Shader lampShader("res/Shaders_2/VertexShader_Light_21.txt", "res/Shaders_2/FragmentShader_Light_21.txt");

	//Vertex Data
	GLfloat  vertices[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,


		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f

	};
	//initialize VBO,VAO
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//materials
	glm::vec3 cubePositions[] = {
		glm::vec3(0,0,0), 
		glm::vec3(2,3,-5), 
		glm::vec3(-2,-2,-3), 
		glm::vec3(4,0,-8), 
		glm::vec3(-4,4,-6),
		glm::vec3(1,-3,-4), 
		glm::vec3(-3,1,-2), 
		glm::vec3(5,5,-10), 
		glm::vec3(-5,-4,-7), 
		glm::vec3(0,5,-3)
	};

	//runnning loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// ImGui Render 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Lighting Controls (AM: 59065)");
		ImGui::Checkbox("Point Lights Active", &pointLightsActive);
		ImGui::Checkbox("Dir Lights Active", &dirLightsActive);
		ImGui::Separator();
		ImGui::Text("Attenuation Distance:");
		ImGui::RadioButton("20", &attenuationChoice, 0); 
		ImGui::SameLine();
		ImGui::RadioButton("32", &attenuationChoice, 1); 
		ImGui::SameLine();
		ImGui::RadioButton("50", &attenuationChoice, 2); 
		ImGui::SameLine();
		ImGui::RadioButton("65", &attenuationChoice, 3); 
		ImGui::SameLine();
		ImGui::RadioButton("100", &attenuationChoice, 4);
		ImGui::SliderFloat("Moving Light Radius", &lightMovingRadius, 2.0f, 10.0f);
		ImGui::End();


		//Lighting Calculations
		lightingShader.use();
		lightingShader.setVec3("viewPos", myCamera.Position);
		lightingShader.setBool("pointLightsActive", pointLightsActive);
		lightingShader.setBool("dirLightsActive", dirLightsActive);

		lightingShader.setVec3("dirLights[0].direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLights[1].direction", 1.0f, -0.5f, 0.0f);
		glm::vec3 movingLightPos(sin(currentFrame) * lightMovingRadius, 2.0f, cos(currentFrame) * lightMovingRadius);
		

		// Point Lights 
		glm::vec3 pPositions[] = { 
			glm::vec3(0.7f, 0.2f, 2.0f), 
			glm::vec3(2.3f, -3.3f, -4.0f), 
			glm::vec3(-4.0f, 2.0f, -12.0f) 
		};

		for (int i = 0; i < 3; i++) {
			std::string base = "pointLights[" + std::to_string(i) + "].";
			lightingShader.setVec3(base + "position", pPositions[i]);
			lightingShader.setFloat(base + "constant", attenOptions[attenuationChoice].constant);
			lightingShader.setFloat(base + "linear", attenOptions[attenuationChoice].linear);
			lightingShader.setFloat(base + "quadratic", attenOptions[attenuationChoice].quadratic);
			lightingShader.setVec3(base + "ambient", 0.1f, 0.1f, 0.1f);
			lightingShader.setVec3(base + "diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.setVec3(base + "specular", 1.0f, 1.0f, 1.0f);
		}

		// Moving Light 
		glm::vec3 movPos(sin(currentFrame)* lightMovingRadius, 1.0f, cos(currentFrame)* lightMovingRadius);
		lightingShader.setVec3("pointLights[3].position", movPos);
		lightingShader.setFloat("pointLights[3].linear", attenOptions[attenuationChoice].linear);

		glm::mat4 view = myCamera.GetViewMatrix();
		glm::mat4 proj = glm::perspective(glm::radians(myCamera.Zoom), 1024.0f / 768.0f, 0.1f, 100.0f);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", proj);

		glBindVertexArray(VAO);
		for (int i = 0; i < 10; i++) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			lightingShader.setMat4("model", model);
			lightingShader.setVec3("material.ambient", 0.1f, 0.1875f, 0.1745f); // Example Emerald
			lightingShader.setVec3("material.diffuse", 0.396f, 0.74151f, 0.69102f);
			lightingShader.setVec3("material.specular", 0.297254f, 0.30829f, 0.306678f);
			lightingShader.setFloat("material.shininess", 12.8f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lampShader.use();
		lampShader.setMat4("view", view); 
		lampShader.setMat4("projection", proj);
		lampShader.setMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), movPos), glm::vec3(0.2f)));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Render(); 
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window); glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
