#include "game.h"
#include "game_state.h"
#include "elemental_reaction.h"

Choice *choice;

// 向后兼容：保留全局变量，但使用状态管理器
// 这些变量现在通过GameState_*函数访问
int choice_i=0;
int isover=0;
int isround_0=0;
int isround_1=0;
int isact_1=0;
int selection=1;

// 同步全局变量到状态管理器（用于向后兼容）
static void Sync_Global_State(void) {
    choice_i = GameState_GetChoiceIndex();
    isover = GameState_GetGameResult();
    isround_0 = GameState_IsRoundEnded() ? 1 : 0;
    isround_1 = GameState_IsPlayerActionDone() ? 1 : 0;
    isact_1 = GameState_IsPlayerActionDone() ? 1 : 0;
    selection = GameState_GetSelectedRole();
}

void Do_Menu_Logic()
{
    // 使用资源管理器
    SDL_Surface* bg = Resource_Get(RES_BACKGROUND_COVER);
    if (bg != NULL) {
        app.img_surf = bg;
    } else {
        Load_Cover();  // 向后兼容
    }
    
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
    GameState_Init();  // 初始化游戏状态
    Init_Choice();
    Init_Roles();
    Init_Summons();

    // 使用资源管理器
    SDL_Surface* bg = Resource_Get(RES_BACKGROUND_PREPARATION);
    if (bg != NULL) {
        app.img_surf = bg;
    } else {
        Load_Preparation();  // 向后兼容
    }
    
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
            GameState_SetState(GAME_STATE_FIGHTING);
            return;
        }
    }
}

void Do_Game_Logic()
{
    // 同步状态
    Sync_Global_State();
    
    if(roles[choice_i+1].ischosen==false) {
        if (roles[choice_i + 2].ischosen == false)
            choice_i = 2;
        else
            choice_i = 1;
        GameState_SetChoiceIndex(choice_i);
    }

    NEW_ROUND:;
    
    // 使用状态管理器初始化回合
    GameState_StartNewRound();
    Sync_Global_State();

    summons[0].isact=false;
    summons[1].isact=false;
    GameState_SetPlayerPoints(8);
    GameState_SetEnemyPoints(8);
    points_0 = 8;
    points_1 = 8;
    Load_Res();
    SDL_Event event;
    Draw_Refresh();
    SDL_RenderPresent(app.renderer);
    
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && !GameState_IsGameOver() && !GameState_IsRoundEnded() && SDL_WaitEvent(&event)){
        Judge_Win_Lose();
        GameState_SetPlayerActionDone(false);
        isact_1 = 0;
        Do_Event(event);
        
        while (!app.keyboard[SDL_SCANCODE_ESCAPE] && !GameState_IsPlayerActionDone() && !GameState_IsRoundEnded() && !GameState_IsGameOver() && SDL_WaitEvent(&event)) {
            Do_Event(event);
            Draw_Refresh();
            int isview_temp = GameState_IsViewingIntro() ? 1 : 0;
            Draw_Introduce_All(event, &isview_temp);
            GameState_SetViewingIntro(isview_temp != 0);
            Draw_Refresh();
            SDL_RenderPresent(app.renderer);
            Do_Choice(choice_i + 1);
            if (app.keyboard[SDL_SCANCODE_SPACE] && event.type==SDL_KEYDOWN){
                GameState_EndPlayerRound();
                isround_1 = 1;
            }
            Sync_Global_State();
        }
        
        Judge_Win_Lose();
        if (!GameState_IsRoundEnded() && !GameState_IsGameOver())
            Do_Maguu_Kenki();
        Draw_Refresh();
        Judge_Win_Lose();
        if (GameState_IsGameOver())
            return;
        SDL_RenderPresent(app.renderer);

        if (GameState_IsRoundEnded()){
            Do_Round_Logic();
            goto NEW_ROUND;
        }
        Sync_Global_State();
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

    if (GameState_IsGameOver()){
        return;
    }
    
    // 使用资源管理器加载背景
    SDL_Surface* bg = Resource_Get(RES_BACKGROUND_ROUND);
    if (bg != NULL) {
        app.img_surf = bg;
    } else {
        Load_Round();  // 向后兼容
    }
    
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
    
    // 使用资源管理器加载结果背景
    SDL_Surface* bg = NULL;
    if (GameState_GetGameResult() > 0) {
        bg = Resource_Get(RES_BACKGROUND_WIN);
        if (bg == NULL) Load_Win();
        else app.img_surf = bg;
    } else {
        bg = Resource_Get(RES_BACKGROUND_LOSE);
        if (bg == NULL) Load_Lose();
        else app.img_surf = bg;
    }
    
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event)){
        Do_Event(event);

        if (GameState_GetGameResult() > 0){
            if (app.img_surf != bg) {
                bg = Resource_Get(RES_BACKGROUND_WIN);
                if (bg == NULL) Load_Win();
                else app.img_surf = bg;
            }
            Draw_Img();
            Text_Win();
        }
        else {
            if (app.img_surf != bg) {
                bg = Resource_Get(RES_BACKGROUND_LOSE);
                if (bg == NULL) Load_Lose();
                else app.img_surf = bg;
            }
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
    // 使用布局系统获取实际角色大小
    RolePosition pos1 = Layout_GetRolePosition(1);
    RolePosition pos2 = Layout_GetRolePosition(2);
    RolePosition pos3 = Layout_GetRolePosition(3);
    
    if (event.motion.x>=roles[1].x
        && event.motion.x<=roles[1].x+pos1.width
        && event.motion.y>=roles[1].y
        && event.motion.y<=roles[1].y+pos1.height){
        roles[1].ischosen=!roles[1].ischosen;
    } else if (event.motion.x>=roles[2].x
               && event.motion.x<=roles[2].x+pos2.width
               && event.motion.y>=roles[2].y
               && event.motion.y<=roles[2].y+pos2.height){
        roles[2].ischosen=!roles[2].ischosen;
    }else if (event.motion.x>=roles[3].x
              && event.motion.x<=roles[3].x+pos3.width
              && event.motion.y>=roles[3].y
              && event.motion.y<=roles[3].y+pos3.height) {
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
    // 使用状态管理器
    if (roles[0].HP<=0){
        GameState_SetGameResult(1);  // 玩家胜利
        isover = 1;
        return;
    }
    if ((roles[1].HP<=0 || !roles[1].ischosen)
        && (roles[2].HP<=0 || !roles[2].ischosen)
        && (roles[3].HP<=0 || !roles[3].ischosen)){
        GameState_SetGameResult(-1);  // 玩家失败
        isover = -1;
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



// 使用新的元素反应系统
void Elemental_Reaction_Jugde(Role *src,Role *dst,int element)
{
    ElementalReaction_Process(src, dst, (Element_type)element);
}

// 向后兼容函数：保留旧接口，内部使用新系统
void Elemental_Reaction(Role *src,Role *dst,int debuff)
{
    // 这个函数现在由ElementalReaction_Apply内部处理
    // 保留此函数以保持向后兼容性
    // 实际反应逻辑已在ElementalReaction_Process中实现
}
