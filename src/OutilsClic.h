#ifndef OUTILSCLIC_H
#define OUTILSCLIC_H

#include <SDL2/SDL.h>

class OutilsClic
{
public:
    static bool pointDansRect(int x, int y, const SDL_Rect& rect)
    {
        return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
    }
};

#endif
