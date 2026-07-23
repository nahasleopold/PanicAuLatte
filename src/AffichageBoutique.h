#ifndef AFFICHAGEBOUTIQUE_H
#define AFFICHAGEBOUTIQUE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "sdlsprite.h"

class SDLJeu;

/// gestion de l'interface visuelle de la boutique (boutons et prix)
class AffichageBoutique
{
private:
    /// zones de collision pour les clics sur les items
    SDL_Rect boutonTable;
    SDL_Rect boutonsServeurs[5];
    SDL_Rect boutonsMachines[3];

public:
    /// setup des positions des elements de la boutique
    AffichageBoutique();

    /// methode de dessin principale de l'interface
    void afficher(SDL_Renderer* renderer, TTF_Font* font, const SDLSprite& spriteTable, const SDLSprite spriteServeurs[5], float argent, SDLJeu& sdlJeu);

    /// getters pour la detection des clics utilisateur
    SDL_Rect getBoutonTable() const;
    SDL_Rect getBoutonServeur(int index) const;
    SDL_Rect getBoutonMachine(int index) const;
};

#endif