#include "AffichageMenu.h"
#include "AffichageTexte.h"
#include "OutilsClic.h"

/// setup des zones de clic pour les boutons du menu
AffichageMenu::AffichageMenu()
{
    int largeurFenetre = 1700;
    int largeurBouton = 520;
    int hauteurBouton = 95;
    int xCentre = (largeurFenetre - largeurBouton) / 2;
    
    /// calcul des positions centrees pour chaque bouton
    boutonJouer = {xCentre, 450, largeurBouton, hauteurBouton};
    boutonBoutique = {xCentre, 585, largeurBouton, hauteurBouton};
    boutonQuitter = {xCentre, 720, largeurBouton, hauteurBouton};
}

/// chargement des images des boutons
void AffichageMenu::chargerSprites(SDL_Renderer* renderer)
{
    spriteContinuer.loadFromFile("data/continuer.png", renderer);
    spriteBoutique.loadFromFile("data/boutique.png", renderer);
    spriteQuitter.loadFromFile("data/quitter.png", renderer);
}

void AffichageMenu::libererSprites()
{
    spriteContinuer.release();
    spriteBoutique.release();
    spriteQuitter.release();
}

/// rendu du menu avec gestion du survol (hover)
void AffichageMenu::afficher(SDL_Renderer* renderer, TTF_Font* font, SDLSprite& fondMenu, bool cafeFerme) const
{
    /// dessin du background
    fondMenu.draw(renderer, 0, 0, 1700, 1000);
    
    /// detection de la souris pour l'effet visuel au survol
    int sourisX, sourisY;
    SDL_GetMouseState(&sourisX, &sourisY);
    bool hoverJouer = OutilsClic::pointDansRect(sourisX, sourisY, boutonJouer);
    bool hoverBoutique = OutilsClic::pointDansRect(sourisX, sourisY, boutonBoutique);
    bool hoverQuitter = OutilsClic::pointDansRect(sourisX, sourisY, boutonQuitter);

    /// affichage des boutons (legerement plus grands si survolés)
    if (hoverJouer) spriteContinuer.draw(renderer, boutonJouer.x - 6, boutonJouer.y - 4, boutonJouer.w + 12, boutonJouer.h + 8);
    else spriteContinuer.draw(renderer, boutonJouer.x, boutonJouer.y, boutonJouer.w, boutonJouer.h);

    if (hoverBoutique) spriteBoutique.draw(renderer, boutonBoutique.x - 6, boutonBoutique.y - 4, boutonBoutique.w + 12, boutonBoutique.h + 8);
    else spriteBoutique.draw(renderer, boutonBoutique.x, boutonBoutique.y, boutonBoutique.w, boutonBoutique.h);

    if (hoverQuitter) spriteQuitter.draw(renderer, boutonQuitter.x - 6, boutonQuitter.y - 4, boutonQuitter.w + 12, boutonQuitter.h + 8);
    else spriteQuitter.draw(renderer, boutonQuitter.x, boutonQuitter.y, boutonQuitter.w, boutonQuitter.h);

    /// message d'info si la boutique est verrouillee
    if (!cafeFerme)
    {
        SDL_Color blanc = {255, 255, 255, 255};
        AffichageTexte::dessinerTexte(renderer, font, "Boutique accessible seulement quand le cafe est ferme", 600, 920, blanc);
    }
}

/// getters pour la gestion des clics dans le main
SDL_Rect AffichageMenu::getBoutonJouer() const { return boutonJouer; }
SDL_Rect AffichageMenu::getBoutonBoutique() const { return boutonBoutique; }
SDL_Rect AffichageMenu::getBoutonQuitter() const { return boutonQuitter; }