#version 440 core

in vec3 color;

layout (location = 0) out vec4 fColor;   // cor final do fragmento

void main()
{
	//fColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	fColor = vec4(color, 1.0f);
}