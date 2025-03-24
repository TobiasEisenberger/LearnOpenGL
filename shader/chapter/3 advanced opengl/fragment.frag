#version 330 core

in vec2 TexCoords;

uniform sampler2D texture1;
uniform float nearPlane;
uniform float farPlane;

out vec4 FragColor;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));	
}

void main()
{    
    FragColor = texture(texture1, TexCoords);

    // non-linear depth visualization
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);

    // float depth = LinearizeDepth(gl_FragCoord.z) / farPlane; // divide by far for demonstration
    // FragColor = vec4(vec3(depth), 1.0);
}