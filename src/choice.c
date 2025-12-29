#include "choice.h"
#include "game_state.h"
#include "utils/layout.h"

int isswitch=0;

void Init_Choice()
{
    choice = malloc(NUM_ROLES*sizeof(Choice));
    
    // 使用布局系统获取角色位置，然后计算选择标记位置
    RolePosition pos1 = Layout_GetRolePosition(1);
    RolePosition pos2 = Layout_GetRolePosition(2);
    RolePosition pos3 = Layout_GetRolePosition(3);
    
    // 选择标记在角色下方
    choice[0]=(Choice){pos1.x + pos1.width/2 - 37, pos1.y + pos1.height + 20, Do_Bennett};
    choice[1]=(Choice){pos2.x + pos2.width/2 - 37, pos2.y + pos2.height + 20, Do_Xingqiu};
    choice[2]=(Choice){pos3.x + pos3.width/2 - 37, pos3.y + pos3.height + 20, Do_Yoimiya};
}
void Quit_Choice()
{
    free(choice);
}
void Do_Choice(int sel)
{
    int selection = GameState_GetSelectedRole();
    int choice_i = GameState_GetChoiceIndex();
    
    if (roles[selection].isfighting){
        Act_Choice(selection);
    }

    if (app.keyboard[SDL_SCANCODE_LEFT]) {
        Prev_Choice();
        // 使用资源管理器
        SDL_Surface* bg = Resource_Get(RES_BACKGROUND_FIGHTING);
        if (bg != NULL) {
            app.img_surf = bg;
        } else {
            Load_Fighting();  // 向后兼容
        }
        Draw_Img();
    }else if(app.keyboard[SDL_SCANCODE_RIGHT]){
        Next_Choice();
        SDL_Surface* bg = Resource_Get(RES_BACKGROUND_FIGHTING);
        if (bg != NULL) {
            app.img_surf = bg;
        } else {
            Load_Fighting();  // 向后兼容
        }
        Draw_Img();
    } else if (app.keyboard[SDL_SCANCODE_RETURN] && roles[choice_i+1].HP>0) {
        if (!GameState_HasSwitchedRole() || selection==sel){
            GameState_SetHasSwitchedRole(true);
            isswitch = 1;
            selection = sel;
            GameState_SetSelectedRole(selection);
            roles[selection].isfighting=true;
            Act_Choice(sel);
        } else if (selection!=sel){
            GameState_ConsumePlayerPoints(1);
            points_1--;
            selection = sel;
            GameState_SetSelectedRole(selection);
            roles[selection].isfighting=true;
            Act_Choice(sel);
        }
    }
}
void Next_Choice()
{
    int choice_i = GameState_GetChoiceIndex();
    if (choice_i<2) {
        if (roles[choice_i+2].HP>0 && roles[choice_i+2].ischosen==true) {
            choice_i++;
            GameState_SetChoiceIndex(choice_i);
        }
        else if (choice_i<=0 && roles[3].ischosen==true && roles[3].HP>0) {
            choice_i+=2;
            GameState_SetChoiceIndex(choice_i);
        }
    }
}
void Prev_Choice()
{
    int choice_i = GameState_GetChoiceIndex();
    if (choice_i>0) {
        if (roles[choice_i].HP>0 && roles[choice_i].ischosen==true) {
            choice_i--;
            GameState_SetChoiceIndex(choice_i);
        }
        else if (choice_i>=2 && roles[1].ischosen==true && roles[1].HP>0) {
            choice_i-=2;
            GameState_SetChoiceIndex(choice_i);
        }
    }
}
void Act_Choice(int sel)
{
    void (*action)()=choice[choice_i].action;
    if (action && roles[sel].HP>0 && roles[sel].debuff==no_debuff){
    action();
    }
}