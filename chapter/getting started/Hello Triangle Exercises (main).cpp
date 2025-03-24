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

	unsigned int vertexArrayObjectIds[2];
	glGenVertexArrays(2, vertexArrayObjectIds);
	glBindVertexArray(vertexArrayObjectIds[0]);

	float vertices[] = {
		-0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		-0.25f, 0.5f, 0.0f,
	};

	unsigned int vertexBufferObjectIds[2];
	glGenBuffers(2, vertexBufferObjectIds);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	unsigned int elementBufferObjectId;
	glGenBuffers(1, &elementBufferObjectId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjectId);

	glBindVertexArray(vertexArrayObjectIds[1]);

	float verticesSecond[] = {
		0.0f, 0.0f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectIds[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSecond), verticesSecond, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

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

	const char* secondFragmentShader =
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\0";

	unsigned int secondFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(secondFragmentShaderId, 1, &secondFragmentShader, nullptr);

	glCompileShader(fragmentShaderId);
	glCompileShader(secondFragmentShaderId);

	unsigned int shaderProgramId = glCreateProgram();
	unsigned int secondShaderProgramId = glCreateProgram();

	glAttachShader(shaderProgramId, vertexShaderId);
	glAttachShader(shaderProgramId, fragmentShaderId);

	glAttachShader(secondShaderProgramId, vertexShaderId);
	glAttachShader(secondShaderProgramId, secondFragmentShaderId);

	glLinkProgram(shaderProgramId);
	glLinkProgram(secondShaderProgramId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteShader(secondFragmentShaderId);

	while (!glfwWindowShouldClose(window))
	{
		processInput(glfwGetCurrentContext());

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramId);
		glBindVertexArray(vertexArrayObjectIds[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(secondShaderProgramId);
		glBindVertexArray(vertexArrayObjectIds[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, vertexArrayObjectIds);
	glDeleteBuffers(2, vertexBufferObjectIds);

	glDeleteProgram(shaderProgramId);
	glDeleteProgram(secondShaderProgramId);

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
