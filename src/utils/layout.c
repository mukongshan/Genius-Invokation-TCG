#include "utils/layout.h"
#include <stdlib.h>

static LayoutConfig g_layout = {0};
static float g_scale_x = 1.0f;
static float g_scale_y = 1.0f;
static float g_scale = 1.0f;  // 较小的缩放比例（保持宽高比）

// 默认布局配置（基于2520x1680的基准分辨率）
static void Layout_InitDefault(void) {
    // 窗口配置
    g_layout.window.x = 0;
    g_layout.window.y = 0;
    g_layout.window.width = 2520;
    g_layout.window.height = 1680;
    
    // 角色位置（玩家角色在底部，敌人在顶部）
    g_layout.roles[0] = (RolePosition){1125, 200, 278, 480};  // 敌人
    g_layout.roles[1] = (RolePosition){725, 1000, 278, 480};  // Bennett
    g_layout.roles[2] = (RolePosition){1125, 1000, 278, 480}; // Xingqiu
    g_layout.roles[3] = (RolePosition){1525, 1000, 278, 480}; // Yoimiya
    
    // 选择标记位置（相对于角色位置）
    g_layout.choice_marker = (RolePosition){0, 0, 75, 75};
    
    // 技能按钮位置
    g_layout.skill_buttons.normal_attack = (SDL_Rect){1900, 1350, 120, 120};
    g_layout.skill_buttons.elemental_skill = (SDL_Rect){2100, 1350, 120, 120};
    g_layout.skill_buttons.elemental_burst = (SDL_Rect){2300, 1350, 120, 120};
    
    // 文本位置
    g_layout.text.hp = (SDL_Rect){0, -60, 75, 60};  // 相对于角色位置
    g_layout.text.energy = (SDL_Rect){280, 10, 55, 50};  // 相对于角色位置
    g_layout.text.points_player = (SDL_Rect){1860, 220, 420, 60};
    g_layout.text.points_ai = (SDL_Rect){1860, 300, 420, 60};
    g_layout.text.fighting_label = (SDL_Rect){0, -80, 120, 80};  // 相对于角色位置
    g_layout.text.round_end = (SDL_Rect){1100, 725, 270, 180};
    g_layout.text.continue_prompt = (SDL_Rect){925, 1480, 720, 180};
    
    // 召唤物位置
    g_layout.summons[0] = (SummonPosition){300, 200, 160, 240};
    g_layout.summons[1] = (SummonPosition){500, 200, 160, 240};
    
    // 其他UI元素
    g_layout.introduce_panel = (SDL_Rect){1520, 0, 1000, 1200};
    g_layout.action_button = (SDL_Rect){350, 1000, 100, 100};
    g_layout.element_icon = (SDL_Rect){130, -70, 60, 65};  // 相对于角色位置
    g_layout.debuff_icon = (SDL_Rect){288, 60, 40, 40};    // 相对于角色位置
    g_layout.buff_icon = (SDL_Rect){-50, 0, 40, 40};       // 相对于角色位置，y坐标根据buff类型调整
}

LayoutConfig* Layout_GetConfig(void) {
    if (g_layout.window.width == 0) {
        Layout_InitDefault();
    }
    return &g_layout;
}

