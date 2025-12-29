#include "utils/audio.h"

void Play_MP3()
{
    audio.music= Mix_LoadMUS("..\\..\\res\\music\\music.mp3");
    if (audio.music==NULL){
        SDL_Log("Play_MP3",Mix_GetError);
    }
    Mix_PlayMusic(audio.music,-1);
}