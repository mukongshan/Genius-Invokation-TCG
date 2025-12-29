#include "utils/display.h"
#include "utils/layout.h"
#include "utils/resource.h"
#include "game_state.h"

// 向后兼容：保留旧的location变量，但使用布局系统初始化
static SDL_Rect location_1 = {1900, 1350, 120, 120};
static SDL_Rect location_2 = {2100, 1350, 120, 120};
static SDL_Rect location_3 = {2300, 1350, 120, 120};

// 初始化技能按钮位置（使用布局系统）
static void Init_Skill_Locations(void) {
    SkillButtonLayout* buttons = Layout_GetSkillButtons();
    location_1 = buttons->normal_attack;
    location_2 = buttons->elemental_skill;
    location_3 = buttons->elemental_burst;
}

void Draw_Refresh()
{
    Draw_Img();
    Draw_Choice();
    Draw_Element_All();
    Draw_Debuff_All();
    Draw_Roles_All();
    Draw_Skill_Log();
    Text_HP_All();
    Text_Energy_All();
    Text_Points();
    Draw_Xingdongfang();
    Text_Fighting();
    if (roles[selection].isfighting){
        Draw_Skill(selection);
    }
}

void Draw_Img()
{
    if (app.img_surf==NULL) {
        HANDLE_ERROR("Draw_Img_surface");
    }
    if (app.renderer==NULL){
        HANDLE_ERROR("Draw_Img_renderer");
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer,app.img_surf);
    SDL_RenderCopy(app.renderer,texture,NULL,NULL);
    SDL_DestroyTexture(texture);
}

void Draw_Role(int num)
{
    if (num < 0 || num > 3) {
        return;
    }
    
    // 使用布局系统获取角色位置
    RolePosition pos = Layout_GetRolePosition(num);
    SDL_Rect role_rect = {roles[num].x, roles[num].y, pos.width, pos.height};
    
    // 使用资源管理器加载角色图像
    ResourceType res_type;
    switch (num) {
        case 0: res_type = RES_ROLE_MAGUU_KENKI; break;
        case 1: res_type = RES_ROLE_BENNETT; break;
        case 2: res_type = RES_ROLE_XINGQIU; break;
        case 3: res_type = RES_ROLE_YOIMIYA; break;
        default: return;
    }
    
    SDL_Surface* surface = Resource_Get(res_type);
    if (surface == NULL) {
        // 向后兼容：如果资源管理器失败，使用旧方法
        switch (num) {
            case 1: Load_Bennett(); surface = game.surf_1; break;
            case 2: Load_Xingqiu(); surface = game.surf_2; break;
            case 3: Load_Yoimiya(); surface = game.surf_3; break;
            case 0: Load_Maguu_Kenki(); surface = game.surf_0; break;
        }
    }
    
    if (surface == NULL) {
        HANDLE_ERROR("Draw_Role_surface");
    }
    if (app.renderer == NULL) {
        HANDLE_ERROR("Draw_Role_renderer");
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    SDL_RenderCopy(app.renderer, texture, NULL, &role_rect);
    SDL_DestroyTexture(texture);
}

void Draw_Roles_All()
{
    if (roles[1].ischosen==true)
        Draw_Role(1);
    if (roles[2].ischosen==true)
        Draw_Role(2);
    if (roles[3].ischosen==true)
        Draw_Role(3);
    Draw_Role(0);
}

void Draw_Choice()
{
    // 使用资源管理器
    SDL_Surface* surface = Resource_Get(RES_UI_CHOICE);
    if (surface == NULL) {
        // 向后兼容
        Load_Choice();
        surface = game.surf_other;
    }
    
    if (surface == NULL) {
        HANDLE_ERROR("Draw_Choice");
    }
    if (app.renderer == NULL) {
        HANDLE_ERROR("Draw_Choice");
    }

    // 使用游戏状态管理器获取选择索引
    int choice_idx = GameState_GetChoiceIndex();
    SDL_Rect choice_rect = {choice[choice_idx].x, choice[choice_idx].y, 75, 75};
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    SDL_RenderCopy(app.renderer, texture, NULL, &choice_rect);
    SDL_DestroyTexture(texture);
}

void Draw_Skill(int i)
{
    // 初始化技能按钮位置（如果还未初始化）
    static bool locations_initialized = false;
    if (!locations_initialized) {
        Init_Skill_Locations();
        locations_initialized = true;
    }
    
    ResourceType normal_attack_res, skill_res, ultimate_res;
    
    // 根据角色确定技能资源
    switch (i) {
        case 1:  // Bennett
            normal_attack_res = RES_SKILL_SWORD_ATTACK;
            skill_res = RES_SKILL_3;
            ultimate_res = RES_ULTIMATE_1;
            break;
        case 2:  // Xingqiu
            normal_attack_res = RES_SKILL_SWORD_ATTACK;
            skill_res = RES_SKILL_2;
            ultimate_res = RES_ULTIMATE_2;
            break;
        case 3:  // Yoimiya
            normal_attack_res = RES_SKILL_BOW_ATTACK;
            skill_res = RES_SKILL_1;
            ultimate_res = RES_ULTIMATE_3;
            break;
        default:
            return;
    }
    
    // 绘制普通攻击
    SDL_Surface* surf = Resource_Get(normal_attack_res);
    if (surf == NULL) {
        // 向后兼容
        if (i == 3) Load_Bow_Attack();
        else Load_Sword_Attack();
        surf = game.surf_other;
    }
    if (surf != NULL) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surf);
        SDL_RenderCopy(app.renderer, texture, NULL, &location_1);
        SDL_DestroyTexture(texture);
    }
    
    // 绘制元素战技
    surf = Resource_Get(skill_res);
    if (surf == NULL) {
        // 向后兼容
        switch (i) {
            case 1: Load_Skill_1(); break;
            case 2: Load_Skill_2(); break;
            case 3: Load_Skill_3(); break;
        }
        surf = game.surf_other;
    }
    if (surf != NULL) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surf);
        SDL_RenderCopy(app.renderer, texture, NULL, &location_2);
        SDL_DestroyTexture(texture);
    }
    
    // 绘制元素爆发
    surf = Resource_Get(ultimate_res);
    if (surf == NULL) {
        // 向后兼容
        switch (i) {
            case 1: Load_Ultimate_1(); break;
            case 2: Load_Ultimate_2(); break;
            case 3: Load_Ultimate_3(); break;
        }
        surf = game.surf_other;
    }
    if (surf != NULL) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surf);
        SDL_RenderCopy(app.renderer, texture, NULL, &location_3);
        SDL_DestroyTexture(texture);
    }
}

