#include "Chaise.h"

/// constructeur par defaut avec valeurs initiales
Chaise::Chaise()
{
    position = Coordonnee(0, 0);
    occupee = false;
    type = CHAISE_GAUCHE;
}

/// init avec parametres specifiques de position et d'orientation
Chaise::Chaise(const Coordonnee& pos, TypeChaise t)
{
    position = pos;
    occupee = false;
    type = t;
}

/// recup de l'emplacement exact
Coordonnee Chaise::getPosition() const
{
    return position;
}

/// maj des coordonnees
void Chaise::setPosition(const Coordonnee& pos)
{
    position = pos;
}

/// check du statut de la place
bool Chaise::estOccupee() const
{
    return occupee;
}

/// verrouillage de la chaise par un client
void Chaise::occuper()
{
    occupee = true;
}

/// remise a dispo de la chaise
void Chaise::liberer()
{
    occupee = false;
}

/// recup du type (droite ou gauche)
TypeChaise Chaise::getType() const
{
    return type;
}

/// modif de l'orientation
void Chaise::setType(TypeChaise t)
{
    type = t;
}

/// calcul du decalage pour l'affichage propre du sprite
Coordonnee Chaise::getPositionAffichage() const
{
    if (type == CHAISE_GAUCHE)
    {
        return Coordonnee(position.getX() - 30, position.getY() - 45);
    }
    else
    {
        return Coordonnee(position.getX() -50, position.getY() - 45);
    }
}

/// largeur fixe pour le rendu
int Chaise::getLargeurAffichage() const
{
    return 130;
}

/// hauteur fixe pour le rendu
int Chaise::getHauteurAffichage() const
{
    return 175;
}

/// definition des zones accessibles pour le pathfinding
std::vector<Coordonnee> Chaise::getPositionsAcces() const
{
    std::vector<Coordonnee> acces;

    /// au-dessus
    acces.push_back(Coordonnee(position.getX(), position.getY() - 50));

    /// en-dessous
    acces.push_back(Coordonnee(position.getX(), position.getY() + 50));

    return acces;
}

/// calcul de la zone de superposition pour asseoir le sprite client
Coordonnee Chaise::getPositionAssise() const
{
    if (type == CHAISE_GAUCHE)
        return Coordonnee(position.getX() + 40, position.getY() +30);
    else
        return Coordonnee(position.getX() + 30, position.getY() +30);
}