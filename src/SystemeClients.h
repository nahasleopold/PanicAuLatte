#ifndef SYSTEMECLIENTS_H
#define SYSTEMECLIENTS_H

#include "cafe.h"
#include "Grille.h"
#include "CalculChemin.h"
#include "AdaptLib.h"
#include <SDL2/SDL.h>

/// moteur logique pilotant le flux et le comportement des clients
class SystemeClients
{
private:
    /// references vers les composants structurels du jeu
    Cafe& cafe;
    Grille& grille;
    CalculChemin& calculChemin;

    /// outils internes pour la navigation et la planification des flux
    Coordonnee getPositionPorte() const;
    Chaise* trouverChaiseLibreAccessible(const Coordonnee& depart, std::vector<Coordonnee>& cheminTrouve);
    Uint32 calculerDelaiArriveeClients() const;

public:
    /// initialisation avec injection des dependances necessaires
    SystemeClients(Cafe& cafeRef, Grille& grilleRef, CalculChemin& calculRef);

    /// gestionnaire de l'apparition des nouveaux clients selon l'etat du cafe
    void gererArriveeClients(Uint32& dernierTickSpawnClient, bool cafeFerme);
    
    /// processus d'attribution d'un siege pour les clients en attente
    void assignerClientsAuxChaises();
    
    /// procedure d'evacuation generale lors de la fermeture
    void faireSortirClients();
    
    /// purge de la memoire pour les entites ayant quitte la simulation
    void nettoyerClientsPartis();
    
    /// surveillance de la satisfaction client liee aux delais de service
    void gererReputationAttente();
};

#endif