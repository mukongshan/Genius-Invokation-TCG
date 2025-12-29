//
// Created by HUAWEI on 2024/1/19.
//

#ifndef GENIUS_INVOKATION_TCG_2_DISPLAY_H
#define GENIUS_INVOKATION_TCG_2_DISPLAY_H

#include "common.h"
#include "game.h"
#include "role.h"
#include "utils/input.h"
#include "utils/word.h"
#include "utils/display.h"

void Draw_Refresh();
void Draw_Img();
void Draw_Role(int num);
void Draw_Roles_All();
void Draw_Choice();
void Draw_Element(Role role);
void Draw_Element_Img(Role role);
void Draw_Element_All();
void Draw_Debuff(Role role);
void Draw_Debuff_All();
void Draw_Chooceit();

void Draw_Skill(int i);
void Draw_Skill_Log();
void Draw_GuWuLingYu();
void Draw_YuLianJian();
void Draw_HongShiJian();
void Draw_LiuJinHuoGuang();
void Draw_TingHuoYanXiao();
void Draw_GuFeng();
void Draw_Shuangchi();
void Draw_Introduce_All(SDL_Event event,int *view);
void Draw_Introduce(int i);
void Draw_Xingdongfang();

void Load_Cover();
void Load_Preparation();
void Load_Fighting();
void Load_Round();
void Load_Win();
void Load_Lose();

void Load_Bennett();
void Load_Xingqiu();
void Load_Yoimiya();
void Load_Maguu_Kenki();
void Load_Bennett_jieshao();
void Load_Xingqiu_jieshao();
void Load_Yoimiya_jieshao();
void Load_Maguu_Kenki_jieshao();

void Load_Choice();
void Load_Chooceit();
void Load_Xingdong();
void Load_Res();


void Load_Bow_Attack();
void Load_Sword_Attack();
void Load_Skill_1();
void Load_Skill_2();
void Load_Skill_3();
void Load_Ultimate_1();
void Load_Ultimate_2();
void Load_Ultimate_3();
void Load_GuWuLingYu();
void Load_YuLianJian();
void Load_HongShiJian();
void Load_LiuJinHuoGuang();
void Load_TingHuoYanXiao();
void Load_GuFeng();
void Load_ShuangChi();


void Load_Nothing();
void Load_Fire();
void Load_Water();
void Load_Ice();
void Load_Freeze();

void Vedio_Common();
void Vedio_Attack_1(Role *role);
void Vedio_Attack_0(int *y);



#endif //GENIUS_INVOKATION_TCG_2_DISPLAY_H
