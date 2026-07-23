#ifndef CHAISE_H
#define CHAISE_H

#include "coordonnee.h"
#include <vector>

/// enumeration pour l'orientation visuelle
enum TypeChaise
{
    CHAISE_GAUCHE,
    CHAISE_DROITE
};

/// entite representant une place assise pour un client
class Chaise
{
private:
    /// emplacement logique sur la map
    Coordonnee position;   /// position logique de la chaise
    /// statut de la place (libre ou prise)
    bool occupee;
    /// sens du sprite associe
    TypeChaise type;

public:
    /// constructeurs de base et parametre
    Chaise();
    Chaise(const Coordonnee& pos, TypeChaise t);

    /// acces a la position sur la grille
    Coordonnee getPosition() const;
    void setPosition(const Coordonnee& pos);

    /// gestion de l'occupation par un client
    bool estOccupee() const;
    void occuper();
    void liberer();

    /// acces a l'orientation visuelle
    TypeChaise getType() const;
    void setType(TypeChaise t);

    /// calculs de decalage et tailles pour le rendu graphique
    Coordonnee getPositionAffichage() const;
    int getLargeurAffichage() const;
    int getHauteurAffichage() const;

    /// zones d'interaction pour le pathfinding et l'animation
    std::vector<Coordonnee> getPositionsAcces() const;
    Coordonnee getPositionAssise() const;
};

#endif