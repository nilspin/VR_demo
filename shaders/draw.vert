#version 430

in vec2 texCoords;
out vec2 UV;
in vec3 position;
//out vec3 v_position;
uniform mat4 MVP;

float pi = 3.14159265358;
float R = 10.0;

void main()
{
    UV = texCoords;
    vec3 pos = position;
    float x = R*cos(pos.x*pi/R);// + pos.x;
    float y = pos.y;
    float z = pos.z - R*sin(pos.x*pi/R);
	gl_Position = MVP*vec4(pos, 1.0f);
//	gl_Position = MVP*vec4(x, y, z, 1.0f);
}