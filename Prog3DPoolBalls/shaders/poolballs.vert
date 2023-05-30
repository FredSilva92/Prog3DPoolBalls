#version 440 core

in vec3 position;
in vec3 normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;		// View * Model
uniform mat4 Projection;
uniform mat3 NormalMatrix;

void main()
{
    gl_Position = Projection * ModelView * vec4(position, 1.0);
}