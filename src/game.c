#include "game.h"

Choice *choice;
int choice_i=0;
int isover=0;
int isround_0=0;
int isround_1=0;
int isact_1=0;
int selection=1;

void Do_Menu_Logic()
{
    Load_Cover();
    SDL_Event event;
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event)){
        Do_Event(event);

        Draw_Img();
        Text_Begin();
        SDL_RenderPresent(app.renderer);

        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
            return;
    }
}

void Do_Preparation_Logic()
{
    Init_Choice();
    Init_Roles();
    Init_Summons();

    Load_Preparation();
    SDL_Event event;
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event)){
        Do_Event(event);
        Draw_Img();
        Draw_Role(1);
        Draw_Role(2);
        Draw_Role(3);
        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT) {
            Choose_Role(event);
        }
        Draw_Chooceit();
        Text_Preparartion();
        SDL_RenderPresent(app.renderer);

        if (app.keyboard[SDL_SCANCODE_RETURN] && event.type==SDL_KEYDOWN
            && (roles[1].ischosen || roles[2].ischosen || roles[3].ischosen)){
            return;
        }
    }
}

void Do_Game_Logic()
{
    if(roles[choice_i+1].ischosen==false) {
        if (roles[choice_i + 2].ischosen == false)
            choice_i = 2;
        else
            choice_i = 1;
    }

    NEW_ROUND:;

    summons[0].isact=false;
    summons[1].isact=false;
    points_0=8;
    points_1=8;
    Load_Res();
    SDL_Event event;
    Draw_Refresh();
    SDL_RenderPresent(app.renderer);
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && !isover && (!isround_1||!isround_0) && SDL_WaitEvent(&event)){
        Judge_Win_Lose();
        isact_1=0;
        Do_Event(event);
        while (!app.keyboard[SDL_SCANCODE_ESCAPE] && !isact_1 && !isround_1 && !isover && SDL_WaitEvent(&event)) {
            Do_Event(event);
            Draw_Refresh();
            Draw_Introduce_All(event,&isview);
            Draw_Refresh();
            SDL_RenderPresent(app.renderer);
            Do_Choice(choice_i + 1);
            if (app.keyboard[SDL_SCANCODE_SPACE] && event.type==SDL_KEYDOWN){
                isround_1=1;
            }
        }
        Judge_Win_Lose();
        if (!isround_0 && !isover)
            Do_Maguu_Kenki();
        Draw_Refresh();
        Judge_Win_Lose();
        if (isover)
            return;
        SDL_RenderPresent(app.renderer);

        if (isround_1 && isround_0){
            Do_Round_Logic();
            isround_1=0;
            isround_0=0;
            goto NEW_ROUND;
        }
    }
}

void Do_Round_Logic()
{
    Summons_Attack();
    Skills_Settlement();
    SDL_Delay(500);
    for (int i=0;i<=3;i++){
        roles[i].debuff=no_debuff;
    }

    if (isover!=0){
        return;
    }
    Load_Round();
    SDL_Event event;
    Do_Event(event);
    Draw_Img();
    Text_Round();
    SDL_RenderPresent(app.renderer);
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event)){
        Do_Event(event);
        Draw_Img();
        Text_Round();
        SDL_RenderPresent(app.renderer);
        if (app.keyboard[SDL_SCANCODE_RETURN] && event.type==SDL_KEYDOWN) {
            return;
        }
    }
}


void Do_Over_Logic()
{
    Quit_Choice();
    Quit_Roles();
    Quit_Summons();
    SDL_Event event;
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event)){
        Do_Event(event);

        if (isover>0){
            Load_Win();
            Draw_Img();
            Text_Win();
        }
        else {
            Load_Lose();
            Draw_Img();
            Text_Lose();
        }
        SDL_RenderPresent(app.renderer);

        if (event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
            return;
    }
}


void Choose_Role(SDL_Event event)
{
    if (event.motion.x>=roles[1].x
        && event.motion.x<=roles[1].x+278
        && event.motion.y>=roles[1].y
        && event.motion.y<=roles[1].y+480){
        roles[1].ischosen=!roles[1].ischosen;
    } else if (event.motion.x>=roles[2].x
               && event.motion.x<=roles[2].x+278
               && event.motion.y>=roles[2].y
               && event.motion.y<=roles[2].y+480){
        roles[2].ischosen=!roles[2].ischosen;
    }else if (event.motion.x>=roles[3].x
              && event.motion.x<=roles[3].x+278
              && event.motion.y>=roles[3].y
              && event.motion.y<=roles[3].y+480) {
        roles[3].ischosen = !roles[3].ischosen;
    }
}

int Inflict(Role *src,Role *dst,int att)
{
    int inflict=Skills_Log(src,dst,att);
    inflict+=src->attack_riase;
    src->attack_riase=0;
    if (inflict-dst->defense_raise>0){
        dst->defense_raise=0;
        return inflict-dst->defense_raise;
    } else {
        dst->defense_raise-=inflict;
        return 0;
    }
}

