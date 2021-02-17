#include <window.h>

/*
 * \brief Create an empty window.
 */
Window::Window()
{
	free();
	//Create a window.
	window = SDL_CreateWindow("Tetris Game",
							  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  WINDOW_WIDTH, WINDOW_HEIGHT,
							  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
		SDL_Log("SDL_CreateWindow error: %s\n", SDL_GetError());
	else
	{
		this->w = w;
		this->h = h;
		minimized = false;
		//Create a renderer.
		rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (rend == NULL)
		{
			SDL_DestroyWindow(window);
			window = NULL;
			SDL_Log("SDL_CreateRenderer error: %s\n", SDL_GetError());
		}
		else
			SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	}
}

/*
 * \brief Handle window events.
 * \param event If not NULL, the next event is removed from the queue and stored in that area.
 */
void Window::handleEvent(SDL_Event event)
{
	if (event.type == SDL_WINDOWEVENT)
	{
		switch (event.window.event)
		{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				w = event.window.data1;
				h = event.window.data2;
				SDL_RenderPresent(rend);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_RenderPresent(rend);
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				minimized = true;
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				minimized = false;
				break;
			case SDL_WINDOWEVENT_RESTORED:
				minimized = false;
				break;
		}
	}
}

/*
 * \brief Get the width of the window.
 * \return The width of the window.
 */
int Window::getWidth()
{
	return w;
}

/*
 * \brief Get the height of the window.
 * \return The height of the window.
 */
int Window::getHeight()
{
	return h;
}

/*
 * \brief Determine if the window has been minimized.
 * \return 1 if minimized, or 0 if not minimized.
 */
bool Window::isMinimized()
{
	return minimized;
}

/*
 * \brief Free the window.
 */
void Window::free()
{
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
		window = NULL;
		SDL_DestroyRenderer(rend);
		rend = NULL;
		w = 0;
		h = 0;
		minimized = false;
	}
}