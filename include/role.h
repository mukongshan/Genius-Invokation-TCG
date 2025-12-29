//
// Created by HUAWEI on 2024/1/21.
//

#ifndef GENIUS_INVOKATION_TCG_2_ROLE_H
#define GENIUS_INVOKATION_TCG_2_ROLE_H

#include "common.h"
#include "utils/display.h"
#include "utils/word.h"
#include "utils/input.h"

extern int gu_wu_ling_yu;
extern int yu_lian_jian;
extern int hong_shi_jian;
extern int ting_huo_yan_xiao;
extern int liu_jin_huo_guang;

extern Role *roles;
extern Summons *summons;

void Init_Roles();
void Init_Summons();
void Quit_Roles();
void Quit_Summons();

int Skills_Log(Role *src,Role *dst,int att);
void After_Normal_Attack_Enhencement();
void After_Skill_Enhencement();
void After_Ultimate_Enhencement();
void Summons_Attack();
void Role_Fail();

void Do_Bennett();
void Bennett_Attack();
void Bennett_Skill();
void Bennett_Ultimate();
void Gu_Wu_Ling_Yu(Role *role);

void Do_Xingqiu();
void Xingqiu_Attack();
void Xingqiu_Skill();
void Xingqiu_Ultimate();
int Yu_Lian_Jian(int att);
void Hong_Shi_Jian();

void Do_Yoimiya();
void Yoimiya_Attack();
void Yoimiya_Skill();
void Yoimiya_Ultimate();
int Ting_Huo_Yan_Xiao(Role *src,Role *dst,int att);
void Liu_Jin_Huo_Guang();

void Do_Maguu_Kenki();
void Maguu_Kenki_Attack();
void Maguu_Kenki_Skill1();
void Maguu_Kenki_Skill2();
void Maguu_Kenki_Ultimate();




#endif //GENIUS_INVOKATION_TCG_2_ROLE_H