void Judge_Win_Lose()
{
    if (roles[0].HP<=0){
        isover=1;
        return;
    }
    if ((roles[1].HP<=0 || !roles[1].ischosen)
        && (roles[2].HP<=0 || !roles[2].ischosen)
        && (roles[3].HP<=0 || !roles[3].ischosen)){
        isover=-1;
        return;
    }
}

void Skills_Settlement()
{
    if (summons[0].PP>0)
        summons[0].PP--;
    if(summons[1].PP>0)
        summons[1].PP--;
    if (gu_wu_ling_yu>0)
        gu_wu_ling_yu--;
    if (liu_jin_huo_guang>0)
        liu_jin_huo_guang--;
}



void Elemental_Reaction_Jugde(Role *src,Role *dst,int element)
{
    switch (element) {
        case fire:{
            switch (dst->element) {
                case fire:
                    return;
                case water:
                    Elemental_Reaction(src,dst,FW);
                    return;
                case thunder:
                    Elemental_Reaction(src,dst,FT);
                    return;
                case grass:
                    Elemental_Reaction(src,dst,FG);
                    return;
                case ice:
                    Elemental_Reaction(src,dst,FI);
                    return;
                default:
                    dst->element=fire;
                    return;
            }
        }
        case water:{
            switch (dst->element) {
                case fire:
                    Elemental_Reaction(src,dst,FW);
                    return;
                case water:
                    return;
                case thunder:
                    Elemental_Reaction(src,dst,WT);
                    return;
                case grass:
                    Elemental_Reaction(src,dst,WG);
                    return;
                case ice:
                    Elemental_Reaction(src,dst,WI);
                    return;
                default:
                    dst->element=water;
                    return;
            }
        }
        case thunder:{
            switch (dst->element) {
                case fire:
                    Elemental_Reaction(src,dst,FT);
                    return;
                case water:
                    Elemental_Reaction(src,dst,WT);
                    return;
                case thunder:
                    return;
                case grass:
                    Elemental_Reaction(src,dst,TG);
                    return;
                case ice:
                    Elemental_Reaction(src,dst,TI);
                    return;
                default:
                    dst->element=thunder;
                    return;
            }
        }
        case grass:{
            switch (dst->element) {
                case fire:
                    Elemental_Reaction(src,dst,FG);
                    return;
                case water:
                    Elemental_Reaction(src,dst,WG);
                    return;
                case thunder:
                    Elemental_Reaction(src,dst,TG);
                    return;
                case grass:
                    return;
                case ice:
                    return;
                default:
                    dst->element=grass;
                    return;
            }
        }
        case ice:{
            switch (dst->element) {
                case fire:
                    Elemental_Reaction(src,dst,FI);
                    return;
                case water:
                    Elemental_Reaction(src,dst,WI);
                    return;
                case thunder:
                    Elemental_Reaction(src,dst,TI);
                    return;
                case grass:
                    return;
                case ice:
                    return;
                default:
                    dst->element=ice;
                    return;
            }
        }
        case wind:{
            switch (dst->element) {
                case fire:
                    Elemental_Reaction(src,dst,WindF);
                    return;
                case water:
                    Elemental_Reaction(src,dst,WindW);
                    return;
                case thunder:
                    Elemental_Reaction(src,dst,WindT);
                    return;
                case grass:
                    return;
                case ice:
                    Elemental_Reaction(src,dst,WindI);
                    return;
                default:
                    return;
            }
        }
        case rock:{
            Elemental_Reaction(src,dst,RockAny);
        }
        default:
            return;

    }
}

void Elemental_Reaction(Role *src,Role *dst,int debuff)
{
    switch (debuff) {
        case FW:
            Text_Reaction(FW);
            SDL_RenderPresent(app.renderer);
            SDL_Delay(500);
            Draw_Refresh();
            src->attack_riase+=2;
            dst->element=no_element;
            return;
        case FT:
            return;
        case FG:
            return;
        case FI:
            return;
        case WT:
            return;
        case WG:
            return;
        case WI:
            Text_Reaction(WI);
            SDL_RenderPresent(app.renderer);
            SDL_Delay(500);
            Draw_Refresh();
            src->attack_riase+=1;
            dst->element=no_element;
            dst->debuff=WI;
            return;
        case TG:
            return;
        case TI:
            return;
        case WindF:
            return;
        case WindW:
            Text_Reaction(WindW);
            SDL_RenderPresent(app.renderer);
            SDL_Delay(500);
            Draw_Refresh();
            dst->element=no_element;
            for (int i=0;i<=2;i++){
                if (i!=choice_i){
                    Elemental_Reaction_Jugde(src,&roles[i+1],water);
                    roles[i+1].HP-=1;
                }
            }
            return;
        case WindT:
            return;
        case WindI:
            Text_Reaction(WindI);
            SDL_RenderPresent(app.renderer);
            SDL_Delay(500);
            Draw_Refresh();
            dst->element=no_element;
            for (int i=0;i<=2;i++){
                if (i!=choice_i){
                    Elemental_Reaction_Jugde(src,&roles[i+1],ice);
                    roles[i+1].HP-=1;
                }
            }
            return;
        case RockAny:
            return;
        default:
            return;

    }
}
