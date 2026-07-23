#ifndef SYSTEMESERVEURS_H
#define SYSTEMESERVEURS_H

#include "cafe.h"
#include "Grille.h"
#include "CalculChemin.h"
#include "AdaptLib.h"

/// le cerveau qui dirige tous les serveurs du cafe
class SystemeServeurs
{
private:
    /// les outils pour connaitre le cafe et calculer les trajets
    Cafe& cafe;
    Grille& grille;
    CalculChemin& calculChemin;

    /// fonctions pour trouver quelqu'un ou une machine
    Client* trouverClientParId(unsigned int idClient);
    Machine* trouverMachinePourProduit(TypeProduit produit);
    Serveur* trouverServeurLibreLePlusProche(const Coordonnee& positionClient);

    /// calculs pour savoir si on est arrive ou si c'est trop loin
    float distanceCarree(const Coordonnee& a, const Coordonnee& b) const;
    bool positionsProches(const Coordonnee& a, const Coordonnee& b) const;
    bool delaiTermine(Uint32 tickDebut, Uint32 dureeMs) const;

    /// fonctions pour lancer les deplacements et les etapes de la commande
    void lancerDeplacement(Serveur& serveur, const Coordonnee& cible, EtatServeur nouvelEtat);
    bool updateDeplacement(Serveur& serveur, const Coordonnee& cible);
    void lancerProchaineEtapeCommande(Serveur& serveur, Client& client);
    Coordonnee getPositionSortie() const;

    /// gestion de chaque action : aller voir le client, preparer, servir, encaisser
    void gererServeurVaPrendreCommande(Serveur& serveur, Client& client);
    void gererServeurPrendCommande(Serveur& serveur, Client& client);
    void gererServeurVaMachine(Serveur& serveur, Client& client);
    void gererServeurPrepareCommande(Serveur& serveur, Client& client);
    void gererServeurVaServir(Serveur& serveur, Client& client);
    void gererServeurSertClient(Serveur& serveur, Client& client);
    void gererServeurVaEncaisser(Serveur& serveur, Client& client);
    void gererServeurEncaisse(Serveur& serveur, Client& client);

public:
    /// on cree le systeme avec tout ce dont il a besoin
    SystemeServeurs(Cafe& cafeRef, Grille& grilleRef, CalculChemin& calculRef);

    /// donner un travail au serveur (commande ou argent)
    bool assignerMissionCommande(Client& client);
    bool assignerMissionEncaissement(Client& client);

    /// mise a jour de tout le monde a chaque image du jeu
    void update();
};

#endif