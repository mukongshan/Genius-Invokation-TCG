#include "main.h"

App app;
Audio audio;
Game game;
Win win;

int main(int argc, char *argv[])
{
    Init_App();
    Play_MP3();
    Do_Menu_Logic();
    Do_Preparation_Logic();
    Do_Game_Logic();
    Do_Over_Logic();
    Quit_App();
    return 0;
}

// 其他函数定义
static void Init_App()
{
    Init_Window();
    Init_Keyboard();
    Init_Word();
    Init_Img();
    Init_Music();
}

static void Quit_App()
{
    SDL_FreeSurface(app.img_surf);
    SDL_FreeSurface(app.txt_surf);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    free(app.keyboard);
    TTF_CloseFont(app.font);
    SDL_FreeSurface(game.surf_0);
    SDL_FreeSurface(game.surf_1);
    SDL_FreeSurface(game.surf_2);
    SDL_FreeSurface(game.surf_3);
    SDL_FreeSurface(game.surf_other);
    Mix_CloseAudio();
    Mix_FreeMusic(audio.music);
    Mix_Quit();
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
}

static void Init_Window()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        HANDLE_ERROR("Init SDL");
    }

    app.window = NULL;
    app.img_surf = NULL;
    app.txt_surf = NULL;
    app.renderer = NULL;
    game.surf_0 = NULL;
    game.surf_1 = NULL;
    game.surf_2 = NULL;
    game.surf_3 = NULL;
    game.surf_other = NULL;

    win.x = 0;
    win.y = 0;
    win.w = 2520;
    win.h = 1680;

    app.window = SDL_CreateWindow("Genius_Invokation_TCG", win.x,
                                  win.y, win.w, win.h, SDL_WINDOW_SHOWN);
    if (app.window == NULL)
    {
        HANDLE_ERROR("Init window");
    }

    app.renderer = SDL_CreateRenderer(app.window, -1, 0);
    if (app.renderer == NULL)
    {
        HANDLE_ERROR("Init renderer");
    }
}

static void Init_Keyboard()
{
    app.keyboard = calloc(SDL_NUM_SCANCODES, sizeof(bool));
}

static void Init_Word()
{
    if (TTF_Init() < 0)
    {
        SDL_Log("TTF init: %s", TTF_GetError());
        return;
    }
    Init_Text();
}

static void Init_Img()
{
    if (SDL_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0)
    {
        SDL_Log("IMG init: %s", IMG_GetError());
        return;
    }
}

static void Init_Music()
{
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
}
