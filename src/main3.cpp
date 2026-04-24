#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<string>
#include <lib/Shader.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"


float radius12 = 3.0f; //radius between first cube and second
float radius23 = 2.0f; //radius between second cube and third
bool rotationActive[5] = { true,true,true,true,true };
float rotationsSpeed[5] = { 1.0f,1.0f,1.0f,1.0f,1.0f };
float rotationAngles[5] = { 0.0f,0.0f,0.0f,0.0f,0.0f };

void FrameBufferSizeCallback(GLFWwindow* givenWindow, int givenWidth, int givenHeight)
{
	glViewport(0, 0, givenWidth, givenHeight);
}




int main()
{

	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw!" << std::endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "AM: 59065 - Ergasia 2 - Exercise 3", NULL, NULL);
	if (!window) {
		std::cout << "Failed to initialize the window!" << std::endl;
		glfwTerminate();
		return -1; 
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

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


	Shader shader("res/Shaders_59065/VertexShader_13.txt", "res/Shaders_59065/FragmentShader_13.txt");

	GLfloat vertices[] = {
		//position            //color             
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0,

		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0,

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

		0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.5f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.5f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.5f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.5f,

		-0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.0f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	GLfloat lastTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentTime = glfwGetTime();
		GLfloat deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Cube Controls");
		for (int i = 0; i < 5; i++) {
			std::string rotName = "rotation" + std::to_string(i + 1);
			std::string speedName = "speed" + std::to_string(i + 1);
			ImGui::Checkbox(rotName.c_str(), &rotationActive[i]); 
			ImGui::SameLine();
			ImGui::SliderFloat(speedName.c_str(), &rotationsSpeed[i], 1.0f, 10.0f);
			if (rotationActive[i]) 
			{
				rotationAngles[i] += rotationsSpeed[i] * deltaTime; 
			}
		}

		ImGui::SliderFloat("radius 12", &radius12, 1.0f, 10.0f);
		ImGui::SliderFloat("radius 23", &radius23, 1.0f, 10.0f);
		ImGui::End();

		//use the shader
		shader.use();

		//locations of uniforms
		GLuint projectionLoc = glGetUniformLocation(shader.ID, "projection");
		GLuint viewLoc = glGetUniformLocation(shader.ID, "view");
		GLuint modelLoc = glGetUniformLocation(shader.ID, "model");

		//camera 
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(5, 5, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);

		//first cube 
		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::rotate(model1, rotationAngles[0], glm::vec3(0, 1, 0));
		model1 = glm::scale(model1, glm::vec3(1.0f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//second cube 
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::rotate(model2, rotationAngles[1], glm::vec3(1, 1, 0));
		model2 = glm::translate(model2, glm::vec3(radius12, 0, 0));
		model2 = glm::rotate(model2, rotationAngles[3], glm::vec3(0, 0, 1));
		model2 = glm::scale(model2, glm::vec3(1.6f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//third cube 
		glm::mat4 model3 = glm::mat4(1.0f);
		model3 = glm::rotate(model3, rotationAngles[1], glm::vec3(1, 1, 0));
		model3 = glm::translate(model3, glm::vec3(radius12, 0, 0));
		model3 = glm::rotate(model3, rotationAngles[2], glm::vec3(1, 1, 0));
		model3 = glm::translate(model3, glm::vec3(radius23, 0, 0));
		model3 = glm::rotate(model3, rotationAngles[4], glm::vec3(1, 0, 1));
		model3 = glm::scale(model3, glm::vec3(1.5f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}
