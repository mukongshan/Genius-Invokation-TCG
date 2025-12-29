#include "utils/word.h"
#include "utils/layout.h"

SDL_Color color_HP = {255, 64, 64, 0};
static TTF_Font* g_font = NULL;

void Init_Text()
{
    // 使用布局系统获取缩放后的字体大小
    int font_size = Layout_GetFontSize(128);
    g_font = TTF_OpenFont("..\\..\\res\\typeface\\MTCORSVA.TTF", font_size);
    if (g_font == NULL) {
        // 如果失败，尝试使用默认大小
        g_font = TTF_OpenFont("..\\..\\res\\typeface\\MTCORSVA.TTF", 128);
    }
    if (g_font == NULL) {
        SDL_Log("TTF_OpenFont: %s", TTF_GetError());
        return;
    }
    app.font = g_font;
}

static void Draw_Text(SDL_Rect rect)
{
    if (app.txt_surf==NULL) {
        SDL_Log("Draw_Text_Begin txt_surf: %s", TTF_GetError());
        return;
    }
    if (app.renderer==NULL){
        HANDLE_ERROR("Draw_Text_Begin renderer");
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer,app.txt_surf);
    SDL_RenderCopy(app.renderer,texture,NULL,&rect);
    SDL_DestroyTexture(texture);
}

void Transform(char *hp_s,int hp)
{
    char *s=hp_s;
    while (*s!='\0'){
        s++;
    }
    if (hp>=10){
        *s=hp/10+'0';
        s++;
    }
    *s=hp%10+'0';
    s++;
    *s='\0';
}

void Text_Begin()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color white = {255, 255, 255, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Left Button to Begin",white);
    TextLayout* text_layout = Layout_GetTextLayout();
    // 使用continue_prompt位置作为开始文本位置
    SDL_Rect begin = text_layout->continue_prompt;
    begin.y = (int)(begin.y * 0.98f);  // 稍微上移
    Draw_Text(begin);
}

void Text_Preparartion()
{
    TextLayout* text_layout = Layout_GetTextLayout();
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color white = {80, 128, 255, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Return to Continue",white);
    Draw_Text(text_layout->continue_prompt);
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf = TTF_RenderText_Solid(app.font,"Left Button to Choose",white);
    SDL_Rect Continue2 = text_layout->round_end;
    Continue2.y = (int)(Continue2.y * 0.42f);  // 调整到合适位置
    Draw_Text(Continue2);
}

void Text_Fighting()
{
    if (isswitch) {
        SDL_FreeSurface(app.txt_surf);
        app.txt_surf = NULL;
        SDL_Color white = {0, 0, 255, 0};
        app.txt_surf = TTF_RenderText_Solid(app.font, "Fighting", white);
        TextLayout* text_layout = Layout_GetTextLayout();
        SDL_Rect Fighting = {roles[selection].x + text_layout->fighting_label.x, 
                            roles[selection].y + text_layout->fighting_label.y, 
                            text_layout->fighting_label.w, text_layout->fighting_label.h};
        Draw_Text(Fighting);
    }
}

void Text_Round()
{
    TextLayout* text_layout = Layout_GetTextLayout();
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {180, 32, 64, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font, "Round End", color);
    Draw_Text(text_layout->round_end);
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf = TTF_RenderText_Solid(app.font, "Press Return to Continue", color);
    Draw_Text(text_layout->continue_prompt);
}

void Text_Points()
{
    TextLayout* text_layout = Layout_GetTextLayout();
    
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    char point[16]="Your Points: ";
    Transform(point,points_1);
    SDL_Color color = {128, 255, 64, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,point,color);
    Draw_Text(text_layout->points_player);

    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    char stack[16]="AI's Points: ";
    Transform(stack,points_0);
    app.txt_surf = TTF_RenderText_Solid(app.font,stack,color);
    Draw_Text(text_layout->points_ai);
}

void Text_HP(Role role)
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    if (role.ischosen)
    {
        TextLayout* text_layout = Layout_GetTextLayout();
        if (role.HP > 0) {
            char hp[3] = {'\0'};
            Transform(hp, role.HP);
            app.txt_surf = TTF_RenderText_Solid(app.font, hp, color_HP);
            // 使用布局系统的相对位置
            SDL_Rect HP_1 = {role.x + text_layout->hp.x, role.y + text_layout->hp.y, 
                            text_layout->hp.w, text_layout->hp.h};
            Draw_Text(HP_1);
        } else {
            app.txt_surf = TTF_RenderText_Solid(app.font, "Fail", color_HP);
            SDL_Rect HP_1 = {role.x + text_layout->hp.x, role.y + text_layout->hp.y, 
                            (int)(text_layout->hp.w * 1.6f), text_layout->hp.h};
            Draw_Text(HP_1);
        }
    }
}

void Text_Energy(Role role)
{
    if (role.ischosen) {
        char pp[2] = {'\0'};
        Transform(pp, role.energy);
        SDL_FreeSurface(app.txt_surf);
        app.txt_surf=NULL;
        SDL_Color color = {240, 120, 120, 0};
        app.txt_surf = TTF_RenderText_Solid(app.font, pp, color);
        TextLayout* text_layout = Layout_GetTextLayout();
        SDL_Rect PP = {role.x + text_layout->energy.x, role.y + text_layout->energy.y, 
                      text_layout->energy.w, text_layout->energy.h};
        Draw_Text(PP);
    }
}

