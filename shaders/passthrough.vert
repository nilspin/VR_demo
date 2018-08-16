#version 430

in vec2 texCoords;
out vec2 UV;
in vec3 position;
uniform mat4 MVP;

void main()
{
    UV = texCoords;
	gl_Position = MVP*vec4(position, 1.0f);
}