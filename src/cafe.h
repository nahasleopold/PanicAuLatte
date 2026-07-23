#ifndef CAFE_H
#define CAFE_H

#include <iostream>
#include "EnsembleClients.h"
#include "EnsemblePersonnel.h"
///#include "Cuisine.h"
#include "EnsembleMeubles.h"
#include "EnsembleMachines.h"

/// classe principale qui centralise toutes les donnees du jeu
class Cafe {
private:
    /// stats globales du joueur
    float argent;
    int reputation;
    int clientsServis;

    /// listes de toutes les entites presentes sur la map
    EnsembleClients ensembleClients;
    EnsemblePersonnel ensemblePersonnel;
    ///Cuisine cuisine;
    EnsembleMeubles ensembleMeubles;
    EnsembleMachines ensembleMachines;

public:
    /// setup initial des stats et du personnel
    Cafe();

    /// getters pour la lecture des stats
    float getArgent() const;
    int getReputation() const;

    /// acces aux listes en lecture seule pour l'affichage
    const EnsembleClients& getConstEnsembleClients() const;
    const EnsemblePersonnel& getConstEnsemblePersonnel() const;
    const EnsembleMeubles& getConstEnsembleMeubles() const;
    const EnsembleMachines& getConstEnsembleMachines() const;
    
    /// acces aux listes en ecriture pour la logique
    EnsembleClients& getEnsembleClients();
    EnsemblePersonnel& getEnsemblePersonnel();
    EnsembleMeubles& getEnsembleMeubles();
    EnsembleMachines& getEnsembleMachines();
    
    /// gestion de la population et du mobilier
    void ajouterClient(const Client& client);
    void recruterServeur(int skin = 0);
    void recruterServeur(int skin, float vitesse);
    void mettreAJourReputation(int valeur);
    void enregistrerClientServi();
    void ajouterTables();
    void ajouterTablePosition(const Coordonnee& pos, int col, int lig);
    void faireSortirClients();

    /// gestion de l'equipement de cuisine
    bool produitDisponible(TypeProduit produit) const;
    void ajouterMachine(TypeMachine type, const Coordonnee& position, int dureePreparation = 3);

    /// systeme de tresorerie pour les achats
    bool peutDepenserArgent(float montant) const;
    bool depenserArgent(float montant);
    void ajouterArgent(float montant);
    bool acheterElement(float prix);

    /// moteur logique et logs console
    void update();
    void afficher() const;
};

#endif