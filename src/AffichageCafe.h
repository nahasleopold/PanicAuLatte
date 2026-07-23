#ifndef AFFICHAGECAFE_H
#define AFFICHAGECAFE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "sdlsprite.h"

class Jeu;
class AffichageBulles;

/// gestion du rendu global de la zone de jeu
class AffichageCafe
{
public:
    /// dessin de tous les elements (fond, clients, serveurs, meubles)
    void afficher(SDL_Renderer* renderer, TTF_Font* font, Jeu& jeu, AffichageBulles& affichageBulles, SDLSprite& fond, SDLSprite& fondNuit, SDLSprite& spriteClient, SDLSprite spriteServeurs[5], SDLSprite& spriteTable, SDLSprite& spriteChaiseGauche, SDLSprite& spriteChaiseDroite, SDLSprite& spriteFileAttente, SDLSprite& spriteMachineCafe, SDLSprite& spriteMachineMatcha, SDLSprite& spriteMachineJus, SDLSprite& spriteMachineDessert) const;
    
    /// affichage de l'aide au placement pour les nouveaux objets
    void dessinerGrillePlacement(SDL_Renderer* renderer, const Jeu& jeu, SDLSprite& spriteFileAttente) const;
};

#endif