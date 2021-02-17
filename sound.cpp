#include <sound.h>

void playMusic(const char* file)
{
	Mix_Music* music = Mix_LoadMUS(file);
	if (music == NULL)
	{
		SDL_Log("Mix_LoadMUS error: %s", Mix_GetError());
		return;
	}
	Mix_PlayMusic(music, -1);
}

void playChunk(const char* file)
{
	Mix_Chunk* chunk = Mix_LoadWAV(file);
	if (chunk == NULL)
	{
		SDL_Log("Mix_LoadWAV error: %s", Mix_GetError());
		return;
	}
	Mix_PlayChannel(-1, chunk, 0);
}