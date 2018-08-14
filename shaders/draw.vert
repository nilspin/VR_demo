#version 430

//in vec2 texCoords;
//out vec2 texCoord;
in vec2 position;
uniform mat4 MVP;

void main()
{
//    texCoord = texCoords;
    vec3 pos = vec3(position, 0.0f);
	gl_Position = MVP*vec4(pos, 1.0f);
}