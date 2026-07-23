#ifndef MACHINE_H
#define MACHINE_H

#include "coordonnee.h"
#include "produit.h"
#include <string>

/// categories d'equipements disponibles pour la production
enum TypeMachine
{
    MACHINE_CAFE,
    MACHINE_MATCHA,
    MACHINE_JUS,
    MACHINE_DESSERT
};

/// representation d'un automate de preparation de commandes
class Machine
{
private:
    /// identifiant unique au sein du parc
    int idMachine;
    /// modele de l'appareil determinant ses capacites
    TypeMachine type;
    /// emplacement sur le plan de la cuisine
    Coordonnee position;
    /// indicateur de disponibilite pour une nouvelle tache
    bool occupee;
    /// temps de cycle pour finaliser une boisson ou un plat
    int dureePreparation; /// en secondes

public:
    /// initialisations avec valeurs par defaut ou specifiques
    Machine();
    Machine(int id, TypeMachine t, const Coordonnee& pos, int dureePrep = 3);

    /// accesseurs pour la consultation des caracteristiques et de l'etat
    int getIdMachine() const;
    TypeMachine getType() const;
    const Coordonnee& getPosition() const;
    bool getOccupee() const;
    int getDureePreparation() const;

    /// mutateurs pour la configuration dynamique du materiel
    void setIdMachine(int id);
    void setType(TypeMachine t);
    void setPosition(const Coordonnee& pos);
    void setOccupee(bool valeur);
    void setDureePreparation(int duree);

    /// verification de la compatibilite entre la machine et une recette
    bool peutPreparer(TypeProduit produit) const;
    /// recuperation du label textuel pour l'affichage
    std::string getNom() const;
};

#endif