//
// Created by HUAWEI on 2024/1/20.
//

#ifndef GENIUS_INVOKATION_TCG_2_WORD_H
#define GENIUS_INVOKATION_TCG_2_WORD_H

#include "common.h"
#include "choice.h"

void Init_Text();
static void Draw_Text(SDL_Rect rect);
void Transform(char *hp_s,int hp);
void Text_Begin();
void Text_Preparartion();
void Text_Fighting();
void Text_Round();
void Text_Points();
void Text_HP(Role role);
void Text_Energy(Role role);
void Text_Energy_All();
void Text_HP_Summons(Summons summon);
void Text_HP_All();
void Text_Win();
void Text_Lose();
void Text_Reaction(int debuff);

void Text_Normal_Attack_0();
void Text_Normal_Attack_1();
void Text_Elemental_Skill();
void Text_Elemental_Skill_1();
void Text_Elemental_Skill_2();
void Text_Elemental_Burst_0();
void Text_Elemental_Burst_1();

void Show_Text();

#endif //GENIUS_INVOKATION_TCG_2_WORD_H
