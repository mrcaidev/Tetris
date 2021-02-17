#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <global.h>

//Window wrapper class
class Window
{
private:
	SDL_Window* window;
	int w, h;
	bool minimized;
public:
	SDL_Renderer* rend;
	Window();
	void handleEvent(SDL_Event event);
	int getWidth();
	int getHeight();
	bool isMinimized();
	void free();
};


#endif // !WINDOW_H
