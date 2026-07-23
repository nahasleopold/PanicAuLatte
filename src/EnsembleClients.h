#ifndef ENSEMBLECLIENTS_H
#define ENSEMBLECLIENTS_H

#include <vector>
#include <iostream>
#include "Client.h"

/// gestionnaire de la collection d'objets clients presents dans le cafe
class EnsembleClients {
private:
    /// stockage dynamique de la population du cafe
    std::vector<Client> clients;
    /// compteur pour l'attribution d'identifiants uniques
    unsigned int prochainId;

public:
    /// initialisation de la liste et du compteur d'id
    EnsembleClients();

    /// acces aux vecteurs de clients en lecture seule ou modification
    const std::vector<Client>& getConstClients() const;
    std::vector<Client>& getClients();

    /// ajout d'une nouvelle entite avec incrementation de l'id
    void ajouterClient(const Client& client);
    
    /// calcul des coordonnees cibles pour chaque client dans la queue
    void mettreAJourFileAttente();
    
    /// mise a jour logique de l'ensemble des membres de la liste
    void update();
    
    /// sortie console pour le suivi du nombre de clients
    void afficher() const;
    
    /// nettoyage de la memoire en retirant les entites ayant quitte la scene
    void supprimerClientsPartis();

    /// getters pour la taille de la liste et le filtrage par etat
    int getNombreClients() const;
    int getNombreClientsEnFile() const;
    
    /// recuperation d'une reference sur un client precis
    Client& getClient(int index);
};

#endif