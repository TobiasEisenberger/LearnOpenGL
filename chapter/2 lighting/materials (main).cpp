#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <GLM/fwd.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include <stb/stb_image.h>

#include "Shader.h"
#include "Camera.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void checkShaderCompilation(unsigned int shaderId);
void checkShaderLinkage(unsigned int shaderProgramId);

int g_width{ 800 };
int g_height{ 600 };

float lastX = 400.0f;
float lastY = 300.0f;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Camera camera(glm::vec3(0.0f, 1.0f, 6.0f));

int main()
{
	std::cout << "Hello World!\n";

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!\n";
		return -1;
	}

	glViewport(0, 0, g_width, g_height);
	glfwSwapInterval(1);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

	unsigned int VAOCubeId;
	glGenVertexArrays(1, &VAOCubeId);
	glBindVertexArray(VAOCubeId);

	float vertices[] = {
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
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	};

	unsigned int VBOCubeId;
	glGenBuffers(1, &VBOCubeId);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCubeId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int VAOLightCubeId;
	glGenVertexArrays(1, &VAOLightCubeId);
	glBindVertexArray(VAOLightCubeId);

	glBindVertexArray(VAOLightCubeId);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCubeId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	Shader shaderCube("shader/chapter/lighting/basic-lighting/vertex-color.vert", "shader/chapter/lighting/materials/fragment-color.frag");
	shaderCube.use();
	shaderCube.setFloat3("material.ambient", 1.0f, 0.5f, 0.31f);
	shaderCube.setFloat3("material.diffuse", 1.0f, 0.5f, 0.31f);
	shaderCube.setFloat3("material.specular", 0.5f, 0.5f, 0.5f);
	shaderCube.setInt("material.shininess", 32);
	shaderCube.setFloat3("light.specular", 1.0f, 1.0f, 1.0f);

	Shader shaderLight("shader/chapter/lighting/color/vertex-color-light.vert", "shader/chapter/lighting/materials/fragment-color-light.frag");

	glm::mat4 identity(1.0f);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(glfwGetCurrentContext());

		glm::mat4 viewMatrix = camera.GetViewMatrix();
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)g_width / g_height, 0.1f, 100.f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		shaderCube.use();
		shaderCube.setMat4f("model", glm::value_ptr(glm::mat4(1.0f)));
		shaderCube.setMat4f("view", glm::value_ptr(viewMatrix));
		shaderCube.setMat4f("projection", glm::value_ptr(projectionMatrix));
		
		shaderCube.setFloat3("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
		shaderCube.setFloat3("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
		shaderCube.setFloat3("lightColor", lightColor.x, lightColor.y, lightColor.z);

		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		shaderCube.setFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);

		shaderCube.setFloat3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		
		glBindVertexArray(VAOCubeId);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		shaderLight.use();
		glm::mat4 lightTransform = glm::translate(glm::mat4(1.0f), lightPos);
		lightTransform = glm::scale(lightTransform, glm::vec3(0.2f));
		shaderLight.setMat4f("model", glm::value_ptr(lightTransform));
		shaderLight.setMat4f("view", glm::value_ptr(viewMatrix));
		shaderLight.setMat4f("projection", glm::value_ptr(projectionMatrix));

		shaderLight.setFloat3("lightColor", lightColor.x, lightColor.y, lightColor.z);

		glBindVertexArray(VAOLightCubeId);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAOCubeId);
	glDeleteVertexArrays(1, &VAOLightCubeId);
	glDeleteBuffers(1, &VBOCubeId);
	glDeleteProgram(shaderCube.ID);
	glDeleteProgram(shaderLight.ID);

	glfwTerminate();

	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	g_width = width;
	g_height = height;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void checkShaderCompilation(unsigned int shaderId)
{
	int wasSuccessful;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &wasSuccessful);

	char infoLog[512];
	if (!wasSuccessful)
	{
		glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
		std::cout << "Shader compilation failed!\n";
		std::cout << infoLog << '\n';
	}
}

void checkShaderLinkage(unsigned int shaderProgramId)
{
	int wasSuccessful;
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &wasSuccessful);

	char infoLog[512];
	if (!wasSuccessful)
	{
		glGetProgramInfoLog(shaderProgramId, 512, nullptr, infoLog);
		std::cout << "Shader linkage failed!\n";
		std::cout << infoLog << '\n';
	}
}
