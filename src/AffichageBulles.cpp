#include "AffichageBulles.h"
#include "OutilsClic.h"

AffichageBulles::AffichageBulles() {}

/// chargement des images 
void AffichageBulles::chargerSprites(SDL_Renderer* renderer)
{
    spriteBulleCommande.loadFromFile("data/bulle.png", renderer);
    spritePiece.loadFromFile("data/argent.png", renderer);
    spriteCafe.loadFromFile("data/cafe.png", renderer);
    spriteMatcha.loadFromFile("data/matcha.png", renderer);
    spriteCarrotCake.loadFromFile("data/carrotcake.png", renderer);
    spriteJusOrange.loadFromFile("data/jusdorange.png", renderer);
    spriteDonut.loadFromFile("data/donut.png", renderer);
}

void AffichageBulles::libererSprites()
{
    spriteBulleCommande.release();
    spritePiece.release();
    spriteCafe.release();
    spriteMatcha.release();
    spriteCarrotCake.release();
    spriteJusOrange.release();
    spriteDonut.release();
}

/// calcul de la position de la bulle de commande au dessus du client
SDL_Rect AffichageBulles::getRectBulleCommande(const Client& client) const
{
    Coordonnee pos = client.getPositionC();
    int xClientAff = (int)pos.getX() - 65;
    int yClientAff = (int)pos.getY() - 105;
    SDL_Rect rect{ xClientAff + 10, yClientAff - 20, 130, 80 };
    return rect;
}

/// calcul de la position de la piece de monnaie au dessus du client
SDL_Rect AffichageBulles::getRectBulleArgent(const Client& client) const
{
    Coordonnee pos = client.getPositionC();
    int xClientAff = (int)pos.getX() - 65;
    int yClientAff = (int)pos.getY() - 105;
    SDL_Rect rect{ xClientAff + 30, yClientAff - 10, 60, 60 };
    return rect;
}

/// rendu de l'icone du produit selon son type
void AffichageBulles::afficherSpriteProduit(SDL_Renderer* renderer, TypeProduit type, int x, int y)
{
    switch (type)
    {
        case CAFE: spriteCafe.draw(renderer, x, y, 26, 26); break;
        case MATCHA: spriteMatcha.draw(renderer, x, y, 26, 26); break;
        case CARROT_CAKE: spriteCarrotCake.draw(renderer, x, y, 26, 26); break;
        case JUS_ORANGE: spriteJusOrange.draw(renderer, x, y, 26, 26); break;
        case DONUT: spriteDonut.draw(renderer, x, y, 26, 26); break;
    }
}

/// affichage des bulles de commande pour les clients qui attendent
void AffichageBulles::afficherBullesCommandes(SDL_Renderer* renderer, const EnsembleClients& ensembleClients)
{
    const std::vector<Client>& clients = ensembleClients.getConstClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        const Client& client = clients[i];
        /// verif si le client est assis et attend sa commande
        if (!client.getAssis()) continue;
        if (!client.aUneCommande()) continue;
        if (client.getCommandeCliquee()) continue;
        if (client.getEtat() != ASSIS_ATTEND_COMMANDE) continue;

        SDL_Rect rectBulle = getRectBulleCommande(client);
        spriteBulleCommande.draw(renderer, rectBulle.x, rectBulle.y, rectBulle.w, rectBulle.h);

        /// dessin des icones des produits dans la bulle
        const std::vector<Produit>& produits = client.getCommande().getProduits();
        for (size_t j = 0; j < produits.size(); j++)
        {
            int xIcone = rectBulle.x + 15 + (int)j * 32;
            int yIcone = rectBulle.y + 18;
            afficherSpriteProduit(renderer, produits[j].getType(), xIcone, yIcone);
        }
    }
}

/// affichage des pieces quand le client a fini et doit payer
void AffichageBulles::afficherBullesArgent(SDL_Renderer* renderer, const EnsembleClients& ensembleClients)
{
    const std::vector<Client>& clients = ensembleClients.getConstClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        const Client& client = clients[i];
        if (client.getEtat() != ATTEND_ARGENT) continue;
        if (client.getArgentClique()) continue;
        SDL_Rect rect = getRectBulleArgent(client);
        spritePiece.draw(renderer, rect.x, rect.y, rect.w, rect.h);
    }
}

/// getters pour recup les zones de clic des bulles
SDL_Rect AffichageBulles::getRectCommandeClient(const Client& client) const
{
    return getRectBulleCommande(client);
}

SDL_Rect AffichageBulles::getRectArgentClient(const Client& client) const
{
    return getRectBulleArgent(client);
}