#ifndef AFFICHAGEHUD_H
#define AFFICHAGEHUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "sdlsprite.h"

class Jeu;
class Cafe;

/// gestion des elements d'interface fixes en haut de l'ecran
class AffichageHUD
{
public:
    /// rendu des textes et icones (argent, temps, reputation)
    void afficher(SDL_Renderer* renderer, TTF_Font* font, const Jeu& jeu, const Cafe& cafe, SDLSprite& spriteTitre, SDLSprite& spriteHeure, SDLSprite& spriteOuvert, SDLSprite& spriteFerme, SDLSprite& spriteArgentHud, SDLSprite& spriteReputation) const;
};

#endif