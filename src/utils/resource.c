#include "utils/resource.h"
#include <string.h>
#include <stdlib.h>

static ResourceManager g_resource_mgr = {0};
ResourceManager* g_resources = &g_resource_mgr;

// 资源路径映射
static const char* g_resource_paths[RES_COUNT] = {
    [RES_BACKGROUND_COVER] = "..\\..\\res\\img\\Cover.png",
    [RES_BACKGROUND_PREPARATION] = "..\\..\\res\\img\\preparation_4.png",
    [RES_BACKGROUND_FIGHTING] = "..\\..\\res\\img\\Fighting2.png",
    [RES_BACKGROUND_ROUND] = "..\\..\\res\\img\\round.png",
    [RES_BACKGROUND_WIN] = "..\\..\\res\\img\\Win.jpg",
    [RES_BACKGROUND_LOSE] = "..\\..\\res\\img\\Lose.png",
    
    [RES_ROLE_BENNETT] = "..\\..\\res\\img\\Bennett.bmp",
    [RES_ROLE_XINGQIU] = "..\\..\\res\\img\\Xingqiu.bmp",
    [RES_ROLE_YOIMIYA] = "..\\..\\res\\img\\Yoimiya.bmp",
    [RES_ROLE_MAGUU_KENKI] = "..\\..\\res\\img\\MAguu_Kenki.bmp",
    
    [RES_ROLE_INTRO_BENNETT] = "..\\..\\res\\img\\bennett_jieshao.png",
    [RES_ROLE_INTRO_XINGQIU] = "..\\..\\res\\img\\xingqiu_jieshao.png",
    [RES_ROLE_INTRO_YOIMIYA] = "..\\..\\res\\img\\Yoimiya_jieshao.png",
    [RES_ROLE_INTRO_MAGUU_KENKI] = "..\\..\\res\\img\\Maguu_Kenki_jieshao.png",
    
    [RES_SKILL_SWORD_ATTACK] = "..\\..\\res\\img\\sword_attack.png",
    [RES_SKILL_BOW_ATTACK] = "..\\..\\res\\img\\bow_attack.png",
    [RES_SKILL_1] = "..\\..\\res\\img\\skill1.png",
    [RES_SKILL_2] = "..\\..\\res\\img\\skill2.png",
    [RES_SKILL_3] = "..\\..\\res\\img\\skill3.png",
    [RES_ULTIMATE_1] = "..\\..\\res\\img\\ultimate_1.png",
    [RES_ULTIMATE_2] = "..\\..\\res\\img\\ultimate_2.png",
    [RES_ULTIMATE_3] = "..\\..\\res\\img\\ultimate_3.png",
    
    [RES_BUFF_GU_WU_LING_YU] = "..\\..\\res\\img\\guwulingyu.png",
    [RES_BUFF_YU_LIAN_JIAN] = "..\\..\\res\\img\\yulianjian.png",
    [RES_BUFF_HONG_SHI_JIAN] = "..\\..\\res\\img\\hongshijian.png",
    [RES_BUFF_LIU_JIN_HUO_GUANG] = "..\\..\\res\\img\\liujinhuoguang.png",
    [RES_BUFF_TING_HUO_YAN_XIAO] = "..\\..\\res\\img\\tingyanxioahuo.png",
    
    [RES_SUMMON_GU_FENG] = "..\\..\\res\\img\\gufeng.bmp",
    [RES_SUMMON_SHUANG_CHI] = "..\\..\\res\\img\\shuangchi.bmp",
    
    [RES_ELEMENT_FIRE] = "..\\..\\res\\img\\fire.png",
    [RES_ELEMENT_WATER] = "..\\..\\res\\img\\water.png",
    [RES_ELEMENT_ICE] = "..\\..\\res\\img\\ice.png",
    [RES_ELEMENT_NOTHING] = "..\\..\\res\\img\\nothing_2.png",
    
    [RES_DEBUFF_FREEZE] = "..\\..\\res\\img\\freeze.png",
    
    [RES_UI_CHOICE] = "..\\..\\res\\img\\choice.png",
    [RES_UI_CHOOSE_IT] = "..\\..\\res\\img\\chooceit.png",
    [RES_UI_ACTION_BUTTON] = "..\\..\\res\\img\\xingdongfang.png",
    [RES_UI_NOTHING] = "..\\..\\res\\img\\nothing_2.png"
};

void Resource_Init(void) {
    memset(&g_resource_mgr, 0, sizeof(ResourceManager));
}

void Resource_Cleanup(void) {
    for (int i = 0; i < RES_COUNT; i++) {
        if (g_resource_mgr.surfaces[i] != NULL) {
            SDL_FreeSurface(g_resource_mgr.surfaces[i]);
            g_resource_mgr.surfaces[i] = NULL;
        }
        g_resource_mgr.loaded[i] = false;
    }
}

