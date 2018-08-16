#ifndef APPLICATION_H
#define APPLICATION_H

#include "prereq.h"
#include "Window.h"
#include "ShaderProgram.hpp"
#include "camera.h"
#include<cmath>

using namespace std;
using namespace glm;

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

class Application
{
public:
	void run();
	Application();
	void setupShaders();
	void setupBuffers();
	void setupTextures();
	void processEvents();
	void draw();
private:
	//Dims
	int imgWidth = 0;
	int imgHeight = 0;
	int channels = 0;
	float wide = 0.1;
	int bufferHeight = 12;
	int bufferWidth = 12;

	//Window and events
	Window window;
	//SDL_Event e;
	
	bool quit=false;
	bool isLeft = true;	//flag to alternate between left and right textures

	//Camera & transforms
	Camera cam1, cam2;
	glm::mat4 model = glm::mat4(1);
	glm::mat4 view_cam1 = glm::mat4(1);
	glm::mat4 view_cam2 = glm::mat4(1);
	glm::mat4 proj = glm::perspective(80.0f, 1.0f, 0.1f, 1000.0f);
	glm::mat4 MVP_cam1 = glm::mat4(1);
	glm::mat4 MVP_cam2 = glm::mat4(1);

	//Shaders
	unique_ptr<ShaderProgram> drawShader;// Draws regular quad
	unique_ptr<ShaderProgram> cylinderTransformShader;//Draws cylinder projection
	unique_ptr<ShaderProgram> barrelProjShader;//Draws barrel projection
	
	
	//Texture & images
	GLuint leftTex, rightTex;
	u_char *left=nullptr;
	u_char *right=nullptr;
	
	//OpenGL Buffer objects
	vector<glm::vec3>	rectangle;
	vector<glm::vec2> texCoords;
	vector<uint> indices;
	vector<uint> realIndices;
	GLuint drawVBO;	//buffer to store rectangle coords
	GLuint texCoordVBO;	//buffer to hold texture coordinates
	GLuint indexVBO;
	GLuint drawVAO;	//vertex array object

};

#endif //APPLICATION_H
