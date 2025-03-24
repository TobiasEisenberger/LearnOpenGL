#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkShaderCompilation(unsigned int shaderId);
void checkShaderLinkage(unsigned int shaderProgramId);

int main()
{
	std::cout << "Hello World!\n";

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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

	glViewport(0, 0, 800, 600);
	glfwSwapInterval(1);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

	unsigned int vertexArrayObjectId;
	glGenVertexArrays(1, &vertexArrayObjectId);
	glBindVertexArray(vertexArrayObjectId);

	float vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, // top left 
	};

	unsigned int vertexBufferObjectId;
	glGenBuffers(1, &vertexBufferObjectId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	unsigned int elementBufferObjectId;
	glGenBuffers(1, &elementBufferObjectId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjectId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	const char* vertexShader = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);\n"
		"}\0";
	
	unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShader, nullptr);
	glCompileShader(vertexShaderId);

	const char* fragmentShader =
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentShader, nullptr);
	glCompileShader(fragmentShaderId);

	unsigned int shaderProgramId = glCreateProgram();
	glAttachShader(shaderProgramId, vertexShaderId);
	glAttachShader(shaderProgramId, fragmentShaderId);
	glLinkProgram(shaderProgramId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	glUseProgram(shaderProgramId);

	while (!glfwWindowShouldClose(window))
	{
		processInput(glfwGetCurrentContext());
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vertexArrayObjectId);
	glDeleteBuffers(1, &vertexBufferObjectId);
	glDeleteBuffers(1, &elementBufferObjectId);
	glDeleteProgram(shaderProgramId);

	glfwTerminate();

	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
