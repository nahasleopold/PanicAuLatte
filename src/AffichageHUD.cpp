#include "AffichageHUD.h"
#include "AffichageTexte.h"
#include "jeu.h"
#include "cafe.h"
#include <string>

/// dessin des infos generales (heure, etat cafe, thune, reputation)
void AffichageHUD::afficher(SDL_Renderer* renderer, TTF_Font* font, const Jeu& jeu, const Cafe& cafe, SDLSprite& spriteTitre, SDLSprite& spriteHeure, SDLSprite& spriteOuvert, SDLSprite& spriteFerme, SDLSprite& spriteArgentHud, SDLSprite& spriteReputation) const
{
    SDL_Color blanc = {255, 255, 255, 255};
    
    /// affichage du logo/titre du jeu
    spriteTitre.draw(renderer, 700, 30, 260, 100);
    int yHud = 40;

    /// affichage horloge avec formatage 00:00
    spriteHeure.draw(renderer, 100, yHud, 210, 50);
    std::string texteHeure = (jeu.getHeure() < 10 ? "0" : "") + std::to_string(jeu.getHeure()) + ":" + (jeu.getMinute() < 10 ? "0" : "") + std::to_string(jeu.getMinute());
    AffichageTexte::dessinerTexte(renderer, font, texteHeure, 200, yHud + 12, blanc);

    /// indicateur visuel ouvert ou ferme
    if (jeu.cafeEstFerme()) spriteFerme.draw(renderer, 400, yHud, 210, 60);
    else spriteOuvert.draw(renderer, 400, yHud, 210, 60);

    /// rendu du solde actuel
    spriteArgentHud.draw(renderer, 1000, yHud, 210, 50);
    AffichageTexte::dessinerTexte(renderer, font, std::to_string((int)cafe.getArgent()), 1090, yHud + 12, blanc);

    /// rendu du score de reputation
    spriteReputation.draw(renderer, 1310, yHud, 210, 50);
    AffichageTexte::dessinerTexte(renderer, font, std::to_string(cafe.getReputation()), 1380, yHud + 12, blanc);
}