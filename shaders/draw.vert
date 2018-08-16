#version 430

in vec2 texCoords;
out vec2 UV;
in vec3 position;
//out vec3 v_position;
uniform mat4 MVP;

float pi = 3.14159265358;
float dist = 5;
float R = 5.0f;
void main()
{
    UV = texCoords;
    vec3 pos = position;
    float x = R*sin(pos.x*(0.5)*pi/R);// ;//
    float y = pos.y;
    float z = -R*cos(pos.x*(0.5)*pi/R);//+ pos.z;// 
//	gl_Position = MVP*vec4(pos, 1.0f);
	gl_Position = MVP*vec4(x, y, z, 1.0f);
}