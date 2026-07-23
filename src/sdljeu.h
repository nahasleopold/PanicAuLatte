#ifndef SDLJEU_H
#define SDLJEU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "jeu.h"
#include "sdlsprite.h"
#include "AffichageMenu.h"
#include "AffichageBoutique.h"
#include "AffichageBulles.h"
#include "AffichageHUD.h"
#include "AffichageCafe.h"
#include <string>

class SDLJeu {
private:
    Jeu jeu;
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool quit;

    SDLSprite fond;
    SDLSprite fondNuit;
    SDLSprite fondMenu;
    SDLSprite spriteClient;
    SDLSprite spriteServeurs[5];
    SDLSprite spriteTable;
    SDLSprite spriteChaiseGauche;
    SDLSprite spriteChaiseDroite;
    SDLSprite spriteFileAttente;
    SDLSprite spriteMachineCafe;
    SDLSprite spriteMachineMatcha;
    SDLSprite spriteMachineJus;
    SDLSprite spriteMachineDessert;
    SDLSprite spriteFondBoutique; 

    /// HUD sprites
    SDLSprite spriteHeure;
    SDLSprite spriteOuvert;
    SDLSprite spriteFerme;
    SDLSprite spriteReputation;
    SDLSprite spriteTitre;
    SDLSprite spriteArgentHud;

    TTF_Font* font;


    AffichageMenu affichageMenu;
    AffichageBoutique affichageBoutique;
    AffichageBulles affichageBulles;
    AffichageHUD affichageHUD;
    AffichageCafe affichageCafe;

    /// zone cliquable du sprite ouvert / ferme
    SDL_Rect rectEtatCafe;


public:
    SDLJeu();
    ~SDLJeu();

    void sdlBoucle();
    void sdlAff();

    SDL_Renderer* getRenderer();
    TTF_Font* getFont();
    AffichageBulles& getAffichageBulles();
    SDLSprite& getSpriteTable();
    SDLSprite* getSpriteServeurs();
    SDLSprite& getFondMenu();
    Jeu& getJeu();

    SDL_Rect getRectEtatCafe() const;
    SDLSprite& getSpriteFondBoutique() { return spriteFondBoutique; }
    SDLSprite& getSpriteMachineMatcha() { return spriteMachineMatcha; }
    SDLSprite& getSpriteMachineJus() { return spriteMachineJus; }
    SDLSprite& getSpriteMachineDessert() { return spriteMachineDessert; }
};

#endif