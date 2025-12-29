#include "elemental_reaction.h"
#include "utils/display.h"
#include "utils/word.h"

// 元素反应查找表
// [源元素][目标元素] -> 反应结果
static const ReactionResult g_reaction_table[8][8] = {
    // no_element
    [no_element] = {
        [no_element] = {no_debuff, 0, false, false, no_element},
        [fire] = {no_debuff, 0, false, false, no_element},
        [water] = {no_debuff, 0, false, false, no_element},
        [thunder] = {no_debuff, 0, false, false, no_element},
        [grass] = {no_debuff, 0, false, false, no_element},
        [ice] = {no_debuff, 0, false, false, no_element},
        [wind] = {no_debuff, 0, false, false, no_element},
        [rock] = {no_debuff, 0, false, false, no_element}
    },
    
    // fire
    [fire] = {
        [no_element] = {no_debuff, 0, true, false, fire},
        [fire] = {no_debuff, 0, false, false, no_element},
        [water] = {FW, 2, true, false, no_element},
        [thunder] = {FT, 0, false, false, no_element},
        [grass] = {FG, 0, false, false, no_element},
        [ice] = {FI, 0, false, false, no_element},
        [wind] = {no_debuff, 0, false, false, no_element},
        [rock] = {no_debuff, 0, false, false, no_element}
    },
    
    // water
    [water] = {
        [no_element] = {no_debuff, 0, true, false, water},
        [fire] = {FW, 2, true, false, no_element},
        [water] = {no_debuff, 0, false, false, no_element},
        [thunder] = {WT, 0, false, false, no_element},
        [grass] = {WG, 0, false, false, no_element},
        [ice] = {WI, 1, true, true, no_element},
        [wind] = {no_debuff, 0, false, false, no_element},
        [rock] = {no_debuff, 0, false, false, no_element}
    },
    
    // thunder
    [thunder] = {
        [no_element] = {no_debuff, 0, true, false, thunder},
        [fire] = {FT, 0, false, false, no_element},
        [water] = {WT, 0, false, false, no_element},
        [thunder] = {no_debuff, 0, false, false, no_element},
        [grass] = {TG, 0, false, false, no_element},
        [ice] = {TI, 0, false, false, no_element},
        [wind] = {no_debuff, 0, false, false, no_element},
        [rock] = {no_debuff, 0, false, false, no_element}
    },
    
    // grass
    [grass] = {
        [no_element] = {no_debuff, 0, true, false, grass},
        [fire] = {FG, 0, false, false, no_element},
        [water] = {WG, 0, false, false, no_element},
        [thunder] = {TG, 0, false, false, no_element},
        [grass] = {no_debuff, 0, false, false, no_element},
        [ice] = {no_debuff, 0, false, false, no_element},
        [wind] = {no_debuff, 0, false, false, no_element},
        [rock] = {no_debuff, 0, false, false, no_element}
    },
    
    // ice
    [ice] = {
        [no_element] = {no_debuff, 0, true, false, ice},
        [fire] = {FI, 0, false, false, no_element},
        [water] = {WI, 1, true, true, no_element},
        [thunder] = {TI, 0, false, false, no_element},
        [grass] = {no_debuff, 0, false, false, no_element},
        [ice] = {no_debuff, 0, false, false, no_element},
        [wind] = {no_debuff, 0, false, false, no_element},
        [rock] = {no_debuff, 0, false, false, no_element}
    },
    
    // wind
    [wind] = {
        [no_element] = {no_debuff, 0, false, false, no_element},
        [fire] = {WindF, 0, true, true, fire},
        [water] = {WindW, 0, true, true, water},
        [thunder] = {WindT, 0, true, true, thunder},
        [grass] = {no_debuff, 0, false, false, no_element},
        [ice] = {WindI, 0, true, true, ice},
        [wind] = {no_debuff, 0, false, false, no_element},
        [rock] = {no_debuff, 0, false, false, no_element}
    },
    
    // rock
    [rock] = {
        [no_element] = {RockAny, 0, false, false, no_element},
        [fire] = {RockAny, 0, false, false, no_element},
        [water] = {RockAny, 0, false, false, no_element},
        [thunder] = {RockAny, 0, false, false, no_element},
        [grass] = {RockAny, 0, false, false, no_element},
        [ice] = {RockAny, 0, false, false, no_element},
        [wind] = {RockAny, 0, false, false, no_element},
        [rock] = {RockAny, 0, false, false, no_element}
    }
};

ReactionResult ElementalReaction_GetResult(Element_type src_element, Element_type dst_element) {
    if (src_element >= 0 && src_element < 8 && dst_element >= 0 && dst_element < 8) {
        return g_reaction_table[src_element][dst_element];
    }
    return (ReactionResult){no_debuff, 0, false, false, no_element};
}

void ElementalReaction_Process(Role* src, Role* dst, Element_type applied_element) {
    if (src == NULL || dst == NULL) {
        return;
    }
    
    ReactionResult result = ElementalReaction_GetResult(applied_element, dst->element);
    
    // 如果没有反应，直接应用元素
    if (result.debuff_type == no_debuff && result.apply_to_target) {
        dst->element = applied_element;
        return;
    }
    
    // 如果有反应，应用反应效果
    if (result.debuff_type != no_debuff) {
        ElementalReaction_Apply(src, dst, &result);
    }
}

void ElementalReaction_Apply(Role* src, Role* dst, ReactionResult* result) {
    if (src == NULL || dst == NULL || result == NULL) {
        return;
    }
    
    // 显示反应文本
    if (result->debuff_type != no_debuff) {
        Text_Reaction(result->debuff_type);
        SDL_RenderPresent(app.renderer);
        SDL_Delay(500);
        Draw_Refresh();
    }
    
    // 应用伤害加成
    if (result->damage_bonus > 0) {
        src->attack_riase += result->damage_bonus;
    }
    
    // 清除目标元素
    if (result->apply_to_target) {
        dst->element = no_element;
    }
    
    // 应用异常状态
    if (result->debuff_type != no_debuff) {
        dst->debuff = result->debuff_type;
    }
    
    // 扩散效果：对其他角色应用元素
    if (result->apply_to_others && result->spread_element != no_element) {
        extern Role* roles;
        int choice_i = GameState_GetChoiceIndex();
        
        for (int i = 0; i <= 2; i++) {
            if (i != choice_i && roles[i + 1].ischosen && roles[i + 1].HP > 0) {
                ElementalReaction_Process(src, &roles[i + 1], result->spread_element);
                roles[i + 1].HP -= 1;
            }
        }
    }
}

