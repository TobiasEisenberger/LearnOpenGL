#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	int shininess;
};

struct Light
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec3 fragmentPos;
in vec2 textureCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

out vec4 color;

void main()
{
	vec3 texel = texture(material.diffuse, textureCoords).rgb;
	vec3 ambient = light.ambient * texel;

	vec3 lightDir = normalize(lightPos - fragmentPos);

	float diff = max(dot(normalize(normal), lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * texel);

	vec3 viewDirection = normalize(viewPos - fragmentPos);
	vec3 reflectDir = reflect(-lightDir, normalize(normal));

	float spec = pow(max(dot(viewDirection, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specular, textureCoords).rgb);

	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}