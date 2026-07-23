#ifndef ADAPTLIB_H
#define ADAPTLIB_H

#include <SDL2/SDL.h>

/// differents types d'events possibles
enum TypeEvenementLib
{
    LIB_AUCUN,
    LIB_QUITTER,
    LIB_CLIC_GAUCHE,
    LIB_TOUCHE_ENFONCEE
};

/// mapping des touches clavier utilisees
enum ToucheLib
{
    LIB_TOUCHE_AUCUNE,
    LIB_TOUCHE_ESCAPE,
    LIB_TOUCHE_Q,
    LIB_TOUCHE_RETURN,
    LIB_TOUCHE_B
};

/// structure regroupant les infos d'un event
struct EvenementLib
{
    TypeEvenementLib type;
    int x;
    int y;
    ToucheLib touche;
};

/// classe outil pour l'interface avec la sdl
class AdaptLib
{
public:
    /// check si un event est arrive
    static bool pollEvenement(EvenementLib& event);
    /// recup du timer systeme en ms
    static Uint32 getTicks();
};

#endif