void Draw_Skill_Log()
{
    if (gu_wu_ling_yu>0){
        Draw_GuWuLingYu();
    }
    if (yu_lian_jian>0){
        Draw_YuLianJian();
    }
    if (ting_huo_yan_xiao>0){
        Draw_TingHuoYanXiao();
    }
    if (summons[0].PP>0){
        Draw_GuFeng();
    }
    if (summons[1].PP>0){
        Draw_Shuangchi();
    }
    if (hong_shi_jian>0){
        Draw_HongShiJian();
    }
    if (liu_jin_huo_guang>0){
        Draw_LiuJinHuoGuang();
    }
}
void Draw_GuWuLingYu()
{
    Load_GuWuLingYu();
    SDL_Rect rect={roles[selection].x-50,roles[selection].y+300,40,40};
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_1);
    SDL_RenderCopy(app.renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}
void Draw_YuLianJian()
{
    Load_YuLianJian();
    SDL_Rect rect={roles[selection].x-50,roles[selection].y+420,40,40};
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_2);
    SDL_RenderCopy(app.renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}
void Draw_HongShiJian()
{
    Load_HongShiJian();
    SDL_Rect rect={roles[selection].x-50,roles[selection].y+240,40,40};
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_2);
    SDL_RenderCopy(app.renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}
