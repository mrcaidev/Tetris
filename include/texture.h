#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <window.h>

class Texture
{
protected:
	SDL_Texture* texture;
	int w, h;
public:
	Texture();
	~Texture();
	void createFromImage(Window window, const char* file);
	void createFromText(Window window, std::string message, const char* file, SDL_Color color, int size);
	void render(Window window, int x, int y);
	int getWidth();
	int getHeight();
	void free();
};


#endif // !TEXTURE_H
