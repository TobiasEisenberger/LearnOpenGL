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

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkShaderCompilation(unsigned int shaderId);
void checkShaderLinkage(unsigned int shaderProgramId);

float g_blendFactor{ 0.5f };
int g_width{ 800 };
int g_height{ 600 };

int main()
{
	std::cout << "Hello World!\n";

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	vec = translation * vec;
	std::cout << "XYZ after translation: " << vec.x << "," << vec.y << "," << vec.z << '\n';

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
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!\n";
		return -1;
	}

	glViewport(0, 0, g_width, g_height);
	glfwSwapInterval(1);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

	unsigned int vertexArrayObjectId;
	glGenVertexArrays(1, &vertexArrayObjectId);
	glBindVertexArray(vertexArrayObjectId);

	//float vertices[] = {
	//		// positions          // colors           // texture coords
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
	//};

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int vertexBufferObjectId;
	glGenBuffers(1, &vertexBufferObjectId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	
	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// color attribute
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/

	// texture attribute
	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int elementBufferObjectId;
	glGenBuffers(1, &elementBufferObjectId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjectId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader shader("shader/vertex.vert", "shader/fragment.frag");
	shader.use();

	unsigned int textureIds[2];
	glGenTextures(1, textureIds);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIds[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// exercise 2 (display smiley 4 times)
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("resource/img/wooden-container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureIds[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// exercise 2 (display smiley 4 times)
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

	data = stbi_load("resource/img/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	/*glm::mat4 scaleRotTransform = glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	scaleRotTransform = glm::scale(scaleRotTransform, glm::vec3(0.5f, 0.5f, 0.5f));

	shader.setMat4f("transformation", glm::value_ptr(scaleRotTransform));*/

	glm::mat4 identity(1.0f);
	shader.setMat4f("transformation", glm::value_ptr(identity));


	// glm::mat4 modelTransformation = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)g_width / g_height, 0.1f, 100.f);

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
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	while (!glfwWindowShouldClose(window))
	{
		processInput(glfwGetCurrentContext());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// glm::mat4 modelTransformation = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		shader.setFloat("blendFactor", g_blendFactor);
		// shader.setMat4f("model", glm::value_ptr(modelTransformation));
		shader.setMat4f("view", glm::value_ptr(viewMatrix));
		shader.setMat4f("projection", glm::value_ptr(projectionMatrix));
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		
		// glDrawArrays(GL_TRIANGLES, 0, 36);
		
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 modelTransformation = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			float angle = 20.0f * (i + 1);
			modelTransformation = glm::rotate(modelTransformation, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4f("model", glm::value_ptr(modelTransformation));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		/*glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glm::mat4 spinTransformation = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.5f, 0.0f));
		spinTransformation = glm::rotate(spinTransformation, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));*/

		// float timePassedSeconds = (float)(float)glfwGetTime();
		// Exercise 1
		/*glm::mat4 spinTransformation = glm::rotate(glm::mat4(1.0f), timePassedSeconds, glm::vec3(0.0f, 0.0f, 1.0f));
		spinTransformation = glm::translate(spinTransformation, glm::vec3(0.5f, -0.5f, 0.0f));
		shader.setMat4f("transformation", glm::value_ptr(spinTransformation));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);*/

		// Exercise 2
		/*float scaleFactor = glm::sin(timePassedSeconds);
		glm::mat4 scaleTransformation = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));
		spinTransformation = glm::translate(scaleTransformation, glm::vec3(-0.5f, 0.5f, 0.0f));
		shader.setMat4f("transformation", glm::value_ptr(spinTransformation));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vertexArrayObjectId);
	glDeleteBuffers(1, &vertexBufferObjectId);
	glDeleteBuffers(1, &elementBufferObjectId);
	glDeleteTextures(2, textureIds);

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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		g_blendFactor += 0.1f;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		g_blendFactor -= 0.1f;
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