void Draw_LiuJinHuoGuang()
{
    Load_LiuJinHuoGuang();
    SDL_Rect rect={roles[selection].x-50,roles[selection].y+180,40,40};
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_3);
    SDL_RenderCopy(app.renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void Draw_TingHuoYanXiao()
{
    Load_TingHuoYanXiao();
    SDL_Rect rect={roles[3].x-50,roles[3].y+360,40,40};
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_3);
    SDL_RenderCopy(app.renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void Draw_GuFeng()
{
    Load_GuFeng();
    SDL_Rect rect={summons[0].x,summons[0].y,160,240};
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_0);
    SDL_RenderCopy(app.renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void Draw_Shuangchi()
{
    Load_ShuangChi();
    SDL_Rect rect={summons[1].x,summons[1].y,160,240};
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_0);
    SDL_RenderCopy(app.renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void Draw_Introduce_All(SDL_Event event,int *view)
{
    // 使用布局系统获取实际角色大小
    RolePosition pos0 = Layout_GetRolePosition(0);
    RolePosition pos1 = Layout_GetRolePosition(1);
    RolePosition pos2 = Layout_GetRolePosition(2);
    RolePosition pos3 = Layout_GetRolePosition(3);
    
    int flag=0;
    if (event.motion.x>=roles[1].x
        && event.motion.x<=roles[1].x+pos1.width
        && event.motion.y>=roles[1].y
        && event.motion.y<=roles[1].y+pos1.height){
        flag=1;
    } else if (event.motion.x>=roles[2].x
               && event.motion.x<=roles[2].x+pos2.width
               && event.motion.y>=roles[2].y
               && event.motion.y<=roles[2].y+pos2.height){
    flag=2;
    }else if (event.motion.x>=roles[3].x
              && event.motion.x<=roles[3].x+pos3.width
              && event.motion.y>=roles[3].y
              && event.motion.y<=roles[3].y+pos3.height) {
flag=3;
    }else if (event.motion.x>=roles[0].x
              && event.motion.x<=roles[0].x+pos0.width
              && event.motion.y>=roles[0].y
              && event.motion.y<=roles[0].y+pos0.height) {
flag=4;
    }
    if (flag)
    {
        while (flag && !app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event)) {
            if (!((event.motion.x >= roles[1].x
                 && event.motion.x <= roles[1].x + pos1.width
                 && event.motion.y >= roles[1].y
                 && event.motion.y <= roles[1].y + pos1.height) ||
                (event.motion.x >= roles[2].x
                 && event.motion.x <= roles[2].x + pos2.width
                 && event.motion.y >= roles[2].y
                 && event.motion.y <= roles[2].y + pos2.height) ||
                (event.motion.x >= roles[3].x
                 && event.motion.x <= roles[3].x + pos3.width
                 && event.motion.y >= roles[3].y
                 && event.motion.y <= roles[3].y + pos3.height) ||
                (event.motion.x >= roles[0].x
                 && event.motion.x <= roles[0].x + pos0.width
                 && event.motion.y >= roles[0].y
                 && event.motion.y <= roles[0].y + pos0.height))) {
                flag = 0;
            }
            switch (flag) {
                case 1:
                    if (roles[1].ischosen)
                        Draw_Introduce(1);
                    break;
                case 2:
                    if (roles[2].ischosen)
                    Draw_Introduce(2);
                    break;
                case 3:
                    if (roles[3].ischosen)
                    Draw_Introduce(3);
                    break;
                case 4:
                    if (roles[0].ischosen)
                    Draw_Introduce(0);
                    break;
                default:
                    break;
            }
            SDL_RenderPresent(app.renderer);
        }
        Draw_Refresh();
        *view = 1;
    }
}

void Draw_Introduce(int i)
{
    switch (i) {
        case 0:
            Load_Maguu_Kenki_jieshao();
            SDL_Rect rect0={1520,0,1000,1200};
            SDL_Texture *texture0 = SDL_CreateTextureFromSurface(app.renderer, game.surf_0);
            SDL_RenderCopy(app.renderer, texture0, NULL, &rect0);
            SDL_DestroyTexture(texture0);
            break;
        case 1:
            Load_Bennett_jieshao();
            SDL_Rect rect1={1520,0,1000,960};
            SDL_Texture *texture1 = SDL_CreateTextureFromSurface(app.renderer, game.surf_1);
            SDL_RenderCopy(app.renderer, texture1, NULL, &rect1);
            SDL_DestroyTexture(texture1);
            break;
        case 2:
            Load_Xingqiu_jieshao();
            SDL_Rect rect2={1520,0,1000,1200};
            SDL_Texture *texture2 = SDL_CreateTextureFromSurface(app.renderer, game.surf_2);
            SDL_RenderCopy(app.renderer, texture2, NULL, &rect2);
            SDL_DestroyTexture(texture2);
            break;
        case 3:
            Load_Yoimiya_jieshao();
            SDL_Rect rect3={1520,0,1000,1120};
            SDL_Texture *texture3 = SDL_CreateTextureFromSurface(app.renderer, game.surf_3);
            SDL_RenderCopy(app.renderer, texture3, NULL, &rect3);
            SDL_DestroyTexture(texture3);
            break;
        default:
            break;
    }
}

void Draw_Xingdongfang()
{
    if (isround_1 && isround_0){
        Load_Nothing();
        SDL_Rect rect={350,1000,100,100};
        SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_other);
        SDL_RenderCopy(app.renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    } else if ((!isact_1 || isround_0) && !isround_1){
        Load_Xingdong();
        SDL_Rect rect={350,1000,100,100};
        SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_other);
        SDL_RenderCopy(app.renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    } else if (!isround_0){
        Load_Xingdong();
        SDL_Rect rect={350,600,100,100};
        SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_other);
        SDL_RenderCopy(app.renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    }
}
void Draw_Element(Role role)
{
    if (role.ischosen) {
        switch (role.element) {
            case fire:
                Load_Fire();
                Draw_Element_Img(role);
                return;
            case water:
                Load_Water();
                Draw_Element_Img(role);
                return;
            case ice:
                Load_Ice();
                Draw_Element_Img(role);
                return;
            default:
                Load_Nothing();
                Draw_Element_Img(role);
                return;
        }
    }
}

void Draw_Element_Img(Role role)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_other);
    SDL_Rect ele0 = {role.x+130,role.y-70,60,65};
    SDL_RenderCopy(app.renderer, texture, NULL, &ele0);
    SDL_DestroyTexture(texture);
}

void Draw_Element_All()
{
    Draw_Element(roles[0]);
    Draw_Element(roles[1]);
    Draw_Element(roles[2]);
    Draw_Element(roles[3]);
}

void Draw_Debuff(Role role)
{
    if (role.debuff==WI && role.ischosen==true){
        Load_Freeze();
        SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, game.surf_other);
        SDL_Rect ele0 = {role.x+288,role.y+60,40,40};
        SDL_RenderCopy(app.renderer, texture, NULL, &ele0);
        SDL_DestroyTexture(texture);
    }
}

