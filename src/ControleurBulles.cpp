#include "ControleurBulles.h"
#include "OutilsClic.h"

/// recherche du client correspondant au clic sur une bulle de commande
Client* ControleurBulles::getClientCliqueCommande(int x, int y, EnsembleClients& ensembleClients, const AffichageBulles& affichageBulles) const
{
    for (int i = 0; i < ensembleClients.getNombreClients(); i++)
    {
        Client& client = ensembleClients.getClient(i);
        if (!client.getAssis()) continue;
        if (!client.aUneCommande()) continue;
        if (client.getCommandeCliquee()) continue;
        if (client.getEtat() != ASSIS_ATTEND_COMMANDE) continue;
        SDL_Rect rectBulle = affichageBulles.getRectCommandeClient(client);
        if (OutilsClic::pointDansRect(x, y, rectBulle)) return &client;
    }
    return nullptr;
}

/// recherche du client correspondant au clic sur une bulle de paiement
Client* ControleurBulles::getClientCliqueArgent(int x, int y, EnsembleClients& ensembleClients, const AffichageBulles& affichageBulles) const
{
    for (int i = 0; i < ensembleClients.getNombreClients(); i++)
    {
        Client& client = ensembleClients.getClient(i);
        if (client.getEtat() != ATTEND_ARGENT) continue;
        if (client.getArgentClique()) continue;
        SDL_Rect rectBulle = affichageBulles.getRectArgentClient(client);
        if (OutilsClic::pointDansRect(x, y, rectBulle)) return &client;
    }
    return nullptr;
}