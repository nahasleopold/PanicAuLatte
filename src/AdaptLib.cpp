#include "AdaptLib.h"

/// recup et tri des evenements (clavier, souris, fenetre)
bool AdaptLib::pollEvenement(EvenementLib& event)
{
    SDL_Event eventSDL;

    /// verif si un event est en attente
    if (!SDL_PollEvent(&eventSDL))
        return false;

    /// reset des donnees de l'event
    event.type = LIB_AUCUN;
    event.x = 0;
    event.y = 0;
    event.touche = LIB_TOUCHE_AUCUNE;

    /// gestion fermeture fenetre
    if (eventSDL.type == SDL_QUIT)
    {
        event.type = LIB_QUITTER;
    }
    /// detection clic gauche et recup des coordonnees x,y
    else if (eventSDL.type == SDL_MOUSEBUTTONDOWN && eventSDL.button.button == SDL_BUTTON_LEFT)
    {
        event.type = LIB_CLIC_GAUCHE;
        event.x = eventSDL.button.x;
        event.y = eventSDL.button.y;
    }
    /// traitement des touches pressées
    else if (eventSDL.type == SDL_KEYDOWN)
    {
        event.type = LIB_TOUCHE_ENFONCEE;

        /// mapping des touches specifiques
        switch (eventSDL.key.keysym.sym)
        {
            case SDLK_ESCAPE:
                event.touche = LIB_TOUCHE_ESCAPE;
                break;
            case SDLK_q:
                event.touche = LIB_TOUCHE_Q;
                break;
            case SDLK_RETURN:
                event.touche = LIB_TOUCHE_RETURN;
                break;
            case SDLK_b:
                event.touche = LIB_TOUCHE_B;
                break;
            default:
                event.touche = LIB_TOUCHE_AUCUNE;
                break;
        }
    }

    return true;
}

/// recup du temps ecoule depuis le debut en ms
Uint32 AdaptLib::getTicks()
{
    return SDL_GetTicks();
}