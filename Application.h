#ifndef APPLICATION_H
#define APPLICATION_H

#include "prereq.h"
#include "Window.h"
#include "ShaderProgram.hpp"
#include "camera.h"

using namespace std;
using namespace glm;

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

class Application
{
public:
	void run();
	Application();
	~Application();
	void setupShaders();
	void setupBuffers();
	void setupTextures();
	void processEvents();
private:
	//Dims
	int imgWidth = 0;
	int imgHeight = 0;
	int channels = 0;
	float wide = 0.1;
	const int bufferHeight = 10;
	const int bufferWidth = 20;

	//Window and events
	Window window;
	//SDL_Event e;
	
	bool quit=false;

	//Camera & transforms
	Camera cam;
	glm::mat4 model = glm::mat4(1);
	glm::mat4 view = glm::mat4(1);
	glm::mat4 proj = glm::perspective(80.0f, 1.0f, 0.1f, 1000.0f);
	glm::mat4 MVP = glm::mat4(1);

	//Shader
	unique_ptr<ShaderProgram> drawShader;
	
	
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
