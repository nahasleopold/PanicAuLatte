#include "EnsembleMachines.h"

/// init avec compteur d'id unique pour le suivi du parc
EnsembleMachines::EnsembleMachines()
    : prochainId(1)
{
}

/// accesseurs vers la liste complete des equipements
const std::vector<Machine>& EnsembleMachines::getConstMachines() const
{
    return machines;
}

std::vector<Machine>& EnsembleMachines::getMachines()
{
    return machines;
}

/// creation et enregistrement d'une nouvelle machine avec positionnement
void EnsembleMachines::ajouterMachine(TypeMachine type, const Coordonnee& position, int dureePreparation)
{
    machines.push_back(Machine(prochainId, type, position, dureePreparation));
    prochainId++;
}

/// denombrement total des machines installees
int EnsembleMachines::getNombreMachines() const
{
    return static_cast<int>(machines.size());
}

/// verification de l'existence d'un modele specifique dans la cuisine
bool EnsembleMachines::possedeMachine(TypeMachine type) const
{
    for (size_t i = 0; i < machines.size(); i++)
    {
        if (machines[i].getType() == type)
            return true;
    }
    return false;
}

/// check de faisabilite pour un produit donne selon l'equipement actuel
bool EnsembleMachines::produitDisponible(TypeProduit produit) const
{
    return trouverMachinePourProduit(produit) != nullptr;
}

/// recherche de l'unite de production compatible avec un article (version mutable)
Machine* EnsembleMachines::trouverMachinePourProduit(TypeProduit produit)
{
    for (size_t i = 0; i < machines.size(); i++)
    {
        if (machines[i].peutPreparer(produit))
            return &machines[i];
    }
    return nullptr;
}

/// recherche de l'unite de production compatible avec un article (version constante)
const Machine* EnsembleMachines::trouverMachinePourProduit(TypeProduit produit) const
{
    for (size_t i = 0; i < machines.size(); i++)
    {
        if (machines[i].peutPreparer(produit))
            return &machines[i];
    }
    return nullptr;
}

/// ciblage d'une machine par son type de modele (version mutable)
Machine* EnsembleMachines::trouverMachineParType(TypeMachine type)
{
    for (size_t i = 0; i < machines.size(); i++)
    {
        if (machines[i].getType() == type)
            return &machines[i];
    }
    return nullptr;
}

/// ciblage d'une machine par son type de modele (version constante)
const Machine* EnsembleMachines::trouverMachineParType(TypeMachine type) const
{
    for (size_t i = 0; i < machines.size(); i++)
    {
        if (machines[i].getType() == type)
            return &machines[i];
    }
    return nullptr;
}