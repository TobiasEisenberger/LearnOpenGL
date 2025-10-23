#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <GLM/fwd.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>

#include <stb/stb_image.h>

#include "Shader.h"
#include "Camera.h"
#include "Sphere.h"

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
	
	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMapTexture;
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};

	unsigned int VAOLightSphereId;
	glGenVertexArrays(1, &VAOLightSphereId);
	glBindVertexArray(VAOLightSphereId);

	unsigned int VBOSphereId;
	glGenBuffers(1, &VBOSphereId);

	Sphere lightSphere;

	glBindBuffer(GL_ARRAY_BUFFER, VBOSphereId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lightSphere.GetVertices().size(), lightSphere.GetVertices().data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	unsigned int EBOSphereId;
	glGenBuffers(1, &EBOSphereId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOSphereId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * lightSphere.GetIndices().size(), lightSphere.GetIndices().data(), GL_STATIC_DRAW);

	unsigned int planeVAO;
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);

	unsigned int planeVBO;
	glGenBuffers(1, &planeVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	unsigned int VAOCubeId;
	glGenVertexArrays(1, &VAOCubeId);
	glBindVertexArray(VAOCubeId);

	float vertices[] = {
		// positions          // normals
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
	
	Shader shaderDepthMap("shader/chapter/4-advanced-lighting/shadow-mapping/depth-map.vert", "shader/chapter/4-advanced-lighting/shadow-mapping/depth-map.frag");
	Shader shaderLight("shader/chapter/1-lighting/color/vertex-color-light.vert", "shader/chapter/1-lighting/color/fragment-color-light.frag");
	Shader shader("shader/chapter/4-advanced-lighting/shadow-mapping/lighting.vert", "shader/chapter/4-advanced-lighting/shadow-mapping/lighting.frag");
	shader.use();
	shader.setInt("shadowMap", 0);

	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

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
				
		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		// render scene from light's point of view
		shaderDepthMap.use();
		shaderDepthMap.setMat4f("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		shaderDepthMap.setMat4f("model", glm::value_ptr(identity));
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glm::mat4 cubeModelMatrix = glm::translate(identity, glm::vec3(0.0f, 1.5f, 0.0));
		cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(0.5f));
		shaderDepthMap.setMat4f("model", glm::value_ptr(cubeModelMatrix));
		glBindVertexArray(VAOCubeId);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cubeModelMatrix = glm::translate(identity, glm::vec3(2.0f, 0.0f, 1.0));
		cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(0.5f));
		shaderDepthMap.setMat4f("model", glm::value_ptr(cubeModelMatrix));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cubeModelMatrix = glm::translate(identity, glm::vec3(-1.0f, 0.0f, 2.0));
		cubeModelMatrix = glm::rotate(cubeModelMatrix, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(0.5f));
		shaderDepthMap.setMat4f("model", glm::value_ptr(cubeModelMatrix));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, g_width, g_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render light
		glBindVertexArray(VAOLightSphereId);
		shaderLight.use();
		glm::mat4 lightTransform = glm::translate(glm::mat4(1.0f), lightPos);
		lightTransform = glm::scale(lightTransform, glm::vec3(0.15f));
		shaderLight.setMat4f("model", glm::value_ptr(lightTransform));
		shaderLight.setMat4f("view", glm::value_ptr(viewMatrix));
		shaderLight.setMat4f("projection", glm::value_ptr(projectionMatrix));
		glDrawElements(GL_TRIANGLES, lightSphere.GetIndices().size(), GL_UNSIGNED_INT, 0);

		// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		shader.use();
		shader.setMat4f("projection", value_ptr(projectionMatrix));
		shader.setMat4f("view", value_ptr(viewMatrix));
		// set light uniforms
		shader.setFloat3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		shader.setFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);
		shader.setMat4f("lightSpaceMatrix", value_ptr(lightSpaceMatrix));
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);

		shader.setMat4f("model", glm::value_ptr(identity));
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		cubeModelMatrix = glm::translate(identity, glm::vec3(0.0f, 1.5f, 0.0));
		cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(0.5f));
		shader.setMat4f("model", glm::value_ptr(cubeModelMatrix));
		glBindVertexArray(VAOCubeId);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cubeModelMatrix = glm::translate(identity, glm::vec3(2.0f, 0.0f, 1.0));
		cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(0.5f));
		shader.setMat4f("model", glm::value_ptr(cubeModelMatrix));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cubeModelMatrix = glm::translate(identity, glm::vec3(-1.0f, 0.0f, 2.0));
		cubeModelMatrix = glm::rotate(cubeModelMatrix, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(0.5f));
		shader.setMat4f("model", glm::value_ptr(cubeModelMatrix));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAOCubeId);
	glDeleteVertexArrays(1, &VAOLightSphereId);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &VBOCubeId);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &VBOSphereId);
	glDeleteBuffers(1, &EBOSphereId);
	glDeleteProgram(shaderDepthMap.ID);
	glDeleteFramebuffers(1, &depthMapFBO);

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