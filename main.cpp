//Headers to be used.
#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <block.h>
#include <sound.h>

bool engineInit()
{
	//Initialize SDL library
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	//Initialize SDL_ttf
	if (TTF_Init() != 0)
	{
		SDL_Log("TTF_Init error: %s\n", TTF_GetError());
		SDL_Quit();
		return 1;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		SDL_Log("Mix_OpenAudio error: %s\n", Mix_GetError());
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	return 0;
}

void Close()
{
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}

//Main function.
int main(int argc, char** argv)
{
	//Start up.
	if (engineInit() != 0)
		return -1;
	
	//Create a window and its default renderer.
	Window window;

	//Load media.
	Uint32 time = SDL_GetTicks();
	SDL_Color white = { 255,255,255 };
	Texture background, nextBlock, tutorial, score, maxScore, gameover, pressR, pause, pressP, money;
	background.createFromImage(window, "rsc\\background.png");
	nextBlock.createFromText(window, "Next Block:", "rsc\\comicbd.ttf", white, 15);
	tutorial.createFromImage(window, "rsc\\tutorial.png");
	score.createFromText(window, "Score = 0", "rsc\\comicbd.ttf", white, 30);
	maxScore.createFromText(window, "Max Score = 0", "rsc\\comicbd.ttf", white, 30);
	gameover.createFromText(window, "Gameover!", "rsc\\comicbd.ttf", white, 30);
	pressR.createFromText(window, "Press R to restart the game.", "rsc\\comicbd.ttf", white, 20);
	pause.createFromText(window, "Paused", "rsc\\comicbd.ttf", white, 30);
	pressP.createFromText(window, "Press P to continue.", "rsc\\comicbd.ttf", white, 20);
	money.createFromImage(window, "rsc\\money.png");
	Block block(window);
	block.init();
	playMusic("rsc\\PlayingBGM.wav");

	//Event-driven loop.
	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		/*Handle events*/
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				quit = true;
			window.handleEvent(event);
			block.handleEvent(event);
		}
		/*Logic*/
		//If decided to fund.
		if (block.isMoney())
		{
			/*Render*/
			if (!window.isMinimized())
			{
				SDL_RenderClear(window.rend);
				money.render(window, 0, 0);
				SDL_RenderPresent(window.rend);
			}
			continue;
		}
		//If the game has not started.
		if (!block.isStart())
		{
			/*Render*/
			if (!window.isMinimized())
			{
				SDL_RenderClear(window.rend);
				tutorial.render(window, 0, 0);
				SDL_RenderPresent(window.rend);
			}
		}
		//If the game has started.
		else
		{
			//If the game is not over.
			if (!block.isGameover())
			{
				//If the game is paused.
				if (block.isPaused())
				{
					/*Render*/
					if (!window.isMinimized())
					{
						SDL_RenderClear(window.rend);
						background.render(window, 0, 0);
						nextBlock.render(window, 10, 160);
						score.render(window, WINDOW_WIDTH / 2 - score.getWidth() / 2, 50);
						maxScore.render(window, WINDOW_WIDTH / 2 - maxScore.getWidth() / 2, 25);
						pause.render(window, WINDOW_WIDTH / 2 - pause.getWidth() / 2, 520);
						pressP.render(window, WINDOW_WIDTH / 2 - pressP.getWidth() / 2, 560);
						block.draw(window);
						block.drawNextBlock(window);
						block.drawPile(window);
						SDL_RenderPresent(window.rend);
					}
				}
				//If the game is not paused.
				else
				{
					//If the block is dead.
					if (!block.isAlive())
					{
						block.copyToPile();
						block.init();
					}
					//If the block is alive.
					else
					{
						block.move();
						if (SDL_GetTicks() - time >= AUTODROP_INTERVAL)
						{
							time = SDL_GetTicks();
							block.autoDrop();
						}
					}
					score.createFromText(window, "Score = " + std::to_string(block.getScore()), "rsc\\comicbd.ttf", white, 30);
					maxScore.createFromText(window, "Max Score = " + std::to_string(block.getMaxScore()), "rsc\\comicbd.ttf", white, 18);
					/*Render*/
					if (!window.isMinimized())
					{
						SDL_RenderClear(window.rend);
						background.render(window, 0, 0);
						nextBlock.render(window, 10, 160);
						score.render(window, WINDOW_WIDTH / 2 - score.getWidth() / 2, 50);
						maxScore.render(window, WINDOW_WIDTH / 2 - maxScore.getWidth() / 2, 25);
						block.draw(window);
						block.drawNextBlock(window);
						block.drawPile(window);
						SDL_RenderPresent(window.rend);
					}
				}
			}
			//If the game is over.
			else
			{
				/*Render*/
				if (!window.isMinimized())
				{
					SDL_RenderClear(window.rend);
					background.render(window, 0, 0);
					score.render(window, WINDOW_WIDTH / 2 - score.getWidth() / 2, 50);
					maxScore.render(window, WINDOW_WIDTH / 2 - maxScore.getWidth() / 2, 25);
					gameover.render(window, WINDOW_WIDTH / 2 - gameover.getWidth() / 2, 520);
					pressR.render(window, WINDOW_WIDTH / 2 - pressR.getWidth() / 2, 560);
					block.drawPile(window);
					SDL_RenderPresent(window.rend);
				}
			}
		}
		SDL_Delay(50);
	}

	//Free resources and end process.
	money.free();
	pressP.free();
	pause.free();
	pressR.free();
	gameover.free();
	maxScore.free();
	score.free();
	tutorial.free();
	nextBlock.free();
	background.free();
	window.free();
	Close();
	system("pause");
	return 0;
}