#include "game_state.h"
#include <stdlib.h>

static GameStateManager g_state = {0};

GameStateManager* g_game_state = &g_state;

void GameState_Init(void) {
    g_state.current_state = GAME_STATE_MENU;
    g_state.round_state = ROUND_STATE_PLAYER_TURN;
    g_state.game_result = 0;
    g_state.player_round_ended = false;
    g_state.enemy_round_ended = false;
    g_state.player_action_done = false;
    g_state.selected_role_index = 1;
    g_state.choice_index = 0;
    g_state.player_points = 8;
    g_state.enemy_points = 8;
    g_state.is_viewing_intro = false;
    g_state.has_switched_role = false;
}

void GameState_Cleanup(void) {
    // 目前没有动态分配的资源需要清理
}

void GameState_SetState(GameState state) {
    g_state.current_state = state;
}

GameState GameState_GetState(void) {
    return g_state.current_state;
}

void GameState_StartNewRound(void) {
    g_state.player_round_ended = false;
    g_state.enemy_round_ended = false;
    g_state.player_action_done = false;
    g_state.player_points = 8;
    g_state.enemy_points = 8;
    g_state.has_switched_role = false;
    g_state.round_state = ROUND_STATE_PLAYER_TURN;
}

void GameState_EndPlayerRound(void) {
    g_state.player_round_ended = true;
    g_state.round_state = ROUND_STATE_ENEMY_TURN;
}

void GameState_EndEnemyRound(void) {
    g_state.enemy_round_ended = true;
    g_state.round_state = ROUND_STATE_ENDED;
}

bool GameState_IsRoundEnded(void) {
    return g_state.player_round_ended && g_state.enemy_round_ended;
}

void GameState_SetSelectedRole(int role_index) {
    if (role_index >= 0 && role_index < 4) {
        g_state.selected_role_index = role_index;
    }
}

int GameState_GetSelectedRole(void) {
    return g_state.selected_role_index;
}

void GameState_SetChoiceIndex(int index) {
    if (index >= 0 && index < 3) {
        g_state.choice_index = index;
    }
}

int GameState_GetChoiceIndex(void) {
    return g_state.choice_index;
}

void GameState_SetPlayerPoints(int points) {
    g_state.player_points = points;
}

void GameState_SetEnemyPoints(int points) {
    g_state.enemy_points = points;
}

int GameState_GetPlayerPoints(void) {
    return g_state.player_points;
}

int GameState_GetEnemyPoints(void) {
    return g_state.enemy_points;
}

bool GameState_ConsumePlayerPoints(int amount) {
    if (g_state.player_points >= amount) {
        g_state.player_points -= amount;
        return true;
    }
    return false;
}

bool GameState_ConsumeEnemyPoints(int amount) {
    if (g_state.enemy_points >= amount) {
        g_state.enemy_points -= amount;
        return true;
    }
    return false;
}

void GameState_SetGameResult(int result) {
    g_state.game_result = result;
}

int GameState_GetGameResult(void) {
    return g_state.game_result;
}

bool GameState_IsGameOver(void) {
    return g_state.game_result != 0;
}

void GameState_SetPlayerActionDone(bool done) {
    g_state.player_action_done = done;
}

bool GameState_IsPlayerActionDone(void) {
    return g_state.player_action_done;
}

void GameState_SetViewingIntro(bool viewing) {
    g_state.is_viewing_intro = viewing;
}

bool GameState_IsViewingIntro(void) {
    return g_state.is_viewing_intro;
}

void GameState_SetHasSwitchedRole(bool switched) {
    g_state.has_switched_role = switched;
}

bool GameState_HasSwitchedRole(void) {
    return g_state.has_switched_role;
}

