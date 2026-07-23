#ifndef AFFICHAGEMENU_H
#define AFFICHAGEMENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "sdlsprite.h"

/// gestion de l'interface du menu principal
class AffichageMenu
{
private:
    /// boites de collision pour les clics
    SDL_Rect boutonJouer;
    SDL_Rect boutonBoutique;
    SDL_Rect boutonQuitter;

    /// stockage des textures des boutons
    SDLSprite spriteContinuer;
    SDLSprite spriteBoutique;
    SDLSprite spriteQuitter;

public:
    /// calcul des positions des boutons au centre de l'ecran
    AffichageMenu();
    
    /// chargement des fichiers images depuis le disque
    void chargerSprites(SDL_Renderer* renderer);
    void libererSprites();
    
    /// dessin du menu et gestion de l'effet de survol souris
    void afficher(SDL_Renderer* renderer, TTF_Font* font, SDLSprite& fondMenu, bool cafeFerme) const;
    
    /// recup des coordonnees des boutons pour le clic
    SDL_Rect getBoutonJouer() const;
    SDL_Rect getBoutonBoutique() const;
    SDL_Rect getBoutonQuitter() const;
};

#endif