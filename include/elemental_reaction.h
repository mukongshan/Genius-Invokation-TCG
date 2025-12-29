//
// 元素反应系统 - 使用查找表优化
//

#ifndef GENIUS_INVOKATION_TCG_ELEMENTAL_REACTION_H
#define GENIUS_INVOKATION_TCG_ELEMENTAL_REACTION_H

#include "common.h"

// 元素反应结果
typedef struct {
    Debuff debuff_type;
    int damage_bonus;
    bool apply_to_target;
    bool apply_to_others;
    Element_type spread_element;  // 扩散时传播的元素
} ReactionResult;

// 判断元素反应
void ElementalReaction_Process(Role* src, Role* dst, Element_type applied_element);

// 应用元素反应效果
void ElementalReaction_Apply(Role* src, Role* dst, ReactionResult* result);

// 获取反应结果（内部使用）
ReactionResult ElementalReaction_GetResult(Element_type src_element, Element_type dst_element);

#endif //GENIUS_INVOKATION_TCG_ELEMENTAL_REACTION_H

