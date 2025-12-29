//
// Created by HUAWEI on 2024/1/19.
//

#ifndef GENIUS_INVOKATION_TCG_2_INPUT_H
#define GENIUS_INVOKATION_TCG_2_INPUT_H

#include "common.h"

void Do_Event(SDL_Event event);
static void Do_Keydown(SDL_KeyboardEvent key);
static void Do_Keyup(SDL_KeyboardEvent key);
static void Do_Quit(SDL_QuitEvent quit);
static void Do_Mousebuttondown(SDL_MouseButtonEvent button);
static void Do_Mousebuttonup(SDL_MouseButtonEvent button);

#endif //GENIUS_INVOKATION_TCG_2_INPUT_H
