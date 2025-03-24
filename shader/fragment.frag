#version 330 core

// in vec3 ourColor;
in vec2 v_TextureCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blendFactor;

out vec4 color;

void main()
{
	// color = texture(texture1, v_TextureCoord) * vec4(ourColor, 1.0f);
	// color = mix(texture(texture1, v_TextureCoord), texture(texture2, v_TextureCoord), 0.2) * vec4(ourColor, 1.0f);
	// exercise 1
	// color = mix(texture(texture1, v_TextureCoord), texture(texture2, v_TextureCoord * vec2(1.0f, -1.0f)), 0.2) * vec4(ourColor, 1.0f);
	// exercise 3
	// color = mix(texture(texture1, v_TextureCoord), texture(texture2, v_TextureCoord), blendFactor) * vec4(ourColor, 1.0f);
	color = mix(texture(texture1, v_TextureCoord), texture(texture2, v_TextureCoord), blendFactor);
}