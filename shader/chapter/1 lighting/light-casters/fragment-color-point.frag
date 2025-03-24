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
	// vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 normal;
in vec3 fragmentPos;
in vec2 textureCoords;

// uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

out vec4 color;

void main()
{
	vec3 texel = texture(material.diffuse, textureCoords).rgb;
	vec3 ambient = light.ambient * texel;

	vec3 lightDir = normalize(light.position - fragmentPos);
	// vec3 lightDir = normalize(-light.direction);

	float lightDistance = length(light.position - fragmentPos);
	float attenuation = 1.0f / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));

	float diff = max(dot(normalize(normal), lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * texel);

	vec3 viewDirection = normalize(viewPos - fragmentPos);
	vec3 reflectDir = reflect(-lightDir, normalize(normal));

	float spec = pow(max(dot(viewDirection, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specular, textureCoords).rgb);

	vec3 result = ambient * attenuation + diffuse * attenuation + specular * attenuation;
	color = vec4(result, 1.0f);
}