#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>

void playMusic(const char* file);
void playChunk(const char* file);


#endif // !SOUND_H
