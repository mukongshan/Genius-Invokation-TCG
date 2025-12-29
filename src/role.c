#include "role.h"

Role *roles;
Summons *summons;

int gu_wu_ling_yu=0;
int yu_lian_jian=0;
int hong_shi_jian=0;
int ting_huo_yan_xiao=0;
int liu_jin_huo_guang=0;

int points_1=0;
int points_0=0;
int isview=0;


void Init_Roles()
{
    roles =malloc (NUM_MEMBERS*sizeof (Role));
    roles[0]=(Role){true,true,0,0,30,0,no_element,no_debuff,0,0,1125,200};
    roles[1]=(Role){false,false,1,1,10,0,no_element,no_debuff,0,0,725,1000};
    roles[2]=(Role){false,false,1,2,10,0,no_element,no_debuff,0,0,1125,1000};
    roles[3]=(Role){false,false,1,3,10,0,no_element,no_debuff,0,0,1525,1000};
}

void Init_Summons()
{
    summons = malloc(NUM_SUMMONS*sizeof (Summons));
    summons[0]=(Summons){300,200,0,false};
    summons[1]=(Summons){500,200,0,false};
}

void Quit_Roles()
{
    free(roles);
}

void Quit_Summons()
{
    free(summons);
}

int Skills_Log(Role *src,Role *dst,int att)
{
    if (!(isround_1 && isround_0 )&& gu_wu_ling_yu>0 && src->team==1 && !isact_1) {
        Gu_Wu_Ling_Yu(src);
    }
    if (!(isround_1 && isround_0 )&& yu_lian_jian>0&&src->team==0) {
        return Yu_Lian_Jian(att);
    }
    if (!(isround_1 && isround_0 )&& src->num==3&&ting_huo_yan_xiao>0 && !isact_1){
        return Ting_Huo_Yan_Xiao(src,dst,att);
    }
    return att;
}

void After_Normal_Attack_Enhencement()
{
    Hong_Shi_Jian();
}

void After_Skill_Enhencement()
{
    Liu_Jin_Huo_Guang();
}

void After_Ultimate_Enhencement()
{
    Liu_Jin_Huo_Guang();
}

void Summons_Attack()
{
    if (summons[1].PP>0){
        Elemental_Reaction_Jugde(&roles[0],&roles[selection],ice);
        roles[selection].HP-= Inflict(&roles[0],&roles[selection],1);
        Vedio_Attack_0(&summons[1].y);
        Vedio_Common();
        Role_Fail();
    }
    if (summons[0].PP>0){
        Elemental_Reaction_Jugde(&roles[0],&roles[selection],wind);
        roles[selection].HP-= Inflict(&roles[0],&roles[selection],1);
        Vedio_Attack_0(&summons[0].y);
        Vedio_Common();
        Role_Fail();
    }
}

void Role_Fail()
{
    Judge_Win_Lose();
    if (roles[selection].HP<=0){
        for (selection=1;roles[selection].HP<=0 && selection<3;selection++);
    }
}

void Do_Bennett()
{
    Draw_Refresh();
    Draw_Skill(1);
    Text_Fighting();
    SDL_RenderPresent(app.renderer);
    SDL_Event event;
    Do_Event(event);
    while (!app.keyboard[SDL_SCANCODE_TAB] && !app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event))
    {
        Do_Event(event);
        Draw_Introduce_All(event,&isview);
        if (isview){
            isview=0;
            Text_Fighting();
            SDL_RenderPresent(app.renderer);
        }
        if (app.keyboard[SDL_SCANCODE_1] && event.type==SDL_KEYDOWN && points_1>=3){
            points_1-=3;
            Bennett_Attack();
            isact_1=1;
            if (roles[1].energy<2) {
                roles[1].energy++;
            }
            After_Normal_Attack_Enhencement();
            return;
        } else if (app.keyboard[SDL_SCANCODE_2] && event.type==SDL_KEYDOWN && points_1>=3){
            points_1-=3;
            Bennett_Skill();
            isact_1=1;
            if (roles[1].energy<2) {
                roles[1].energy++;
            }
            After_Skill_Enhencement();
            return;
        }else if (app.keyboard[SDL_SCANCODE_3] && event.type==SDL_KEYDOWN && points_1>=4 && roles[1].energy>=2){
            points_1-=4;
            Bennett_Ultimate();
            isact_1=1;
            roles[1].energy=0;
            After_Ultimate_Enhencement();
            return;
        } else if (app.keyboard[SDL_SCANCODE_SPACE] && event.type==SDL_KEYDOWN){
            isact_1=1;
            isround_1=1;
            return;
        }
    }
    roles[1].isfighting=false;
    Draw_Refresh();
}
void Bennett_Attack()
{
    Text_Normal_Attack_1();
    Show_Text();
    roles[0].HP-=Inflict(&roles[1],&roles[0],2);
    Vedio_Attack_1(&roles[1]);
}
void Bennett_Skill()
{
    Text_Elemental_Skill();
    Show_Text();
    Elemental_Reaction_Jugde(&roles[1],&roles[0],fire);
    roles[0].HP-=Inflict(&roles[1],&roles[0],3);
    Vedio_Attack_1(&roles[1]);
}
void Bennett_Ultimate()
{
    Text_Elemental_Burst_1();
    Show_Text();
    Elemental_Reaction_Jugde(&roles[1],&roles[0],fire);
    roles[0].HP-=Inflict(&roles[1],&roles[0],2);
    gu_wu_ling_yu=2;
    Vedio_Attack_1(&roles[1]);
}
void Gu_Wu_Ling_Yu(Role *role)
{
    if (role->HP>=7){
        role->attack_riase+=2;
    } else {
        role->HP+=2;
    }
}

