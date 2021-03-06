#include "Window.h"

void Window::swap()
{
	SDL_GL_SwapWindow(window);
}

Window::Window()
{
	Initialize();
}


Window::~Window()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

bool Window::Initialize()
{
	start = 0;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	window = SDL_CreateWindow("VR_demo", 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_SetWindowPosition(window, 0,0);
	context = SDL_GL_CreateContext(window);

	if (!gladLoadGL()) {
		throw std::runtime_error("GLAD initialization failed");
	}

	return true;
}