void Draw_Debuff_All()
{
    Draw_Debuff(roles[0]);
    Draw_Debuff(roles[1]);
    Draw_Debuff(roles[2]);
    Draw_Debuff(roles[3]);
}

void Draw_Chooceit()
{
    Load_Chooceit();
    if (game.surf_other==NULL) {
        HANDLE_ERROR("Draw_Chooceit_surface");
    }
    if (app.renderer==NULL){
        HANDLE_ERROR("Draw_Chooceit_renderer");
    }
    if (roles[1].ischosen){
        SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer,game.surf_other);
        SDL_Rect rect1 = {roles[1].x+130,roles[1].y-70,60,60};
        SDL_RenderCopy(app.renderer,texture,NULL,&rect1);
        SDL_DestroyTexture(texture);
    }
    if (roles[2].ischosen){
        SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer,game.surf_other);
        SDL_Rect rect1 = {roles[2].x+130,roles[2].y-70,60,60};
        SDL_RenderCopy(app.renderer,texture,NULL,&rect1);
        SDL_DestroyTexture(texture);
    }
    if (roles[3].ischosen){
        SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer,game.surf_other);
        SDL_Rect rect1 = {roles[3].x+130,roles[3].y-70,60,60};
        SDL_RenderCopy(app.renderer,texture,NULL,&rect1);
        SDL_DestroyTexture(texture);
    }
}


void Load_Cover()
{
    SDL_FreeSurface(app.img_surf);
    app.img_surf=NULL;
    app.img_surf = IMG_Load("..\\..\\res\\img\\Cover.png");
    if (app.img_surf == NULL)
    {
        HANDLE_ERROR("Load_Cover");
    }
}

