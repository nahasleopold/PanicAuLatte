#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "sdlsprite.h"

class SDLJeu;
class Jeu;

class Menu
{
private:
    SDL_Rect boutonJouer;
    SDL_Rect boutonBoutique;
    SDL_Rect boutonQuitter;

    SDLSprite spriteContinuer;
    SDLSprite spriteBoutique;
    SDLSprite spriteQuitter;

    bool pointDansRect(int x, int y, const SDL_Rect& rect) const;
    void dessinerTexte(SDL_Renderer* renderer, TTF_Font* font,
                       const std::string& texte, int x, int y, SDL_Color couleur) const;

public:
    Menu();

    void chargerSprites(SDL_Renderer* renderer);

    /// Retour :
    /// 0 = rien
    /// 1 = aller au jeu
    /// 2 = aller a la boutique
    /// 3 = quitter
    int gererEvenement(const SDL_Event& event, bool cafeFerme, bool& quit);

    void update(SDLJeu& sdlJeu, Jeu& jeu, bool& quit);

    void afficher(SDL_Renderer* renderer, TTF_Font* font, SDLSprite& fondMenu, bool cafeFerme) const;
};

#endif