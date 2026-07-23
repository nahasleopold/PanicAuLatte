#ifndef CONTROLEURBULLES_H
#define CONTROLEURBULLES_H

#include "AffichageBulles.h"

/// gestion de l'interaction souris avec les indicateurs au-dessus des clients
class ControleurBulles
{
public:
    /// detection du clic sur l'icone de commande pour lancer la preparation
    Client* getClientCliqueCommande(int x, int y, EnsembleClients& ensembleClients, const AffichageBulles& affichageBulles) const;
    
    /// detection du clic sur l'icone de monnaie pour encaisser la vente
    Client* getClientCliqueArgent(int x, int y, EnsembleClients& ensembleClients, const AffichageBulles& affichageBulles) const;
};

#endif