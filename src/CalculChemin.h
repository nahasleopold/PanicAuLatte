#ifndef CALCULCHEMIN_H
#define CALCULCHEMIN_H

#include <vector>
#include "coordonnee.h"

class Grille;

/// utilitaire de recherche de trajectoire sur la carte
class CalculChemin
{
private:
    /// reference vers la map pour lire les obstacles
    const Grille& grille;

public:
    /// init du composant avec la grille active
    CalculChemin(const Grille& g);

    /// calcul d'itineraire standard pour les clients
    std::vector<Coordonnee> calculerChemin(const Coordonnee& depart, const Coordonnee& arrivee) const;
    
    /// calcul d'itineraire avec acces privilegie pour le staff
    std::vector<Coordonnee> calculerCheminServeur(const Coordonnee& depart, const Coordonnee& arrivee) const;
};

#endif