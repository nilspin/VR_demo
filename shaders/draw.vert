#version 430

//in vec2 texCoords;
//out vec2 texCoord;
in vec2 position;
uniform mat4 MVP;

float pi = 3.14159265358;
float R = 100.0;

void main()
{
//    texCoord = texCoords;
    vec3 pos = vec3(position, 1.0f);
//    float x = R*cos(pos.x*2*pi/200);
//    float y = R*sin(pos.x*2*pi/200);
//    float z = pos.z*300/480;
//	gl_Position = MVP*vec4(x,y,z, 1.0f);
	gl_Position = MVP*vec4(pos, 1.0f);
}