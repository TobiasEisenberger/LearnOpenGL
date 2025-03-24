#version 330 core

in vec3 normal;
in vec3 fragmentPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 color;

void main()
{
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - fragmentPos);

	float diff = max(dot(normalize(normal), lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5f;
	vec3 viewDirection = normalize(viewPos - fragmentPos);
	vec3 reflectDir = reflect(-lightDir, normalize(normal));

	int shininess = 32;
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0f), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0f);
}