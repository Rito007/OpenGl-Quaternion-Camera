#include<iostream>
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include"glm/glm.hpp""
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/quaternion.hpp>
#include "Shader.h"


GLfloat vertices[] = { 
					0.5f, 0.0f,0.5f,       1.0f,0.0f,0.0f,
					-0.5f, 0.0f, 0.5f,     0.0f,1.0f,0.0f,
					0.5f, 0.0f,  -0.5f,     0.0f,0.0f,1.0f,
					-0.5f, 0.0f, -0.5f,     0.0f,0.0f,1.0f,
					0.0f, 1.0f, 0.0f,      0.0f,0.0f,1.0f
};


GLuint indices[] = {
	0,1,3,
	0,2,3,
	0,4,1,
	1,4,3,
	3,4,2,
	2,4,0
};

int main()
{
	//GLFW Inicialização
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "C# is for losers!", NULL, NULL);
	if(!window) { std::cout << "Erro ao criar janela..." << std::endl; return -1; }
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, 800, 800);
	
	Shader defaultShader("defaultShader.vert", "defaultShader.frag");

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	double pervTime = glfwGetTime();

	float speed = 200.0f;
	
	glm::vec3 Position = glm::vec3(0.0f,0.0f,0.0f);
	glm::quat CameraRot = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glEnable(GL_DEPTH_TEST);

	
	float timeDelta = 0;

	while (!glfwWindowShouldClose(window))
	{	

		if (glfwGetTime() - pervTime >= 1 / 60)
		{
			pervTime = glfwGetTime();
			timeDelta = (float)(glfwGetTime() - pervTime);
		}

		glClearColor(0.0f, 0.4f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double mouseY=400, mouseX=400;
		
		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &mouseX, &mouseY);
			glfwSetCursorPos(window, 800 / 2, 800 / 2);
		}


		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Position -= glm::inverse(CameraRot) * glm::vec3(0.0f,0.0f,1.0f) * speed * timeDelta * 200.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Position += glm::inverse(CameraRot) * glm::vec3(0.0f, 0.0f, 1.0f) * speed * timeDelta * 200.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Position += glm::inverse(CameraRot) * glm::vec3(1.0f,0.0f,0.0f) * speed * timeDelta * 200.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Position -= glm::inverse(CameraRot) * glm::vec3(1.0f, 0.0f, 0.0f) * speed * timeDelta * 200.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed * timeDelta * 200.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			Position += glm::vec3(0.0f, 1.0f, 0.0f) * speed * timeDelta * 200.0f;
		}

		
		float RotationX = 0.0f;
		float RotationY = 0.0f;

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);

		RotationX += (float)((mouseY - 400) / 800);
		RotationY += (float)((mouseX - 400) / 800);

		
		CameraRot *= glm::quat(glm::vec3(0.0f, RotationY, 0.0f));
		CameraRot *= glm::quat(glm::cross(glm::vec3(RotationX, 0.0f, 0.0f), CameraRot));

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		
		view = glm::toMat4(CameraRot);
		view = glm::translate(view, Position);
		proj = glm::perspective(glm::radians(45.0f), (float)800 / 800, 0.1f, 100.0f);

		defaultShader.Ativar();
		glUniformMatrix4fv(glGetUniformLocation(defaultShader.shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(defaultShader.shaderID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(defaultShader.shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);



		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
		glUniformMatrix4fv(glGetUniformLocation(defaultShader.shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	defaultShader.Deletar();
	glfwTerminate();
	return 0;
}