void Do_Xingqiu()
{
    Draw_Refresh();
    Draw_Skill(2);
    Text_Fighting();
    SDL_RenderPresent(app.renderer);
    SDL_Event event;
    while (!app.keyboard[SDL_SCANCODE_TAB] && !app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event))
    {
        Do_Event(event);
        Draw_Introduce_All(event,&isview);
        if (isview){
            isview=0;
            Text_Fighting();
            SDL_RenderPresent(app.renderer);
        }
        if (app.keyboard[SDL_SCANCODE_1] && event.type==SDL_KEYDOWN && points_1>=3){
            points_1-=3;
            Xingqiu_Attack();
            isact_1=1;
            if (roles[2].energy<2) {
                roles[2].energy++;
            }
            After_Normal_Attack_Enhencement();
            return;
        } else if (app.keyboard[SDL_SCANCODE_2] && event.type==SDL_KEYDOWN && points_1>=3){
            points_1-=3;
            Xingqiu_Skill();
            isact_1=1;
            if (roles[2].energy<2) {
                roles[2].energy++;
            }
            After_Skill_Enhencement();
            return;
        }else if (app.keyboard[SDL_SCANCODE_3] && event.type==SDL_KEYDOWN && points_1>=3 && roles[2].energy>=2){
            points_1-=3;
            Xingqiu_Ultimate();
            isact_1=1;
            roles[2].energy=0;
            After_Ultimate_Enhencement();
            return;
        } else if (app.keyboard[SDL_SCANCODE_SPACE] && event.type==SDL_KEYDOWN){
            isact_1=1;
            isround_1=1;
            return;
        }
    }
    roles[2].isfighting=false;
    Draw_Refresh();
}
void Xingqiu_Attack()
{
    Text_Normal_Attack_1();
    Show_Text();
    roles[0].HP-=Inflict(&roles[2],&roles[0],2);
    Vedio_Attack_1(&roles[2]);
}
void Xingqiu_Skill()
{
    Text_Elemental_Skill();
    Show_Text();
    yu_lian_jian=2;
    switch (roles[2].element) {
        case no_element:
            roles[2].element=water;
            break;
        case ice:
            Elemental_Reaction(&roles[2],&roles[2],WI);
            roles[2].attack_riase=0;
            break;
        case water:
            break;
        default:
            roles[2].element=no_element;
            break;
    }
    Elemental_Reaction_Jugde(&roles[2],&roles[0],water);
    roles[0].HP-= Inflict(&roles[2],&roles[0],2);
    Vedio_Attack_1(&roles[2]);
}

void Xingqiu_Ultimate()
{
    Text_Elemental_Burst_1();
    Show_Text();
    hong_shi_jian=3;
    yu_lian_jian=2;
    switch (roles[2].element) {
        case no_element:
            roles[2].element=water;
            break;
        case ice:
            Elemental_Reaction(&roles[2],&roles[2],WI);
            roles[2].attack_riase=0;
            break;
        case water:
            break;
        default:
            roles[2].element=no_element;
            break;
    }
    Elemental_Reaction_Jugde(&roles[2],&roles[0],water);
    roles[0].HP-= Inflict(&roles[2],&roles[0],2);
    Vedio_Attack_1(&roles[2]);
}

int Yu_Lian_Jian(int att)
{
    if (att>=3) {
        yu_lian_jian--;
        return att - 1;
    }
}

void Hong_Shi_Jian()
{
    if (hong_shi_jian>0){
        hong_shi_jian--;
        Elemental_Reaction_Jugde(&roles[choice_i+1],&roles[0],water);
        roles[0].HP-= Inflict(&roles[choice_i+1],&roles[0],1);
        Vedio_Attack_1(&roles[choice_i+1]);
    }
}

