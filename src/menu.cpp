#include "menu.h"
#include "sdljeu.h"
#include "jeu.h"

/// centrage des boutons et definition des zones de clic
Menu::Menu()
{
    int largeurFenetre = 1700;
    int largeurBouton = 520;
    int hauteurBouton = 95;
    int xCentre = (largeurFenetre - largeurBouton) / 2;

    /// boutons plus bas
    boutonJouer = {xCentre, 450, largeurBouton, hauteurBouton};
    boutonBoutique = {xCentre, 585, largeurBouton, hauteurBouton};
    boutonQuitter = {xCentre, 720, largeurBouton, hauteurBouton};
}

/// importation des ressources graphiques pour l'interface
void Menu::chargerSprites(SDL_Renderer* renderer)
{
    spriteContinuer.loadFromFile("data/continuer.png", renderer);
    spriteBoutique.loadFromFile("data/boutique.png", renderer);
    spriteQuitter.loadFromFile("data/quitter.png", renderer);
}

/// utilitaire de detection de collision point contre rectangle
bool Menu::pointDansRect(int x, int y, const SDL_Rect& rect) const
{
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

/// rendu de texte via sdl_ttf avec gestion de texture
void Menu::dessinerTexte(SDL_Renderer* renderer, TTF_Font* font,
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

/// traitement des signaux clavier et souris specifiques au menu
int Menu::gererEvenement(const SDL_Event& event, bool cafeFerme, bool& quit)
{
    if (event.type == SDL_QUIT)
    {
        quit = true;
        return 3;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        int x = event.button.x;
        int y = event.button.y;

        if (pointDansRect(x, y, boutonJouer))
            return 1;

        if (pointDansRect(x, y, boutonBoutique) && cafeFerme)
            return 2;

        if (pointDansRect(x, y, boutonQuitter))
        {
            quit = true;
            return 3;
        }
    }

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_RETURN:
                return 1;

            case SDLK_b:
                if (cafeFerme)
                    return 2;
                break;

            case SDLK_ESCAPE:
            case SDLK_q:
                quit = true;
                return 3;

            default:
                break;
        }
    }

    return 0;
}

/// boucle de mise a jour et de rafraichissement du menu
void Menu::update(SDLJeu& sdlJeu, Jeu& jeu, bool& quit)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        int choix = gererEvenement(event, jeu.cafeEstFerme(), quit);

        if (choix == 1)
        {
            jeu.setEtatInterface(Jeu::JEU);
            return;
        }
        else if (choix == 2)
        {
            jeu.setEtatInterface(Jeu::BOUTIQUE);
            return;
        }
        else if (choix == 3)
        {
            quit = true;
            return;
        }
    }

    afficher(sdlJeu.getRenderer(), sdlJeu.getFont(), sdlJeu.getFondMenu(), jeu.cafeEstFerme());
    SDL_RenderPresent(sdlJeu.getRenderer());
}

/// affichage du fond et des boutons avec effet de survol dynamique
void Menu::afficher(SDL_Renderer* renderer, TTF_Font* font, SDLSprite& fondMenu, bool cafeFerme) const
{
    fondMenu.draw(renderer, 0, 0, 1700, 1000);

    int sourisX, sourisY;
    SDL_GetMouseState(&sourisX, &sourisY);

    bool hoverJouer = pointDansRect(sourisX, sourisY, boutonJouer);
    bool hoverBoutique = pointDansRect(sourisX, sourisY, boutonBoutique);
    bool hoverQuitter = pointDansRect(sourisX, sourisY, boutonQuitter);

    /// calcul de l'agrandissement pour l'effet visuel de focus
    if (hoverJouer)
        spriteContinuer.draw(renderer, boutonJouer.x - 6, boutonJouer.y - 4, boutonJouer.w + 12, boutonJouer.h + 8);
    else
        spriteContinuer.draw(renderer, boutonJouer.x, boutonJouer.y, boutonJouer.w, boutonJouer.h);

    if (hoverBoutique)
        spriteBoutique.draw(renderer, boutonBoutique.x - 6, boutonBoutique.y - 4, boutonBoutique.w + 12, boutonBoutique.h + 8);
    else    
        spriteBoutique.draw(renderer, boutonBoutique.x, boutonBoutique.y, boutonBoutique.w, boutonBoutique.h);

    if (hoverQuitter)
        spriteQuitter.draw(renderer, boutonQuitter.x - 6, boutonQuitter.y - 4, boutonQuitter.w + 12, boutonQuitter.h + 8);
    else
        spriteQuitter.draw(renderer, boutonQuitter.x, boutonQuitter.y, boutonQuitter.w, boutonQuitter.h);

    /// message d'information si la boutique est verrouillee
    if (!cafeFerme)
    {
        SDL_Color blanc = {255, 255, 255, 255};
        dessinerTexte(renderer, font,
                      "Boutique accessible seulement quand le cafe est ferme",
                      500, 840, blanc);
    }
}