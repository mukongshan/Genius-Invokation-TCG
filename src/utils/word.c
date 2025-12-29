#include "utils/word.h"

SDL_Color color_HP = {255, 64, 64, 0};

void Init_Text()
{
    app.font = TTF_OpenFont("..\\..\\res\\typeface\\MTCORSVA.TTF",128);
    if (app.font==NULL){
        SDL_Log("TTF_OpenFont: %s",TTF_GetError());
        return;
    }
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
    SDL_Rect begin={850,1450,900,180};
    Draw_Text(begin);
}

void Text_Preparartion()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color white = {80, 128, 255, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Return to Continue",white);
    SDL_Rect Continue={830,1500,940,180};
    Draw_Text(Continue);
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf = TTF_RenderText_Solid(app.font,"Left Button to Choose",white);
    SDL_Rect Continue2={830,700,940,180};
    Draw_Text(Continue2);
}

void Text_Fighting()
{
    if (isswitch) {
        SDL_FreeSurface(app.txt_surf);
        app.txt_surf = NULL;
        SDL_Color white = {0, 0, 255, 0};
        app.txt_surf = TTF_RenderText_Solid(app.font, "Fighting", white);
        SDL_Rect Fighting = {roles[selection].x, roles[selection].y - 80, 120, 80};
        Draw_Text(Fighting);
    }
}

void Text_Round()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {180, 32, 64, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font, "Round End", color);
    SDL_Rect rect={1100,725,270,180};
    Draw_Text(rect);
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf = TTF_RenderText_Solid(app.font, "Press Return to Continue", color);
    SDL_Rect rect2={925,1480,720,180};
    Draw_Text(rect2);
}

void Text_Points()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    char point[16]="Your Points: ";
    Transform(point,points_1);
    SDL_Color color = {128, 255, 64, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,point,color);
    SDL_Rect rect={1860,220,420,60};
    Draw_Text(rect);

    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    char stack[16]="AI's Points: ";
    Transform(stack,points_0);
    app.txt_surf = TTF_RenderText_Solid(app.font,stack,color);
    SDL_Rect rect1={1860,300,420,60};
    Draw_Text(rect1);
}

void Text_HP(Role role)
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    if (role.ischosen)
    {
        if (role.HP > 0) {
            char hp[3] = {'\0'};
            Transform(hp, role.HP);
            app.txt_surf = TTF_RenderText_Solid(app.font, hp, color_HP);
            SDL_Rect HP_1 = {role.x + 195, role.y - 60, 75, 60};
            Draw_Text(HP_1);
        } else {
            app.txt_surf = TTF_RenderText_Solid(app.font, "Fail", color_HP);
            SDL_Rect HP_1 = {role.x + 195, role.y - 60, 120, 60};
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
        SDL_Rect PP = {role.x + 280, role.y + 10, 55, 50};
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
    SDL_Rect begin={1000,200,600,180};
    Draw_Text(begin);
}

void Text_Lose()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {255, 0, 64, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Lose!",color);
    SDL_Rect begin={1000,200,600,180};
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
    SDL_Rect rect={1020,725,510,180};
    Draw_Text(rect);
}


void Text_Normal_Attack_0()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Normal Attack!",color);
    SDL_Rect rect={400,700,280,90};
    Draw_Text(rect);
}
void Text_Normal_Attack_1()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Normal Attack!",color);
    SDL_Rect rect={400,900,280,90};
    Draw_Text(rect);
}
void Text_Elemental_Skill()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Elemental Skill!",color);
    SDL_Rect rect={400,900,320,90};
    Draw_Text(rect);
}
void Text_Elemental_Skill_1()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Elemental Skill 1!",color);
    SDL_Rect rect={400,700,360,90};
    Draw_Text(rect);
}
void Text_Elemental_Skill_2()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Elemental Skill 2!",color);
    SDL_Rect rect={400,700,360,90};
    Draw_Text(rect);
}
void Text_Elemental_Burst_0()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Elemental Burst!",color);
    SDL_Rect rect={400,700,320,90};
    Draw_Text(rect);
}
void Text_Elemental_Burst_1()
{
    SDL_FreeSurface(app.txt_surf);
    app.txt_surf=NULL;
    SDL_Color color = {60, 200, 160, 0};
    app.txt_surf = TTF_RenderText_Solid(app.font,"Elemental Burst!",color);
    SDL_Rect rect={400,900,320,90};
    Draw_Text(rect);
}

void Show_Text()
{
    SDL_RenderPresent(app.renderer);
    SDL_Delay(500);
    Draw_Refresh();
}

