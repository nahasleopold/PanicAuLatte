#ifndef AFFICHAGETEXTE_H
#define AFFICHAGETEXTE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

/// boite a outils pour simplifier l'affichage du texte
class AffichageTexte
{
public:
    /// generation et rendu d'une ligne de texte a l'ecran
    static void dessinerTexte(SDL_Renderer* renderer, TTF_Font* font, const std::string& texte, int x, int y, SDL_Color couleur)
    {
        /// creation d'une surface cpu avec le texte fondu (lisse)
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font, texte.c_str(), couleur);
        if (surface == nullptr) return;

        /// conversion en texture gpu pour un affichage rapide
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr)
        {
            SDL_FreeSurface(surface);
            return;
        }

        /// recup de la taille du texte et placement a la bonne position
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        dest.w = surface->w;
        dest.h = surface->h;

        /// menage en memoire et copie finale sur le rendu
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
    }
};

#endif