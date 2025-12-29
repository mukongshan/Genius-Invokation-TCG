//
// 布局管理系统 - 统一管理所有UI元素的位置
//

#ifndef GENIUS_INVOKATION_TCG_LAYOUT_H
#define GENIUS_INVOKATION_TCG_LAYOUT_H

#include "common.h"

// 窗口配置
typedef struct {
    int x;
    int y;
    int width;
    int height;
} WindowConfig;

// 角色位置配置
typedef struct {
    int x;
    int y;
    int width;
    int height;
} RolePosition;

// 技能按钮位置
typedef struct {
    SDL_Rect normal_attack;
    SDL_Rect elemental_skill;
    SDL_Rect elemental_burst;
} SkillButtonLayout;

// 文本位置配置
typedef struct {
    SDL_Rect hp;
    SDL_Rect energy;
    SDL_Rect points_player;
    SDL_Rect points_ai;
    SDL_Rect fighting_label;
    SDL_Rect round_end;
    SDL_Rect continue_prompt;
} TextLayout;

// 召唤物位置
typedef struct {
    int x;
    int y;
    int width;
    int height;
} SummonPosition;

// 全局布局配置
typedef struct {
    WindowConfig window;
    RolePosition roles[4];  // 0=敌人, 1-3=玩家角色
    RolePosition choice_marker;
    SkillButtonLayout skill_buttons;
    TextLayout text;
    SummonPosition summons[2];
    SDL_Rect introduce_panel;  // 角色介绍面板
    SDL_Rect action_button;     // 行动按钮
    SDL_Rect element_icon;      // 元素图标相对位置
    SDL_Rect debuff_icon;       // 异常状态图标相对位置
    SDL_Rect buff_icon;         // 增益图标相对位置
} LayoutConfig;

// 获取布局配置（基于窗口大小计算相对位置）
LayoutConfig* Layout_GetConfig(void);
void Layout_UpdateForWindow(int window_width, int window_height);

// 获取缩放比例
float Layout_GetScaleX(void);
float Layout_GetScaleY(void);
float Layout_GetScale(void);  // 返回较小的缩放比例（保持宽高比）

// 获取字体大小（根据缩放调整）
int Layout_GetFontSize(int base_size);

// 便捷函数：获取角色位置
RolePosition Layout_GetRolePosition(int role_index);
// 便捷函数：获取技能按钮位置
SkillButtonLayout* Layout_GetSkillButtons(void);
// 便捷函数：获取文本位置
TextLayout* Layout_GetTextLayout(void);

#endif //GENIUS_INVOKATION_TCG_LAYOUT_H

