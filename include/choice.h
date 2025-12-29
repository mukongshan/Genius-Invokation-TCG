//
// Created by HUAWEI on 2024/1/21.
//

#ifndef GENIUS_INVOKATION_TCG_2_CHOICE_H
#define GENIUS_INVOKATION_TCG_2_CHOICE_H

#include "common.h"
#include "role.h"
#include "utils/input.h"
#include "utils/display.h"

void Init_Choice();
void Quit_Choice();
void Do_Choice(int sel);
void Next_Choice();
void Prev_Choice();
void Act_Choice(int sel);

typedef struct {
    int x,y;
    void (*action)();
}Choice;

extern Choice *choice;

#endif //GENIUS_INVOKATION_TCG_2_CHOICE_H
