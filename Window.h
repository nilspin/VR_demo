#include "prereq.h"

class Window
{
public:
	void swap();
	Window();
	~Window();
private:
	SDL_GLContext context;
	const uint windowHeight = 960;
	const uint windowWidth = 1920;
	uint32_t start = 0;
	SDL_Window* window = NULL;
	bool Initialize();
};

