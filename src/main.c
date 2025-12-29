#include "main.h"
#include "game_state.h"
#include "utils/resource.h"
#include "utils/layout.h"

App app;
Audio audio;
Game game;
Win win;

int main(int argc, char *argv[])
{
    Init_App();
    
    // 初始化新系统
    Resource_Init();
    Layout_GetConfig();  // 初始化布局系统
    GameState_Init();
    
    // 预加载资源（可选，也可以按需加载）
    // Resource_LoadAll();
    
    Play_MP3();
    Do_Menu_Logic();
    Do_Preparation_Logic();
    Do_Game_Logic();
    Do_Over_Logic();
    
    // 清理新系统
    Resource_Cleanup();
    GameState_Cleanup();
    
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

    // 获取显示器模式以获取屏幕分辨率
    SDL_DisplayMode display_mode;
    if (SDL_GetDesktopDisplayMode(0, &display_mode) != 0) {
        SDL_Log("无法获取显示器信息，使用默认分辨率: %s", SDL_GetError());
        // 如果获取失败，使用默认值
        display_mode.w = 1920;
        display_mode.h = 1080;
    }
    
    // 基准分辨率（原始设计分辨率）
    const int base_width = 2520;
    const int base_height = 1680;
    
    // 计算缩放比例，保持宽高比
    float scale_x = (float)display_mode.w / base_width;
    float scale_y = (float)display_mode.h / base_height;
    float scale = (scale_x < scale_y) ? scale_x : scale_y;  // 使用较小的比例以保持宽高比
    
    // 设置窗口大小为屏幕的90%（或根据缩放比例调整）
    // 如果屏幕比基准分辨率小，则使用屏幕大小；否则使用缩放后的基准大小
    if (display_mode.w < base_width || display_mode.h < base_height) {
        // 屏幕较小，使用屏幕大小的90%
        win.w = (int)(display_mode.w * 0.9f);
        win.h = (int)(display_mode.h * 0.9f);
    } else {
        // 屏幕较大，使用缩放后的基准大小
        win.w = (int)(base_width * scale);
        win.h = (int)(base_height * scale);
    }
    
    // 确保最小尺寸
    if (win.w < 1280) win.w = 1280;
    if (win.h < 720) win.h = 720;
    
    // 居中窗口
    win.x = (display_mode.w - win.w) / 2;
    win.y = (display_mode.h - win.h) / 2;

    app.window = SDL_CreateWindow("Genius_Invokation_TCG", win.x,
                                  win.y, win.w, win.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (app.window == NULL)
    {
        HANDLE_ERROR("Init window");
    }

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (app.renderer == NULL)
    {
        HANDLE_ERROR("Init renderer");
    }
    
    // 更新布局系统以适应窗口大小
    Layout_UpdateForWindow(win.w, win.h);
    
    SDL_Log("窗口大小: %dx%d (屏幕: %dx%d)", win.w, win.h, display_mode.w, display_mode.h);
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
