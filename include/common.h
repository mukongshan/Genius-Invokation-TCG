//
// Created by HUAWEI on 2024/1/19.
//

#ifndef CODE_COMMON_H
#define CODE_COMMON_H

#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

#define HANDLE_ERROR(msg) printf(msg": %s\n",SDL_GetError()); \
    exit(EXIT_FAILURE)

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *img_surf;
    SDL_Surface *txt_surf;
    TTF_Font *font;
    bool *keyboard;
}App;

typedef struct {
    int x;
    int y;
    int w;
    int h;
}Win;

typedef struct {
    Mix_Music *music;
}Audio;

typedef struct {
    SDL_Surface *surf_0;
    SDL_Surface *surf_1;
    SDL_Surface *surf_2;
    SDL_Surface *surf_3;
    SDL_Surface *surf_other;
}Game;

typedef enum Debuff
{
    no_debuff,FW,FT,FI,FG,WT,WI,WG,TI,TG,WindF,WindW,WindT,WindI,RockAny,
}Debuff;

typedef enum Element_type
{
    no_element,fire,water,thunder,grass,ice,wind,rock,
}Element_type;

typedef struct Role
{
    bool ischosen;
    bool isfighting;
    int team;
    int num;
    int HP;
    int energy;
    int element;
    int debuff;
    int defense_raise;
    int attack_riase;
    int x;
    int y;
}Role;

typedef struct Summons
{
    int x;
    int y;
    int PP;
    bool isact;
}Summons;

extern App app;
extern Win win;
extern Audio audio;
extern Game game;
extern int choice_i;
extern int isover;
extern int isround_0;
extern int isround_1;
extern int isact_1;
extern int points_0;
extern int points_1;
extern int isview;
extern int isswitch;
extern int selection;


#endif //CODE_COMMON_H
