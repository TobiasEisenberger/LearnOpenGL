#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;

out vec4 FragColor;

void main()
{    
    FragColor = texture(screenTexture, TexCoords);
    
    // FragColor = vec4(1.0f);
    // FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}