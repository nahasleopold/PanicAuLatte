#ifndef BOUTIQUE_H
#define BOUTIQUE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "sdlsprite.h"
#include "Machine.h" /// Ajouté pour TypeMachine

class SDLJeu;
class Jeu;

/// definition de la fenetre d'achats du cafe
class Boutique {
private:
    /// zones de clic invisibles pour l'interface
    SDL_Rect boutonTable;
    SDL_Rect boutonsServeurs[5];
    SDL_Rect boutonsMachines[3]; /// Nouveau : boutons pour Matcha, Jus, Dessert
    
    /// config des tarifs in-game
    float prixServeurs[5] = {30.0f, 40.0f, 50.0f, 60.0f, 70.0f};
    float prixMachines[3] = {100.0f, 150.0f, 200.0f}; /// Prix respectifs

    /// utilitaires internes pour la souris et le texte
    bool pointDansRect(int x, int y, const SDL_Rect& rect) const;
    void dessinerTexte(SDL_Renderer* renderer, TTF_Font* font,
                       const std::string& texte, int x, int y, SDL_Color couleur) const;

public:
    /// calcul des positions de la grille
    Boutique();
    
    /// detection de l'item clique
    /// Codes de retour : 1=Retour, 2=Table, 10-14=Serveurs, 20-22=Machines
    int gererEvenement(const SDL_Event& event, bool& quit);
    
    /// boucle logique d'achat et debit de la thune
    void update(SDLJeu& sdlJeu, Jeu& jeu, bool& quit);
    
    /// dessin de tous les boutons et prix a l'ecran
    void afficher(SDL_Renderer* renderer, TTF_Font* font, const SDLSprite& spriteTable, const SDLSprite spriteServeurs[5], float argent, SDLJeu& sdlJeu);
};

#endif