void Load_Preparation()
{
    SDL_FreeSurface(app.img_surf);
    app.img_surf=NULL;
    app.img_surf = IMG_Load("..\\..\\res\\img\\preparation_4.png");
    if (app.img_surf == NULL)
    {
        HANDLE_ERROR("Load_Preparation");
    }
}

void Load_Fighting()
{
    SDL_FreeSurface(app.img_surf);
    app.img_surf=NULL;
    app.img_surf = IMG_Load("..\\..\\res\\img\\Fighting2.png");
    if (app.img_surf == NULL)
    {
        HANDLE_ERROR("Load_Fighting");
    }
}

void Load_Round()
{
    SDL_FreeSurface(app.img_surf);
    app.img_surf=NULL;
    app.img_surf = IMG_Load("..\\..\\res\\img\\round.png");
    if (app.img_surf == NULL)
    {
        HANDLE_ERROR("Load_Round");
    }
}

void Load_Bennett()
{
    SDL_FreeSurface(game.surf_1);
    game.surf_1=NULL;
    game.surf_1 = SDL_LoadBMP("..\\..\\res\\img\\Bennett.bmp");
    if (game.surf_1 == NULL)
    {
        HANDLE_ERROR("Load_Bennett");
    }
}

void Load_Xingqiu()
{
    SDL_FreeSurface(game.surf_2);
    game.surf_2=NULL;
    game.surf_2 = SDL_LoadBMP("..\\..\\res\\img\\Xingqiu.bmp");
    if (game.surf_2 == NULL)
    {
        HANDLE_ERROR("Load_Xingqiu");
    }
}

void Load_Yoimiya()
{
    SDL_FreeSurface(game.surf_3);
    game.surf_3=NULL;
    game.surf_3 = SDL_LoadBMP("..\\..\\res\\img\\Yoimiya.bmp");
    if (game.surf_3 == NULL)
    {
        HANDLE_ERROR("Load_Yoimiya");
    }
}

void Load_Maguu_Kenki()
{
    SDL_FreeSurface(game.surf_0);
    game.surf_0=NULL;
    game.surf_0 = SDL_LoadBMP("..\\..\\res\\img\\MAguu_Kenki.bmp");
    if (game.surf_0 == NULL)
    {
        HANDLE_ERROR("Load_Maguu_Kenki");
    }
}

void Load_Bennett_jieshao()
{
    SDL_FreeSurface(game.surf_1);
    game.surf_1=NULL;
    game.surf_1 = IMG_Load("..\\..\\res\\img\\bennett_jieshao.png");
    if (game.surf_1 == NULL)
    {
        HANDLE_ERROR("Load_Bennett_jieshao");
    }
}
void Load_Xingqiu_jieshao()
{
    SDL_FreeSurface(game.surf_2);
    game.surf_2=NULL;
    game.surf_2 = IMG_Load("..\\..\\res\\img\\xingqiu_jieshao.png");
    if (game.surf_2 == NULL)
    {
        HANDLE_ERROR("Load_Xingqiu_jieshao");
    }
}
void Load_Yoimiya_jieshao()
{
    SDL_FreeSurface(game.surf_3);
    game.surf_3=NULL;
    game.surf_3 = IMG_Load("..\\..\\res\\img\\Yoimiya_jieshao.png");
    if (game.surf_3 == NULL)
    {
        HANDLE_ERROR("Load_Yoimiya_jieshao");
    }
}
void Load_Maguu_Kenki_jieshao()
{
    SDL_FreeSurface(game.surf_0);
    game.surf_0=NULL;
    game.surf_0 = IMG_Load("..\\..\\res\\img\\Maguu_Kenki_jieshao.png");
    if (game.surf_0 == NULL)
    {
        HANDLE_ERROR("Load_Maguu_Kenki_jieshao");
    }
}

void Load_Choice()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\choice.png");
    if (game.surf_other == NULL)
    {
        HANDLE_ERROR("Load_Choice");
    }
}

void Load_Chooceit(){
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\chooceit.png");
    if (game.surf_other == NULL){
        SDL_Log("Load_Ice",IMG_GetError());
        return;
    }
}

