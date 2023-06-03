#version 440 core

//in vec3 color;
layout (location = 0) in vec3 color;			// cor do vértice
layout (location = 1) in vec2 textureCoord;		// coordenada de textura do vértice

layout (location = 0) out vec4 fColor;			// cor final do fragmento

uniform sampler2D TexSampler;					// Sampler de textura

void main()
{
	fColor = texture(TexSampler, textureCoord);
	//fColor = vec4(color, 1.0f);
}