void Layout_UpdateForWindow(int window_width, int window_height) {
    Layout_InitDefault();
    
    // 如果窗口大小与默认不同，进行缩放
    if (window_width != g_layout.window.width || window_height != g_layout.window.height) {
        g_scale_x = (float)window_width / g_layout.window.width;
        g_scale_y = (float)window_height / g_layout.window.height;
        g_scale = (g_scale_x < g_scale_y) ? g_scale_x : g_scale_y;  // 使用较小的比例保持宽高比
        
        float scale_x = g_scale_x;
        float scale_y = g_scale_y;
        
        // 缩放所有角色位置
        for (int i = 0; i < 4; i++) {
            g_layout.roles[i].x = (int)(g_layout.roles[i].x * scale_x);
            g_layout.roles[i].y = (int)(g_layout.roles[i].y * scale_y);
            g_layout.roles[i].width = (int)(g_layout.roles[i].width * scale_x);
            g_layout.roles[i].height = (int)(g_layout.roles[i].height * scale_y);
        }
        
        // 缩放技能按钮
        g_layout.skill_buttons.normal_attack.x = (int)(g_layout.skill_buttons.normal_attack.x * scale_x);
        g_layout.skill_buttons.normal_attack.y = (int)(g_layout.skill_buttons.normal_attack.y * scale_y);
        g_layout.skill_buttons.normal_attack.w = (int)(g_layout.skill_buttons.normal_attack.w * scale_x);
        g_layout.skill_buttons.normal_attack.h = (int)(g_layout.skill_buttons.normal_attack.h * scale_y);
        
        g_layout.skill_buttons.elemental_skill.x = (int)(g_layout.skill_buttons.elemental_skill.x * scale_x);
        g_layout.skill_buttons.elemental_skill.y = (int)(g_layout.skill_buttons.elemental_skill.y * scale_y);
        g_layout.skill_buttons.elemental_skill.w = (int)(g_layout.skill_buttons.elemental_skill.w * scale_x);
        g_layout.skill_buttons.elemental_skill.h = (int)(g_layout.skill_buttons.elemental_skill.h * scale_y);
        
        g_layout.skill_buttons.elemental_burst.x = (int)(g_layout.skill_buttons.elemental_burst.x * scale_x);
        g_layout.skill_buttons.elemental_burst.y = (int)(g_layout.skill_buttons.elemental_burst.y * scale_y);
        g_layout.skill_buttons.elemental_burst.w = (int)(g_layout.skill_buttons.elemental_burst.w * scale_x);
        g_layout.skill_buttons.elemental_burst.h = (int)(g_layout.skill_buttons.elemental_burst.h * scale_y);
        
        // 缩放文本位置
        g_layout.text.hp.x = (int)(g_layout.text.hp.x * scale_x);
        g_layout.text.hp.y = (int)(g_layout.text.hp.y * scale_y);
        g_layout.text.hp.w = (int)(g_layout.text.hp.w * scale_x);
        g_layout.text.hp.h = (int)(g_layout.text.hp.h * scale_y);
        
        g_layout.text.energy.x = (int)(g_layout.text.energy.x * scale_x);
        g_layout.text.energy.y = (int)(g_layout.text.energy.y * scale_y);
        g_layout.text.energy.w = (int)(g_layout.text.energy.w * scale_x);
        g_layout.text.energy.h = (int)(g_layout.text.energy.h * scale_y);
        
        g_layout.text.points_player.x = (int)(g_layout.text.points_player.x * scale_x);
        g_layout.text.points_player.y = (int)(g_layout.text.points_player.y * scale_y);
        g_layout.text.points_player.w = (int)(g_layout.text.points_player.w * scale_x);
        g_layout.text.points_player.h = (int)(g_layout.text.points_player.h * scale_y);
        
        g_layout.text.points_ai.x = (int)(g_layout.text.points_ai.x * scale_x);
        g_layout.text.points_ai.y = (int)(g_layout.text.points_ai.y * scale_y);
        g_layout.text.points_ai.w = (int)(g_layout.text.points_ai.w * scale_x);
        g_layout.text.points_ai.h = (int)(g_layout.text.points_ai.h * scale_y);
        
        g_layout.text.fighting_label.x = (int)(g_layout.text.fighting_label.x * scale_x);
        g_layout.text.fighting_label.y = (int)(g_layout.text.fighting_label.y * scale_y);
        g_layout.text.fighting_label.w = (int)(g_layout.text.fighting_label.w * scale_x);
        g_layout.text.fighting_label.h = (int)(g_layout.text.fighting_label.h * scale_y);
        
        g_layout.text.round_end.x = (int)(g_layout.text.round_end.x * scale_x);
        g_layout.text.round_end.y = (int)(g_layout.text.round_end.y * scale_y);
        g_layout.text.round_end.w = (int)(g_layout.text.round_end.w * scale_x);
        g_layout.text.round_end.h = (int)(g_layout.text.round_end.h * scale_y);
        
        g_layout.text.continue_prompt.x = (int)(g_layout.text.continue_prompt.x * scale_x);
        g_layout.text.continue_prompt.y = (int)(g_layout.text.continue_prompt.y * scale_y);
        g_layout.text.continue_prompt.w = (int)(g_layout.text.continue_prompt.w * scale_x);
        g_layout.text.continue_prompt.h = (int)(g_layout.text.continue_prompt.h * scale_y);
        
        // 缩放召唤物位置
        for (int i = 0; i < 2; i++) {
            g_layout.summons[i].x = (int)(g_layout.summons[i].x * scale_x);
            g_layout.summons[i].y = (int)(g_layout.summons[i].y * scale_y);
            g_layout.summons[i].width = (int)(g_layout.summons[i].width * scale_x);
            g_layout.summons[i].height = (int)(g_layout.summons[i].height * scale_y);
        }
        
        // 缩放其他UI元素
        g_layout.introduce_panel.x = (int)(g_layout.introduce_panel.x * scale_x);
        g_layout.introduce_panel.y = (int)(g_layout.introduce_panel.y * scale_y);
        g_layout.introduce_panel.w = (int)(g_layout.introduce_panel.w * scale_x);
        g_layout.introduce_panel.h = (int)(g_layout.introduce_panel.h * scale_y);
        
        g_layout.action_button.x = (int)(g_layout.action_button.x * scale_x);
        g_layout.action_button.y = (int)(g_layout.action_button.y * scale_y);
        g_layout.action_button.w = (int)(g_layout.action_button.w * scale_x);
        g_layout.action_button.h = (int)(g_layout.action_button.h * scale_y);
        
        g_layout.element_icon.x = (int)(g_layout.element_icon.x * scale_x);
        g_layout.element_icon.y = (int)(g_layout.element_icon.y * scale_y);
        g_layout.element_icon.w = (int)(g_layout.element_icon.w * scale_x);
        g_layout.element_icon.h = (int)(g_layout.element_icon.h * scale_y);
        
        g_layout.debuff_icon.x = (int)(g_layout.debuff_icon.x * scale_x);
        g_layout.debuff_icon.y = (int)(g_layout.debuff_icon.y * scale_y);
        g_layout.debuff_icon.w = (int)(g_layout.debuff_icon.w * scale_x);
        g_layout.debuff_icon.h = (int)(g_layout.debuff_icon.h * scale_y);
        
        g_layout.buff_icon.x = (int)(g_layout.buff_icon.x * scale_x);
        g_layout.buff_icon.y = (int)(g_layout.buff_icon.y * scale_y);
        g_layout.buff_icon.w = (int)(g_layout.buff_icon.w * scale_x);
        g_layout.buff_icon.h = (int)(g_layout.buff_icon.h * scale_y);
        
        // 更新窗口配置
        g_layout.window.width = window_width;
        g_layout.window.height = window_height;
    }
}

RolePosition Layout_GetRolePosition(int role_index) {
    LayoutConfig* config = Layout_GetConfig();
    if (role_index >= 0 && role_index < 4) {
        return config->roles[role_index];
    }
    return (RolePosition){0, 0, 0, 0};
}

SkillButtonLayout* Layout_GetSkillButtons(void) {
    return &Layout_GetConfig()->skill_buttons;
}

TextLayout* Layout_GetTextLayout(void) {
    return &Layout_GetConfig()->text;
}

float Layout_GetScaleX(void) {
    return g_scale_x;
}

float Layout_GetScaleY(void) {
    return g_scale_y;
}

float Layout_GetScale(void) {
    return g_scale;
}

int Layout_GetFontSize(int base_size) {
    return (int)(base_size * g_scale);
}