void Load_Xingdong()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\xingdongfang.png");
    if (game.surf_other == NULL){
        SDL_Log("Load_Xingdong",IMG_GetError());
        return;
    }
}

void Load_Res()
{
    Load_Fighting();
    Load_Choice();
    Load_Bennett();
    Load_Xingqiu();
    Load_Yoimiya();
    Load_Maguu_Kenki();
}

void Load_Bow_Attack()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\bow_attack.png");
    if (game.surf_other == NULL)
    {
        HANDLE_ERROR("Load_Bow_Attack");
    }
}

void Load_Sword_Attack()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\sword_attack.png");
    if (game.surf_other == NULL)
    {
        HANDLE_ERROR("Load_Sword_Attack");
    }
}

void Load_Skill_1()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\skill3.png");
    if (game.surf_other == NULL)
    {
        HANDLE_ERROR("Load_Skill_1");
    }
}

void Load_Skill_2()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\skill2.png");
    if (game.surf_other == NULL)
    {
        HANDLE_ERROR("Load_Skill_2");
    }
}

void Load_Skill_3()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\skill1.png");
    if (game.surf_other == NULL)
    {
        HANDLE_ERROR("Load_Skill_3");
    }
}

void Load_Ultimate_1()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\ultimate_1.png");
    if (game.surf_other == NULL){
        HANDLE_ERROR("Load_Skill_3");
    }
}
void Load_Ultimate_2()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\ultimate_2.png");
    if (game.surf_other == NULL){
        HANDLE_ERROR("Load_Skill_3");
    }
}
void Load_Ultimate_3()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\ultimate_3.png");
    if (game.surf_other == NULL){
        HANDLE_ERROR("Load_Skill_3");
    }
}

void Load_GuWuLingYu()
{
    SDL_FreeSurface(game.surf_1);
    game.surf_1=NULL;
    game.surf_1 = IMG_Load("..\\..\\res\\img\\guwulingyu.png");
    if (game.surf_1 == NULL){
        SDL_Log("Load_YuLianJian",IMG_GetError());
        return;
    }
}

void Load_YuLianJian()
{
    SDL_FreeSurface(game.surf_2);
    game.surf_2=NULL;
    game.surf_2 = IMG_Load("..\\..\\res\\img\\yulianjian.png");
    if (game.surf_2 == NULL){
        SDL_Log("Load_YuLianJian",IMG_GetError());
        return;
    }
}
void Load_HongShiJian()
{SDL_FreeSurface(game.surf_2);
    game.surf_2=NULL;
    game.surf_2 = IMG_Load("..\\..\\res\\img\\hongshijian.png");
    if (game.surf_2 == NULL){
        SDL_Log("Load_HongShiJian",IMG_GetError());
        return;
    }
}
void Load_LiuJinHuoGuang()
{SDL_FreeSurface(game.surf_3);
    game.surf_3=NULL;
    game.surf_3 = IMG_Load("..\\..\\res\\img\\liujinhuoguang.png");
    if (game.surf_3 == NULL){
        SDL_Log("Load_LiuJinHuoGuang",IMG_GetError());
        return;
    }
}
void Load_TingHuoYanXiao()
{SDL_FreeSurface(game.surf_3);
    game.surf_3=NULL;
    game.surf_3 = IMG_Load("..\\..\\res\\img\\tingyanxioahuo.png");
    if (game.surf_3 == NULL){
        SDL_Log("Load_TingHuoYanXiao",IMG_GetError());
        return;
    }
}
void Load_GuFeng()
{
    SDL_FreeSurface(game.surf_0);
    game.surf_0=NULL;
    game.surf_0= SDL_LoadBMP("..\\..\\res\\img\\gufeng.bmp");
    if (game.surf_0 == NULL){
        HANDLE_ERROR("Load_GuFeng");
    }
}
void Load_ShuangChi()
{
    SDL_FreeSurface(game.surf_0);
    game.surf_0=NULL;
    game.surf_0= SDL_LoadBMP("..\\..\\res\\img\\shuangchi.bmp");
    if (game.surf_0 == NULL){
        HANDLE_ERROR("Load_ShuangChi");
    }
}