const char* Resource_GetPath(ResourceType type) {
    if (type >= 0 && type < RES_COUNT) {
        return g_resource_paths[type];
    }
    return NULL;
}

SDL_Surface* Resource_Load(ResourceType type) {
    if (type < 0 || type >= RES_COUNT) {
        return NULL;
    }
    
    // 如果已加载，直接返回
    if (g_resource_mgr.loaded[type] && g_resource_mgr.surfaces[type] != NULL) {
        return g_resource_mgr.surfaces[type];
    }
    
    // 释放旧资源
    if (g_resource_mgr.surfaces[type] != NULL) {
        SDL_FreeSurface(g_resource_mgr.surfaces[type]);
        g_resource_mgr.surfaces[type] = NULL;
    }
    
    const char* path = Resource_GetPath(type);
    if (path == NULL) {
        return NULL;
    }
    
    // 根据文件扩展名选择加载方式
    SDL_Surface* surface = NULL;
    const char* ext = strrchr(path, '.');
    
    if (ext != NULL && strcmp(ext, ".bmp") == 0) {
        surface = SDL_LoadBMP(path);
    } else {
        surface = IMG_Load(path);
    }
    
    if (surface == NULL) {
        SDL_Log("Failed to load resource %d from %s: %s", type, path, 
                ext && strcmp(ext, ".bmp") == 0 ? SDL_GetError() : IMG_GetError());
        return NULL;
    }
    
    g_resource_mgr.surfaces[type] = surface;
    g_resource_mgr.loaded[type] = true;
    return surface;
}

SDL_Surface* Resource_Get(ResourceType type) {
    if (type < 0 || type >= RES_COUNT) {
        return NULL;
    }
    
    if (!g_resource_mgr.loaded[type]) {
        return Resource_Load(type);
    }
    
    return g_resource_mgr.surfaces[type];
}

void Resource_LoadBackgrounds(void) {
    Resource_Load(RES_BACKGROUND_COVER);
    Resource_Load(RES_BACKGROUND_PREPARATION);
    Resource_Load(RES_BACKGROUND_FIGHTING);
    Resource_Load(RES_BACKGROUND_ROUND);
    Resource_Load(RES_BACKGROUND_WIN);
    Resource_Load(RES_BACKGROUND_LOSE);
}

void Resource_LoadRoles(void) {
    Resource_Load(RES_ROLE_BENNETT);
    Resource_Load(RES_ROLE_XINGQIU);
    Resource_Load(RES_ROLE_YOIMIYA);
    Resource_Load(RES_ROLE_MAGUU_KENKI);
    Resource_Load(RES_ROLE_INTRO_BENNETT);
    Resource_Load(RES_ROLE_INTRO_XINGQIU);
    Resource_Load(RES_ROLE_INTRO_YOIMIYA);
    Resource_Load(RES_ROLE_INTRO_MAGUU_KENKI);
}

void Resource_LoadSkills(void) {
    Resource_Load(RES_SKILL_SWORD_ATTACK);
    Resource_Load(RES_SKILL_BOW_ATTACK);
    Resource_Load(RES_SKILL_1);
    Resource_Load(RES_SKILL_2);
    Resource_Load(RES_SKILL_3);
    Resource_Load(RES_ULTIMATE_1);
    Resource_Load(RES_ULTIMATE_2);
    Resource_Load(RES_ULTIMATE_3);
    Resource_Load(RES_BUFF_GU_WU_LING_YU);
    Resource_Load(RES_BUFF_YU_LIAN_JIAN);
    Resource_Load(RES_BUFF_HONG_SHI_JIAN);
    Resource_Load(RES_BUFF_LIU_JIN_HUO_GUANG);
    Resource_Load(RES_BUFF_TING_HUO_YAN_XIAO);
    Resource_Load(RES_SUMMON_GU_FENG);
    Resource_Load(RES_SUMMON_SHUANG_CHI);
}

void Resource_LoadUI(void) {
    Resource_Load(RES_UI_CHOICE);
    Resource_Load(RES_UI_CHOOSE_IT);
    Resource_Load(RES_UI_ACTION_BUTTON);
    Resource_Load(RES_UI_NOTHING);
    Resource_Load(RES_ELEMENT_FIRE);
    Resource_Load(RES_ELEMENT_WATER);
    Resource_Load(RES_ELEMENT_ICE);
    Resource_Load(RES_ELEMENT_NOTHING);
    Resource_Load(RES_DEBUFF_FREEZE);
}

void Resource_LoadAll(void) {
    Resource_LoadBackgrounds();
    Resource_LoadRoles();
    Resource_LoadSkills();
    Resource_LoadUI();
}