void Do_Yoimiya()
{
    Draw_Refresh();
    Draw_Skill(3);
    Text_Fighting();
    SDL_RenderPresent(app.renderer);
    SDL_Event event;
    while (!app.keyboard[SDL_SCANCODE_TAB] && !app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event))
    {
        Do_Event(event);
        Draw_Introduce_All(event,&isview);
        if (isview){
            isview=0;
            Text_Fighting();
            SDL_RenderPresent(app.renderer);
        }
        if (app.keyboard[SDL_SCANCODE_1] && event.type==SDL_KEYDOWN && points_1>=3){
            points_1-=3;
            Yoimiya_Attack();
            isact_1=1;
            if (roles[3].energy<3) {
                roles[3].energy++;
            }
            After_Normal_Attack_Enhencement();
            return;
        } else if (app.keyboard[SDL_SCANCODE_2] && event.type==SDL_KEYDOWN && points_1>=1){
            points_1-=1;
            Yoimiya_Skill();
            isact_1=1;
            if (roles[3].energy<3) {
                roles[3].energy++;
            }
            After_Skill_Enhencement();
            return;
        }else if (app.keyboard[SDL_SCANCODE_3] && event.type==SDL_KEYDOWN && points_1>=3 && roles[3].energy>=3){
            points_1-=3;
            Yoimiya_Ultimate();
            isact_1=1;
            roles[3].energy=0;
            After_Ultimate_Enhencement();
            return;
        } else if (app.keyboard[SDL_SCANCODE_SPACE] && event.type==SDL_KEYDOWN){
            isact_1=1;
            isround_1=1;
            return;
        }
    }
    roles[3].isfighting=false;
    Draw_Refresh();
}
void Yoimiya_Attack()
{
    Text_Normal_Attack_1();
    Show_Text();
    roles[0].HP-=Inflict(&roles[3],&roles[0],2);
    Vedio_Attack_1(&roles[3]);
}
void Yoimiya_Skill()
{
    Text_Elemental_Skill();
    Show_Text();
    ting_huo_yan_xiao=2;
    Vedio_Attack_1(&roles[3]);
}
void Yoimiya_Ultimate()
{
    Text_Elemental_Burst_1();
    Show_Text();
    liu_jin_huo_guang=2;
    Elemental_Reaction_Jugde(&roles[3],&roles[0],fire);
    roles[0].HP-=Inflict(&roles[3],&roles[0],3);
    Vedio_Attack_1(&roles[3]);
}

int Ting_Huo_Yan_Xiao(Role *src,Role *dst,int att)
{
    ting_huo_yan_xiao--;
    Elemental_Reaction_Jugde(src,dst,fire);
    return att+1;
}

void Liu_Jin_Huo_Guang()
{
    if (liu_jin_huo_guang>0 && choice_i+1!=3){
        Elemental_Reaction_Jugde(&roles[choice_i+1],&roles[0],fire);
        roles[0].HP-=Inflict(&roles[choice_i+1],&roles[0],1);
        Vedio_Attack_1(&roles[choice_i+1]);
    }
}


void Do_Maguu_Kenki()
{
    if (points_0>=3 && roles[choice_i+1].HP<=2){
        points_0-=3;
        Maguu_Kenki_Attack();
        if (roles[0].energy<3) {
            roles[0].energy++;
        }
        return;
    }
    if (roles[0].energy>=3 && points_0>=3){
        points_0-=3;
        Maguu_Kenki_Ultimate();
        roles[0].energy=0;
    }
    if (summons[1].PP<=0 && points_0>=3){
        points_0-=3;
        Maguu_Kenki_Skill2();

        if (roles[0].energy<3) {
            roles[0].energy++;
        }
        if (points_0<3)
            isround_0=1;
        return;
    }
    if (summons[0].PP<=0 && points_0>=3){
        points_0-=3;
        Maguu_Kenki_Skill1();

        if (roles[0].energy<3) {
            roles[0].energy++;
        }
        if (points_0<3)
            isround_0=1;
        return;
    }
    if (points_0>=3){
        points_0-=3;
        Maguu_Kenki_Attack();
        if (roles[0].energy<3) {
            roles[0].energy++;
        }
        return;
    }
    if (points_0<3)
        isround_0=1;
}

void Maguu_Kenki_Attack()
{
    Text_Normal_Attack_0();
    Show_Text();
    roles[selection].HP-=Inflict(&roles[0],&roles[selection],2);
    Vedio_Attack_0(&roles[0].y);
    Role_Fail();
}
void Maguu_Kenki_Skill1()
{
    Text_Elemental_Skill_1();
    Show_Text();
    summons[0].PP=2;
    Vedio_Attack_0(&roles[0].y);
}
void Maguu_Kenki_Skill2()
{
    Text_Elemental_Skill_2();
    Show_Text();
    summons[1].PP=2;
    Vedio_Attack_0(&roles[0].y);
}

void Maguu_Kenki_Ultimate()
{
    Text_Elemental_Burst_0();
    Show_Text();
    Elemental_Reaction_Jugde(&roles[0],&roles[selection],wind);
    roles[selection].HP-= Inflict(&roles[0],&roles[selection],4);
    Vedio_Attack_0(&roles[0].y);
    Role_Fail();
    Summons_Attack();
}
