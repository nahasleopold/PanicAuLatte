#ifndef AFFICHAGEBULLES_H
#define AFFICHAGEBULLES_H

#include <SDL2/SDL.h>
#include "sdlsprite.h"
#include "EnsembleClients.h"
#include "produit.h"

/// gestion de l'affichage des bulles de texte et des items au dessus des clients
class AffichageBulles
{
private:
    /// stockage des textures pour les bulles, l'argent et les produits
    SDLSprite spriteBulleCommande;
    SDLSprite spritePiece;
    SDLSprite spriteCafe;
    SDLSprite spriteMatcha;
    SDLSprite spriteCarrotCake;
    SDLSprite spriteJusOrange;
    SDLSprite spriteDonut;

    /// methodes internes pour calculer les positions de rendu
    SDL_Rect getRectBulleCommande(const Client& client) const;
    SDL_Rect getRectBulleArgent(const Client& client) const;
    void afficherSpriteProduit(SDL_Renderer* renderer, TypeProduit type, int x, int y);

public:
    AffichageBulles();
    
    /// chargement initial des fichiers images
    void chargerSprites(SDL_Renderer* renderer);
    void libererSprites();
    
    /// fonctions de dessin pour les differentes etapes (commande et paiement)
    void afficherBullesCommandes(SDL_Renderer* renderer, const EnsembleClients& ensembleClients);
    void afficherBullesArgent(SDL_Renderer* renderer, const EnsembleClients& ensembleClients);
    
    /// recup des zones de clic pour l'interaction avec le joueur
    SDL_Rect getRectCommandeClient(const Client& client) const;
    SDL_Rect getRectArgentClient(const Client& client) const;
};

#endif