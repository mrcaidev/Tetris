#include <texture.h>

/*
 * \brief Create an empty texture.
 */
Texture::Texture()
{
	texture = NULL;
	w = 0;
	h = 0;
}

/*
 * \brief Deallocate the texture.
 */
Texture::~Texture()
{
	free();
}

/*
 * \brief Load an image directly into a texture.
 * \param renderer The renderer which should copy parts of a texture.
 * \param file The path of the source image.
 */
void Texture::createFromImage(Window window, const char* file)
{
	free();
	//Create the texture.
	texture = IMG_LoadTexture(window.rend, file);
	if (texture == NULL)
		SDL_Log("IMG_LoadTexture error: %s", SDL_GetError());
	else
		//Get the width and height of the texture.
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

/*
 * \brief Create a texture from a string of message.
 * \param renderer The renderer which should copy parts of a texture.
 * \param message The string of source message.
 * \param file The path of the source font.
 * \param color The color of text.
 * \param size The size of text.
 */
void Texture::createFromText(Window window, std::string message, const char* file, SDL_Color color, int size)
{
	free();
	//Open the file of font.
	TTF_Font* font = TTF_OpenFont(file, size);
	if (font == NULL)
		SDL_Log("TTF_OpenFont error: %s", TTF_GetError());
	else
	{
		//Load the message into a surface.
		SDL_Surface* surface = TTF_RenderText_Blended(font, message.c_str(), color);
		//Close the file of font.
		TTF_CloseFont(font);
		if (surface == NULL)
			SDL_Log("TTF_RenderText_Blended error: %s", SDL_GetError());
		else
		{
			//Create the texture from surface.
			texture = SDL_CreateTextureFromSurface(window.rend, surface);
			//Free the surface.
			SDL_FreeSurface(surface);
			surface = NULL;
			if (texture == NULL)
				SDL_Log("SDL_CreateTextureFromSurface: %s", SDL_GetError());
			else
				//Get the width and height of the texture.
				SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		}
	}
}

/*
 * \brief Copy the texture to the renderer.
 * \param renderer The renderer which should copy a texture.
 * \param x, y The destination coordinate to copy the texture.
 */
void Texture::render(Window window, int x, int y)
{
	SDL_Rect viewport = { x,y,w,h };
	SDL_RenderCopy(window.rend, texture, NULL, &viewport);
}

/*
 * \brief Get the width of a texture.
 * \return The width of a texture.
 */
int Texture::getWidth()
{
	return w;
}

/*
 * \brief Get the height of a texture.
 * \return The height of a texture.
 */
int Texture::getHeight()
{
	return h;
}

/*
 * \brief Deallocate the texture.
 */
void Texture::free()
{
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		w = 0;
		h = 0;
	}
}