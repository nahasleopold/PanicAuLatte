#include "bulles.h"

/// constructeur vide par defaut
Bulles::Bulles() {}

/// verif si les coordonnees du clic sont dans la boite
bool Bulles::pointDansRect(int x, int y, const SDL_Rect& rect) const
{
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}

/// chargement en memoire des images pour l'ui des bulles
void Bulles::chargerSprites(SDL_Renderer* renderer)
{
    spriteBulleCommande.loadFromFile("data/bulle.png", renderer);
    spritePiece.loadFromFile("data/argent.png", renderer);
    spriteCafe.loadFromFile("data/cafe.png", renderer);
    spriteMatcha.loadFromFile("data/matcha.png", renderer);
    spriteCarrotCake.loadFromFile("data/carrotcake.png", renderer);
    spriteJusOrange.loadFromFile("data/jusdorange.png", renderer);
    spriteDonut.loadFromFile("data/donut.png", renderer);
}

/// calcul de la position de la bulle de texte selon la pos du client
SDL_Rect Bulles::getRectBulleCommande(const Client& client) const
{
    Coordonnee pos = client.getPositionC();
    int xClientAff = (int)pos.getX() - 65;
    int yClientAff = (int)pos.getY() - 105;
    SDL_Rect rect{ xClientAff + 10, yClientAff - 20, 130, 80 };
    return rect;
}

/// calcul de la position de la piece a recup
SDL_Rect Bulles::getRectBulleArgent(const Client& client) const
{
    Coordonnee pos = client.getPositionC();
    int xClientAff = (int)pos.getX() - 65;
    int yClientAff = (int)pos.getY() - 105;
    SDL_Rect rect{ xClientAff + 30, yClientAff - 10, 60, 60 };
    return rect;
}

/// dessin specifique de l'icone selon l'enum du produit
void Bulles::afficherSpriteProduit(SDL_Renderer* renderer, TypeProduit type, int x, int y)
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

/// rendu visuel de toutes les commandes en cours dans le cafe
void Bulles::afficherBullesCommandes(SDL_Renderer* renderer, const EnsembleClients& ensembleClients)
{
    const std::vector<Client>& clients = ensembleClients.getConstClients();
    for (size_t i = 0; i < clients.size(); i++)
    {
        const Client& client = clients[i];
        
        /// filtres pour s'assurer que le client a bien besoin d'une bulle
        if (!client.getAssis()) continue;
        if (!client.aUneCommande()) continue;
        if (client.getCommandeCliquee()) continue;
        if (client.getEtat() != ASSIS_ATTEND_COMMANDE) continue;

        SDL_Rect rectBulle = getRectBulleCommande(client);
        spriteBulleCommande.draw(renderer, rectBulle.x, rectBulle.y, rectBulle.w, rectBulle.h);

        /// boucle de dessin pour chaque item de la commande du client
        const std::vector<Produit>& produits = client.getCommande().getProduits();
        for (size_t j = 0; j < produits.size(); j++)
        {
            int xIcone = rectBulle.x + 15 + (int)j * 32;
            int yIcone = rectBulle.y + 18;
            afficherSpriteProduit(renderer, produits[j].getType(), xIcone, yIcone);
        }
    }
}

/// rendu visuel des pieces pour les clients prets a payer
void Bulles::afficherBullesArgent(SDL_Renderer* renderer, const EnsembleClients& ensembleClients)
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

/// detection du clic sur une bulle de commande et renvoi du pointeur client
Client* Bulles::getClientCliqueCommande(int x, int y, EnsembleClients& ensembleClients)
{
    for (int i = 0; i < ensembleClients.getNombreClients(); i++)
    {
        Client& client = ensembleClients.getClient(i);
        if (!client.getAssis()) continue;
        if (!client.aUneCommande()) continue;
        if (client.getCommandeCliquee()) continue;
        if (client.getEtat() != ASSIS_ATTEND_COMMANDE) continue;
        
        SDL_Rect rectBulle = getRectBulleCommande(client);
        if (pointDansRect(x, y, rectBulle)) return &client;
    }
    return nullptr;
}

/// detection du clic sur une bulle de paiement et renvoi du pointeur client
Client* Bulles::getClientCliqueArgent(int x, int y, EnsembleClients& ensembleClients)
{
    for (int i = 0; i < ensembleClients.getNombreClients(); i++)
    {
        Client& client = ensembleClients.getClient(i);
        if (client.getEtat() != ATTEND_ARGENT) continue;
        if (client.getArgentClique()) continue;
        
        SDL_Rect rect = getRectBulleArgent(client);
        if (pointDansRect(x, y, rect)) return &client;
    }
    return nullptr;
}