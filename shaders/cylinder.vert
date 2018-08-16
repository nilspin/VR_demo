#version 430

in vec2 texCoords;
out vec2 UV;
in vec3 position;
uniform mat4 MVP;

float fov = 80;
float pi = 3.14159265358;
float dist = 5;
float R = dist + (dist/tan(radians(fov/2)));//11.0f;
void main()
{
    UV = texCoords;
    vec3 pos = position;
    float x = R*sin(pos.x*(0.5)*pi/R);//in Cylindrical coordinates
    float y = pos.y*2.039;  //Adjust height to fit screen
    float z = -R*cos(pos.x*(0.5)*pi/R);//in Cylindrical coordinates
	gl_Position = MVP*vec4(x, y, z, 1.0f);
}