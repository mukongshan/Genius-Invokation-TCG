#include "choice.h"

int isswitch=0;


void Init_Choice()
{
    choice = malloc(NUM_ROLES*sizeof(Choice));
    choice[0]=(Choice){824,1500,Do_Bennett};
    choice[1]=(Choice){1224,1500,Do_Xingqiu};
    choice[2]=(Choice){1624,1500,Do_Yoimiya};
}
void Quit_Choice()
{
    free(choice);
}
void Do_Choice(int sel)
{
        if (roles[selection].isfighting){
            Act_Choice(selection);
        }


    if (app.keyboard[SDL_SCANCODE_LEFT]) {
        Prev_Choice();
        Load_Fighting();
        Draw_Img();
    }else if(app.keyboard[SDL_SCANCODE_RIGHT]){
        Next_Choice();
        Load_Fighting();
        Draw_Img();
    } else if (app.keyboard[SDL_SCANCODE_RETURN] && roles[choice_i+1].HP>0) {
        if (isswitch==0 || selection==sel){
            isswitch=1;
            selection=sel;
            roles[selection].isfighting=true;
            Act_Choice(sel);
        } else if (selection!=sel){
            points_1--;
            selection=sel;
            roles[selection].isfighting=true;
            Act_Choice(sel);
        }
    }
}
void Next_Choice()
{
    if (choice_i<2) {
        if (roles[choice_i+2].HP>0 && roles[choice_i+2].ischosen==true)
        choice_i++;
        else if (choice_i<=0 && roles[3].ischosen==true && roles[3].HP>0)
            choice_i+=2;
    }
}
void Prev_Choice()
{
    if (choice_i>0) {
        if (roles[choice_i].HP>0 && roles[choice_i].ischosen==true)
            choice_i--;
        else if (choice_i>=2 && roles[1].ischosen==true && roles[1].HP>0)
            choice_i-=2;
    }
}
void Act_Choice(int sel)
{
    void (*action)()=choice[choice_i].action;
    if (action && roles[sel].HP>0 && roles[sel].debuff==no_debuff){
    action();
    }
}