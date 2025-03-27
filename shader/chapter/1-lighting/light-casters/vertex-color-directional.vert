#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 fragmentPos;
out vec2 textureCoords;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	
	// be careful if there are non-uniform scalings in the model matrix --> normal vector (which is only a directional vector) will be broken
	normal = vec3(model * vec4(v_normal, 0.0f));
	fragmentPos = vec3(model * vec4(pos, 1.0f));

	textureCoords = v_textureCoords;
}