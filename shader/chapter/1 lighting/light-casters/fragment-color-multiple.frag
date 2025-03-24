#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	int shininess;
};

struct DirectionalLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	
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

uniform vec3 viewPos;

uniform Material material;
uniform DirectionalLight directionalLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

out vec4 color;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPos, vec3 viewDir);

void main()
{
	// properties
    vec3 normalizedNormal = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragmentPos);

    // phase 1: Directional lighting
    vec3 result = calculateDirectionalLight(directionalLight, normalizedNormal, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += calculatePointLight(pointLights[i], normalizedNormal, fragmentPos, viewDir);    
    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
	color = vec4(result, 1.0f);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 texelDiffuse = vec3(texture(material.diffuse, textureCoords));
	vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * texelDiffuse;
    vec3 diffuse = light.diffuse * texelDiffuse;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, textureCoords));
    return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragmentPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragmentPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
	vec3 texelDiffuse = vec3(texture(material.diffuse, textureCoords));
    vec3 ambient = light.ambient * texelDiffuse;
    vec3 diffuse = light.diffuse * diff * texelDiffuse;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, textureCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}