void Text_Energy_All()
{
    Text_Energy(roles[0]);
    Text_Energy(roles[1]);
    Text_Energy(roles[2]);
    Text_Energy(roles[3]);
}

void Text_HP_Summons(Summons summon)
{
    if (summon.PP>0) {
        char hp[3] = {'\0'};
        Transform(hp, summon.PP);
        SDL_FreeSurface(app.txt_surf);
        app.txt_surf=NULL;
        app.txt_surf = TTF_RenderText_Solid(app.font, hp, color_HP);
        SDL_Rect HP_1 = {summon.x + 100, summon.y -40, 60, 40};
        Draw_Text(HP_1);
    }
}

void Text_HP_All()
{
    Text_HP(roles[0]);
    Text_HP(roles[1]);
    Text_HP(roles[2]);
    Text_HP(roles[3]);
    Text_HP_Summons(summons[0]);
    Text_HP_Summons(summons[1]);
}


void Text_Win()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color white = {255, 0, 64, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Win!",white);
    TextLayout* text_layout = Layout_GetTextLayout();
    SDL_Rect begin = text_layout->round_end;
    begin.y = (int)(begin.y * 0.28f);  // 调整到顶部
    begin.w = (int)(begin.w * 2.2f);
    Draw_Text(begin);
}

void Text_Lose()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {255, 0, 64, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Lose!",color);
    TextLayout* text_layout = Layout_GetTextLayout();
    SDL_Rect begin = text_layout->round_end;
    begin.y = (int)(begin.y * 0.28f);  // 调整到顶部
    begin.w = (int)(begin.w * 2.2f);
    Draw_Text(begin);
}



void Text_Reaction(int debuff)
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {128, 32, 64, 0};
    //每个字母占30像素宽
    switch (debuff) {
        case FW:
            app.txt_surf = TTF_RenderText_Solid(app.font, "   Evaporative   ", color);
            break;
        case FT:
            app.txt_surf = TTF_RenderText_Solid(app.font, "    Superload    ", color);
            break;
        case FG:
            app.txt_surf = TTF_RenderText_Solid(app.font, "      Burn      ", color);
            break;
        case FI:
            app.txt_surf = TTF_RenderText_Solid(app.font, "      Thaw      ", color);
            break;
        case WT:
            app.txt_surf = TTF_RenderText_Solid(app.font, "      Shock      ", color);
            break;
        case WG:
            app.txt_surf = TTF_RenderText_Solid(app.font, "      Grow      ", color);
            break;
        case WI:
            app.txt_surf = TTF_RenderText_Solid(app.font, "     Freeze     ", color);
            break;
        case TG:
            app.txt_surf = TTF_RenderText_Solid(app.font, "    Intensity    ", color);
            break;
        case TI:
            app.txt_surf = TTF_RenderText_Solid(app.font, "Superconductivity", color);
            break;
        case WindF:
        case WindW:
        case WindT:
        case WindI:
            app.txt_surf = TTF_RenderText_Solid(app.font, "     Diffuse     ", color);
            break;
        default:
            return;
    }
    TextLayout* text_layout = Layout_GetTextLayout();
    // 使用round_end位置作为反应文本位置（可以后续优化）
    SDL_Rect rect = text_layout->round_end;
    rect.w = (int)(rect.w * 1.9f);  // 反应文本稍宽
    Draw_Text(rect);
}


// 辅助函数：获取技能文本位置
static SDL_Rect Get_Skill_Text_Rect(int y_offset) {
    TextLayout* text_layout = Layout_GetTextLayout();
    SDL_Rect rect = text_layout->round_end;
    rect.x = (int)(rect.x * 0.36f);  // 左侧
    rect.y = (int)(rect.y * y_offset);
    rect.w = (int)(rect.w * 1.0f);
    rect.h = (int)(rect.h * 0.5f);
    return rect;
}

void Text_Normal_Attack_0()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Normal Attack!",color);
    SDL_Rect rect = Get_Skill_Text_Rect(0.96f);
    Draw_Text(rect);
}
void Text_Normal_Attack_1()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Normal Attack!",color);
    SDL_Rect rect = Get_Skill_Text_Rect(1.24f);
    Draw_Text(rect);
}
void Text_Elemental_Skill()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Elemental Skill!",color);
    SDL_Rect rect = Get_Skill_Text_Rect(1.24f);
    Draw_Text(rect);
}
void Text_Elemental_Skill_1()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Elemental Skill 1!",color);
    SDL_Rect rect = Get_Skill_Text_Rect(0.96f);
    Draw_Text(rect);
}
void Text_Elemental_Skill_2()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Elemental Skill 2!",color);
    SDL_Rect rect = Get_Skill_Text_Rect(0.96f);
    Draw_Text(rect);
}
void Text_Elemental_Burst_0()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Elemental Burst!",color);
    SDL_Rect rect = Get_Skill_Text_Rect(0.96f);
    Draw_Text(rect);
}
void Text_Elemental_Burst_1()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Elemental Burst!",color);
    SDL_Rect rect = Get_Skill_Text_Rect(1.24f);
    Draw_Text(rect);
}

void Show_Text()
{
    SDL_RenderPresent(app.renderer);
    SDL_Delay(500);
    Draw_Refresh();
}

