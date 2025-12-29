//
// 资源管理器 - 统一管理图像和资源加载
//

#ifndef GENIUS_INVOKATION_TCG_RESOURCE_H
#define GENIUS_INVOKATION_TCG_RESOURCE_H

#include "common.h"

// 资源类型枚举
typedef enum {
    RES_BACKGROUND_COVER,
    RES_BACKGROUND_PREPARATION,
    RES_BACKGROUND_FIGHTING,
    RES_BACKGROUND_ROUND,
    RES_BACKGROUND_WIN,
    RES_BACKGROUND_LOSE,
    
    RES_ROLE_BENNETT,
    RES_ROLE_XINGQIU,
    RES_ROLE_YOIMIYA,
    RES_ROLE_MAGUU_KENKI,
    
    RES_ROLE_INTRO_BENNETT,
    RES_ROLE_INTRO_XINGQIU,
    RES_ROLE_INTRO_YOIMIYA,
    RES_ROLE_INTRO_MAGUU_KENKI,
    
    RES_SKILL_SWORD_ATTACK,
    RES_SKILL_BOW_ATTACK,
    RES_SKILL_1,
    RES_SKILL_2,
    RES_SKILL_3,
    RES_ULTIMATE_1,
    RES_ULTIMATE_2,
    RES_ULTIMATE_3,
    
    RES_BUFF_GU_WU_LING_YU,
    RES_BUFF_YU_LIAN_JIAN,
    RES_BUFF_HONG_SHI_JIAN,
    RES_BUFF_LIU_JIN_HUO_GUANG,
    RES_BUFF_TING_HUO_YAN_XIAO,
    
    RES_SUMMON_GU_FENG,
    RES_SUMMON_SHUANG_CHI,
    
    RES_ELEMENT_FIRE,
    RES_ELEMENT_WATER,
    RES_ELEMENT_ICE,
    RES_ELEMENT_NOTHING,
    
    RES_DEBUFF_FREEZE,
    
    RES_UI_CHOICE,
    RES_UI_CHOOSE_IT,
    RES_UI_ACTION_BUTTON,
    RES_UI_NOTHING,
    
    RES_COUNT  // 资源总数
} ResourceType;

// 资源管理器结构
typedef struct {
    SDL_Surface* surfaces[RES_COUNT];
    bool loaded[RES_COUNT];
} ResourceManager;

// 全局资源管理器
extern ResourceManager* g_resources;

// 初始化资源管理器
void Resource_Init(void);
void Resource_Cleanup(void);

// 加载资源
SDL_Surface* Resource_Load(ResourceType type);
SDL_Surface* Resource_Get(ResourceType type);

// 批量加载
void Resource_LoadBackgrounds(void);
void Resource_LoadRoles(void);
void Resource_LoadSkills(void);
void Resource_LoadUI(void);
void Resource_LoadAll(void);

// 资源路径获取（内部使用）
const char* Resource_GetPath(ResourceType type);

#endif //GENIUS_INVOKATION_TCG_RESOURCE_H

