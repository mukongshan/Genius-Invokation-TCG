//
// Created by HUAWEI on 2024/1/19.
//

#ifndef GENIUS_INVOKATION_TCG_2_GAME_H
#define GENIUS_INVOKATION_TCG_2_GAME_H

#include "common.h"
#include "utils/input.h"
#include "utils/display.h"
#include "choice.h"
#include "role.h"

#define NUM_ROLES 3
#define NUM_MEMBERS 4
#define NUM_SUMMONS 2



void Do_Menu_Logic();
void Do_Preparation_Logic();
void Do_Game_Logic();
void Do_Round_Logic();
void Do_Over_Logic();

void Choose_Role(SDL_Event event);
int Inflict(Role *src,Role *dst,int att);
void Judge_Win_Lose();
void Skills_Settlement();

void Elemental_Reaction_Jugde(Role *src,Role *dst,int element);
void Elemental_Reaction(Role *src,Role *dst,int debuff);




#endif //GENIUS_INVOKATION_TCG_2_GAME_H
