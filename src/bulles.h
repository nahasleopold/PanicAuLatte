#ifndef BULLES_H
#define BULLES_H

#include <SDL2/SDL.h>
#include "sdlsprite.h"
#include "EnsembleClients.h"
#include "produit.h"

/// gestion logique et visuelle des bulles au dessus des clients
class Bulles
{
private:
    /// stockage des textures pour les icones et les bulles
    SDLSprite spriteBulleCommande;
    SDLSprite spritePiece;
    SDLSprite spriteCafe;
    SDLSprite spriteMatcha;
    SDLSprite spriteCarrotCake;
    SDLSprite spriteJusOrange;
    SDLSprite spriteDonut;

    /// methodes internes pour la logique de clic et de positionnement
    bool pointDansRect(int x, int y, const SDL_Rect& rect) const;
    SDL_Rect getRectBulleCommande(const Client& client) const;
    SDL_Rect getRectBulleArgent(const Client& client) const;
    void afficherSpriteProduit(SDL_Renderer* renderer, TypeProduit type, int x, int y);

public:
    /// init de l'objet
    Bulles();
    
    /// chargement initial des assets graphiques
    void chargerSprites(SDL_Renderer* renderer);
    
    /// methodes de rendu pour l'ui en cours de partie
    void afficherBullesCommandes(SDL_Renderer* renderer, const EnsembleClients& ensembleClients);
    void afficherBullesArgent(SDL_Renderer* renderer, const EnsembleClients& ensembleClients);
    
    /// recup du client cible lors d'un clic sur une bulle
    Client* getClientCliqueCommande(int x, int y, EnsembleClients& ensembleClients);
    Client* getClientCliqueArgent(int x, int y, EnsembleClients& ensembleClients);
};

#endif