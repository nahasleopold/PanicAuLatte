#ifndef ENSEMBLEPERSONNEL_H
#define ENSEMBLEPERSONNEL_H

#include <vector>
#include <iostream>
#include "Serveur.h"

/// gestionnaire de la collection d'employes travaillant dans le cafe
class EnsemblePersonnel {
private:
    /// liste regroupant tous les serveurs actifs
    std::vector<Serveur> serveurs;

public:
    /// initialisation de la liste du personnel
    EnsemblePersonnel();

    /// acces aux serveurs en lecture seule ou modification
    const std::vector<Serveur>& getConstServeurs() const;
    std::vector<Serveur>& getServeurs();

    /// methodes de recrutement avec parametrage du visuel et de la rapidite
    void ajouterServeur(int skin = 0);
    void ajouterServeur(int skin, float vitesse);
    
    /// mise a jour de la logique de deplacement et des etats pour tout le staff
    void update();
    
    /// affichage de l'etat des troupes dans la console
    void afficher() const;

    /// recuperation de la taille de l'equipe
    int getNombreServeurs() const;
};

#endif