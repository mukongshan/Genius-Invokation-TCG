#include "utils/input.h"

void Do_Event(SDL_Event event)
{
    switch (event.type) {
        case SDL_QUIT:
            Do_Quit(event.quit);
            return;
        case SDL_KEYDOWN:
            Do_Keydown(event.key);
            break;
        case SDL_KEYUP:
            Do_Keyup(event.key);
            break;
        case SDL_MOUSEBUTTONDOWN:
            Do_Mousebuttondown(event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            Do_Mousebuttonup(event.button);
            break;
        default:
            break;
    }
}

static void Do_Keydown(SDL_KeyboardEvent key)
{
    app.keyboard[key.keysym.scancode] = true;
}

static void Do_Keyup(SDL_KeyboardEvent key)
{
    app.keyboard[key.keysym.scancode] = false;
}

static void Do_Quit(SDL_QuitEvent quit)
{
    app.keyboard[SDL_SCANCODE_ESCAPE] = true;
}
static void Do_Mousebuttondown(SDL_MouseButtonEvent button)
{
}
void Do_Mousebuttonup(SDL_MouseButtonEvent button)
{
}