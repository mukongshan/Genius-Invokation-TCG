//
// 游戏状态管理器 - 统一管理游戏状态
//

#ifndef GENIUS_INVOKATION_TCG_GAME_STATE_H
#define GENIUS_INVOKATION_TCG_GAME_STATE_H

#include "common.h"

// 游戏阶段
typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PREPARATION,
    GAME_STATE_FIGHTING,
    GAME_STATE_ROUND_END,
    GAME_STATE_GAME_OVER
} GameState;

// 回合状态
typedef enum {
    ROUND_STATE_PLAYER_TURN,
    ROUND_STATE_ENEMY_TURN,
    ROUND_STATE_ENDED
} RoundState;

// 游戏状态结构
typedef struct {
    GameState current_state;
    RoundState round_state;
    
    // 游戏结果：0=未结束, 1=玩家胜利, -1=玩家失败
    int game_result;
    
    // 回合控制
    bool player_round_ended;
    bool enemy_round_ended;
    bool player_action_done;
    
    // 当前选择的角色索引
    int selected_role_index;
    int choice_index;  // 选择框索引
    
    // 行动点数
    int player_points;
    int enemy_points;
    
    // 是否正在查看角色介绍
    bool is_viewing_intro;
    
    // 是否已切换角色（用于判断是否需要消耗行动点）
    bool has_switched_role;
} GameStateManager;

// 全局游戏状态管理器
extern GameStateManager* g_game_state;

// 初始化游戏状态
void GameState_Init(void);
void GameState_Cleanup(void);

// 状态转换
void GameState_SetState(GameState state);
GameState GameState_GetState(void);

// 回合管理
void GameState_StartNewRound(void);
void GameState_EndPlayerRound(void);
void GameState_EndEnemyRound(void);
bool GameState_IsRoundEnded(void);

// 角色选择
void GameState_SetSelectedRole(int role_index);
int GameState_GetSelectedRole(void);
void GameState_SetChoiceIndex(int index);
int GameState_GetChoiceIndex(void);

// 行动点管理
void GameState_SetPlayerPoints(int points);
void GameState_SetEnemyPoints(int points);
int GameState_GetPlayerPoints(void);
int GameState_GetEnemyPoints(void);
bool GameState_ConsumePlayerPoints(int amount);
bool GameState_ConsumeEnemyPoints(int amount);

// 游戏结果
void GameState_SetGameResult(int result);  // 1=胜利, -1=失败
int GameState_GetGameResult(void);
bool GameState_IsGameOver(void);

// 其他状态
void GameState_SetPlayerActionDone(bool done);
bool GameState_IsPlayerActionDone(void);
void GameState_SetViewingIntro(bool viewing);
bool GameState_IsViewingIntro(void);
void GameState_SetHasSwitchedRole(bool switched);
bool GameState_HasSwitchedRole(void);

#endif //GENIUS_INVOKATION_TCG_GAME_STATE_H