void Load_Win()
{
    SDL_FreeSurface(app.img_surf);
    app.img_surf=NULL;
    app.img_surf = IMG_Load("..\\..\\res\\img\\Win.jpg");
    if (app.img_surf == NULL)
    {
        HANDLE_ERROR("Load_Win");
    }
}
void Load_Lose()
{
    SDL_FreeSurface(app.img_surf);
    app.img_surf=NULL;
        app.img_surf = IMG_Load("..\\..\\res\\img\\Lose.png");
        if (app.img_surf == NULL)
        {
            HANDLE_ERROR("Load_Lose");
        }
}



void Load_Nothing()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\nothing_2.png");
    if (game.surf_other == NULL){
        SDL_Log("Load_Nothing",IMG_GetError());
        return;
    }
}

void Load_Fire()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
   game.surf_other = IMG_Load("..\\..\\res\\img\\fire.png");
    if (game.surf_other == NULL){
        SDL_Log("Load_Fire",IMG_GetError());
        return;
    }
}

void Load_Water()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\water.png");
    if (game.surf_other == NULL){
        SDL_Log("Load_Water",IMG_GetError());
        return;
    }
}

void Load_Ice()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\ice.png");
    if (game.surf_other == NULL){
        SDL_Log("Load_Ice",IMG_GetError());
        return;
    }
}

void Load_Freeze()
{
    SDL_FreeSurface(game.surf_other);
    game.surf_other=NULL;
    game.surf_other = IMG_Load("..\\..\\res\\img\\freeze.png");
    if (game.surf_other == NULL){
        SDL_Log("Load_Freeze",IMG_GetError());
        return;
    }
}



void Vedio_Common()
{
    Draw_Refresh();
    Text_Fighting();
    Draw_Element_All();
    Draw_Skill(choice_i+1);
    SDL_RenderPresent(app.renderer);
}

void Vedio_Attack_1(Role *role)
{
    // 使用布局系统获取缩放比例
    float scale_y = Layout_GetScaleY();
    
    // 获取角色的初始位置（从布局系统）
    RolePosition role_pos = Layout_GetRolePosition(role->num);
    int base_y = role_pos.y;
    
    // 原始动画：从base_y移动到950，再回到998，然后回到base_y
    // 缩放后的目标位置
    int target_y1 = (int)(950 * scale_y);  // 向上移动的目标
    int target_y2 = (int)(998 * scale_y);  // 向下移动的目标
    int step = (int)(4 * scale_y);
    if (step < 1) step = 1;
    
    // 计算相对于base_y的偏移
    int offset1 = base_y - target_y1;  // 向上移动的距离
    int offset2 = target_y2 - base_y;  // 向下移动的距离
    
    // 向上移动
    for (;role->y >= base_y - offset1; role->y -= step){
        Vedio_Common();
    }
    // 向下移动
    for (;role->y <= base_y + offset2; role->y += step){
        Vedio_Common();
    }
    // 回到原位
    for (;role->y > base_y; role->y -= step){
        Vedio_Common();
    }
    role->y = base_y;  // 确保回到准确位置
    Vedio_Common();
}

void Vedio_Attack_0(int *y)
{
    // 使用布局系统获取缩放比例
    float scale_y = Layout_GetScaleY();
    
    // 获取敌人角色的初始位置
    RolePosition enemy_pos = Layout_GetRolePosition(0);
    int base_y = enemy_pos.y;
    
    // 原始动画：从base_y移动到250，再回到200，然后回到base_y
    // 缩放后的目标位置
    int target_y1 = (int)(200 * scale_y);  // 向上移动的目标
    int target_y2 = (int)(250 * scale_y);  // 向下移动的目标
    int step = (int)(4 * scale_y);
    if (step < 1) step = 1;
    
    // 计算相对于base_y的偏移
    int offset1 = base_y - target_y1;  // 向上移动的距离
    int offset2 = target_y2 - base_y;  // 向下移动的距离
    
    // 向下移动
    for (;*y <= base_y + offset2; *y += step){
        Vedio_Common();
    }
    // 向上移动
    for (;*y >= base_y - offset1; *y -= step){
        Vedio_Common();
    }
    // 确保回到准确位置
    *y = base_y;
    Vedio_Common();
}
