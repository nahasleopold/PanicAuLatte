#include "AffichageBoutique.h"
#include "AffichageTexte.h"
#include "sdljeu.h"
#include <string>

/// constructeur pour setup la grille des boutons de la boutique
AffichageBoutique::AffichageBoutique()
{
    int xDepart = 320;
    int yDepart = 180;
    int espacementX = 200;
    int espacementY = 220;
    int taille = 130;

    /// init zone cliquable pour achat de table
    boutonTable = {xDepart, yDepart, taille, taille};

    /// calcul positions des boutons serveurs sur la grille
    for (int i = 0; i < 5; i++)
    {
        int indexGrille = i + 1;
        int col = indexGrille % 5;
        int lig = indexGrille / 5;
        boutonsServeurs[i] = {xDepart + (col * espacementX), yDepart + (lig * espacementY), taille, taille};
    }

    /// calcul positions des boutons machines sur la grille
    for (int i = 0; i < 3; i++)
    {
        int indexGrille = i + 6;
        int col = indexGrille % 5;
        int lig = indexGrille / 5;
        boutonsMachines[i] = {xDepart + (col * espacementX), yDepart + (lig * espacementY), taille, taille};
    }
}

/// rendu visuel complet de l'interface boutique
void AffichageBoutique::afficher(SDL_Renderer* renderer, TTF_Font* font, const SDLSprite& spriteTable, const SDLSprite spriteServeurs[5], float argent, SDLJeu& sdlJeu)
{
    /// config des tarifs
    float prixServeurs[5] = {30.0f, 40.0f, 50.0f, 60.0f, 70.0f};
    float prixMachines[3] = {100.0f, 150.0f, 200.0f};

    /// affichage du background boutique
    sdlJeu.getSpriteFondBoutique().draw(renderer, 0, 0, 1700, 1000);
    
    /// config visuelle des cadres et couleurs
    int xDepart = 320; int yDepart = 180; int espacementX = 200; int espacementY = 220; int tailleCadre = 130;
    SDL_Color marronCadre = {60, 35, 20, 180}; SDL_Color orBordure = {218, 165, 32, 255};
    SDL_Color blanc = {255, 255, 255, 255}; SDL_Color jauneOr = {255, 215, 0, 255};

    /// boucle de dessin des slots d'achat (9 au total)
    for (int i = 0; i < 9; i++)
    {
        int col = i % 5; int lig = i / 5;
        int posX = xDepart + (col * espacementX);
        int posY = yDepart + (lig * espacementY);
        SDL_Rect rectCadre = {posX, posY, tailleCadre, tailleCadre};
        
        /// dessin du fond et de la bordure du slot
        SDL_SetRenderDrawColor(renderer, marronCadre.r, marronCadre.g, marronCadre.b, marronCadre.a);
        SDL_RenderFillRect(renderer, &rectCadre);
        SDL_SetRenderDrawColor(renderer, orBordure.r, orBordure.g, orBordure.b, 255);
        SDL_RenderDrawRect(renderer, &rectCadre);

        /// affichage specifique selon l'item (table, serveur ou machine)
        if (i == 0)
        {
            spriteTable.draw(renderer, posX + 10, posY + 10, 110, 110);
            AffichageTexte::dessinerTexte(renderer, font, "25$", posX + 45, posY + tailleCadre + 5, blanc);
        }
        else if (i >= 1 && i <= 5)
        {
            spriteServeurs[i - 1].drawFrame(renderer, posX + 10, posY + 10, 110, 110, 0, 0, 3, 5, false);
            AffichageTexte::dessinerTexte(renderer, font, std::to_string((int)prixServeurs[i - 1]) + "$", posX + 45, posY + tailleCadre + 5, blanc);
        }
        else
        {
            int idxMach = i - 6;
            if (idxMach == 0) sdlJeu.getSpriteMachineMatcha().draw(renderer, posX + 10, posY + 10, 110, 110);
            else if (idxMach == 1) sdlJeu.getSpriteMachineJus().draw(renderer, posX + 10, posY + 10, 110, 110);
            else if (idxMach == 2) sdlJeu.getSpriteMachineDessert().draw(renderer, posX + 10, posY + 10, 110, 110);
            AffichageTexte::dessinerTexte(renderer, font, std::to_string((int)prixMachines[idxMach]) + "$", posX + 45, posY + tailleCadre + 5, blanc);
        }
    }

    /// bandeau d'affichage de la thune actuelle
    SDL_Rect rectArgent = {1250, 30, 320, 60};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_RenderFillRect(renderer, &rectArgent);
    AffichageTexte::dessinerTexte(renderer, font, "Argent: " + std::to_string((int)argent) + " $", 1270, 45, jauneOr);
}

/// getters pour recup les zones de clic des differents boutons
SDL_Rect AffichageBoutique::getBoutonTable() const { return boutonTable; }
SDL_Rect AffichageBoutique::getBoutonServeur(int index) const { return boutonsServeurs[index]; }
SDL_Rect AffichageBoutique::getBoutonMachine(int index) const { return boutonsMachines[index]; }