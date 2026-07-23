#include "boutique.h"
#include "sdljeu.h"
#include "jeu.h"

/// init des zones de clic pour les items de la boutique
Boutique::Boutique()
{
    int xDepart = 320;
    int yDepart = 180;     
    int espacementX = 200; 
    int espacementY = 220; 
    int taille = 130; 

    boutonTable = {xDepart, yDepart, taille, taille};

    /// Boutons Serveurs (Indices 1 à 5 dans la grille)
    for (int i = 0; i < 5; i++) 
    {
        int indexGrille = i + 1; 
        int col = indexGrille % 5;
        int lig = indexGrille / 5;
        boutonsServeurs[i] = {xDepart + (col * espacementX), yDepart + (lig * espacementY), taille, taille};
    }

    /// Boutons Machines (Indices 6 à 8 dans la grille)
    for (int i = 0; i < 3; i++)
    {
        int indexGrille = i + 6;
        int col = indexGrille % 5;
        int lig = indexGrille / 5;
        boutonsMachines[i] = {xDepart + (col * espacementX), yDepart + (lig * espacementY), taille, taille};
    }
}

/// verif de la position de la souris par rapport a un bouton
bool Boutique::pointDansRect(int x, int y, const SDL_Rect& rect) const
{
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

/// generation et affichage d'un texte sdl
void Boutique::dessinerTexte(SDL_Renderer* renderer, TTF_Font* font,
                             const std::string& texte, int x, int y, SDL_Color couleur) const
{
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, texte.c_str(), couleur);
    if (surface == nullptr) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = surface->w;
    dest.h = surface->h;

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &dest);
    SDL_DestroyTexture(texture);
}

/// recup et tri des clics dans l'interface
int Boutique::gererEvenement(const SDL_Event& event, bool& quit)
{
    if (event.type == SDL_QUIT) { quit = true; return 3; }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) return 1;

    /// detection du clic gauche sur les differents items
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        int x = event.button.x;
        int y = event.button.y;

        if (pointDansRect(x, y, boutonTable)) return 2;

        for (int i = 0; i < 5; i++) {
            if (pointDansRect(x, y, boutonsServeurs[i])) return 10 + i;
        }

        for (int i = 0; i < 3; i++) {
            if (pointDansRect(x, y, boutonsMachines[i])) return 20 + i;
        }
    }
    return 0;
}

/// boucle principale de gestion des achats
void Boutique::update(SDLJeu& sdlJeu, Jeu& jeu, bool& quit)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        int choix = gererEvenement(event, quit);

        /// retour au jeu sans achat
        if (choix == 1) {
            jeu.setEtatInterface(Jeu::JEU);
            return;
        }
        /// process d'achat d'une table
        else if (choix == 2) {
            if (jeu.getCafe().acheterElement(25.0f)) { 
                jeu.ajouterTableEnAttente(); 
                jeu.setEtatInterface(Jeu::JEU);
                return;
            }
        }
        /// process d'achat d'un serveur selon le skin
        else if (choix >= 10 && choix <= 14) {
            int skin = choix - 10; 
            if (jeu.getCafe().acheterElement(prixServeurs[skin])) {
                jeu.getCafe().recruterServeur(skin);
            }
        }
        /// process d'achat d'une machine et mode placement
        else if (choix >= 20 && choix <= 22) {
            int idx = choix - 20;
            if (jeu.getCafe().acheterElement(prixMachines[idx])) {
                /// 1. On ajoute au stock
                jeu.ajouterMachineEnAttente(static_cast<TypeMachine>(idx + 1));
                
                /// 2. ON RENVOIE AU JEU IMMÉDIATEMENT
                jeu.setEtatInterface(Jeu::JEU); 
                return;
    }
}
    }
    /// maj visuelle de la boutique
    afficher(sdlJeu.getRenderer(), sdlJeu.getFont(), sdlJeu.getSpriteTable(), sdlJeu.getSpriteServeurs(), jeu.getConstCafe().getArgent(), sdlJeu);
    SDL_RenderPresent(sdlJeu.getRenderer());
}

/// rendu graphique complet du magasin
void Boutique::afficher(SDL_Renderer* renderer, TTF_Font* font, const SDLSprite& spriteTable, const SDLSprite spriteServeurs[5], float argent, SDLJeu& sdlJeu)
{
    sdlJeu.getSpriteFondBoutique().draw(renderer, 0, 0, 1700, 1000); 

    int xDepart = 320; int yDepart = 180; int espacementX = 200; int espacementY = 220; int tailleCadre = 130; 
    SDL_Color marronCadre = {60, 35, 20, 180}; SDL_Color orBordure = {218, 165, 32, 255};
    SDL_Color blanc = {255, 255, 255, 255}; SDL_Color jauneOr = {255, 215, 0, 255};

    /// creation de la grille des 9 cases dispo
    for (int i = 0; i < 9; i++) /// 1 table + 5 serveurs + 3 machines = 9
    {
        int col = i % 5; int lig = i / 5;
        int posX = xDepart + (col * espacementX);
        int posY = yDepart + (lig * espacementY);
        SDL_Rect rectCadre = {posX, posY, tailleCadre, tailleCadre};

        SDL_SetRenderDrawColor(renderer, marronCadre.r, marronCadre.g, marronCadre.b, marronCadre.a);
        SDL_RenderFillRect(renderer, &rectCadre);
        SDL_SetRenderDrawColor(renderer, orBordure.r, orBordure.g, orBordure.b, 255);
        SDL_RenderDrawRect(renderer, &rectCadre);

        /// affichage specifique selon l'element (table, perso ou machine)
        if (i == 0) {
            spriteTable.draw(renderer, posX + 10, posY + 10, 110, 110);
            dessinerTexte(renderer, font, "25$", posX + 45, posY + tailleCadre + 5, blanc);
        } 
        else if (i >= 1 && i <= 5) {
            /// NOUVELLE VERSION : On utilise drawFrame pour découper la case "Face / Statique"
            /// Ligne 0, Colonne 0, Grille 3x5
            spriteServeurs[i-1].drawFrame(renderer, posX + 10, posY + 10, 110, 110, 0, 0, 3, 5, false);
            
            dessinerTexte(renderer, font, std::to_string((int)prixServeurs[i-1]) + "$", posX + 45, posY + tailleCadre + 5, blanc);
        }
        else {
            int idxMach = i - 6;
            /// Utilisation des sprites de machines via sdlJeu
            if (idxMach == 0) sdlJeu.getSpriteMachineMatcha().draw(renderer, posX + 10, posY + 10, 110, 110);
            else if (idxMach == 1) sdlJeu.getSpriteMachineJus().draw(renderer, posX + 10, posY + 10, 110, 110);
            else if (idxMach == 2) sdlJeu.getSpriteMachineDessert().draw(renderer, posX + 10, posY + 10, 110, 110);
            
            dessinerTexte(renderer, font, std::to_string((int)prixMachines[idxMach]) + "$", posX + 45, posY + tailleCadre + 5, blanc);
        }
    }

    /// Affichage Argent
    SDL_Rect rectArgent = {1250, 30, 320, 60};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_RenderFillRect(renderer, &rectArgent);
    dessinerTexte(renderer, font, "Argent: " + std::to_string((int)argent) + " $", 1270, 45, jauneOr);
}