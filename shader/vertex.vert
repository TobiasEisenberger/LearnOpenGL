#version 330 core

layout (location = 0) in vec3 pos;
// layout (location = 1) in vec3 color;
layout (location = 1) in vec2 textureCoord;
// layout (location = 2) in vec2 textureCoord;

// out vec3 ourColor;
out vec2 v_TextureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	// ourColor = color;
	v_TextureCoord = textureCoord;
}