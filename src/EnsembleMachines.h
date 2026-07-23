#ifndef ENSEMBLEMACHINES_H
#define ENSEMBLEMACHINES_H

#include <vector>
#include "Machine.h"

/// gestionnaire du parc d'equipements de production du cafe
class EnsembleMachines
{
private:
    /// liste des machines installees en cuisine
    std::vector<Machine> machines;
    /// compteur pour l'attribution d'identifiants uniques aux machines
    int prochainId;

public:
    /// initialisation de la collection de machines
    EnsembleMachines();

    /// accesseurs vers le vecteur d'equipements (lecture seule ou modification)
    const std::vector<Machine>& getConstMachines() const;
    std::vector<Machine>& getMachines();

    /// ajout d'une unite de production avec ses parametres de base
    void ajouterMachine(TypeMachine type, const Coordonnee& position, int dureePreparation = 3);

    /// outils de verification de l'inventaire et des capacites de service
    int getNombreMachines() const;
    bool possedeMachine(TypeMachine type) const;
    bool produitDisponible(TypeProduit produit) const;

    /// recherche de l'equipement requis selon le produit souhaite
    Machine* trouverMachinePourProduit(TypeProduit produit);
    const Machine* trouverMachinePourProduit(TypeProduit produit) const;

    /// ciblage d'un equipement par son type de modele
    Machine* trouverMachineParType(TypeMachine type);
    const Machine* trouverMachineParType(TypeMachine type) const;
};

#endif