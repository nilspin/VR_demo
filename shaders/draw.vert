#version 430

in vec2 texCoords;
out vec2 texCoord;
in vec3 position;
//out vec3 v_position;
uniform mat4 MVP;

float pi = 3.14159265358;
float R = 30.0;

void main()
{
    texCoord = texCoords;
    vec3 pos = position;
    float x = R*cos(pos.x*1.9*pi/30);
    float y = pos.y;
    float z = R*sin(pos.x*1.9*pi/30) ;
//	gl_Position = MVP*vec4(pos, 1.0f);
	gl_Position = MVP*vec4(x, y, z, 1.0f);
}