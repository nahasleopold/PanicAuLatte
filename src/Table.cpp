#include "Table.h"

/// creation d'une table vide par defaut
Table::Table()
{
    position = Coordonnee(0, 0);
    col = 0;
    lig = 0;
    nbChaises = 2;

    /// on met une chaise a gauche et une a droite
    chaises[0] = Chaise(Coordonnee(0, 0), CHAISE_GAUCHE);
    chaises[1] = Chaise(Coordonnee(0, 0), CHAISE_DROITE);
}

/// creation d'une table a un endroit precis avec ses deux chaises
Table::Table(const Coordonnee &pos, int colonne, int ligne)
{
    position = pos;
    col = colonne;
    lig = ligne;
    nbChaises = 2;

    /// on calcule la place des chaises par rapport a la table
    chaises[0] = Chaise(Coordonnee(pos.getX() - 50, pos.getY()), CHAISE_GAUCHE);
    chaises[1] = Chaise(Coordonnee(pos.getX() + 100, pos.getY()), CHAISE_DROITE);
}

/// donner la position de la table
Coordonnee Table::getPosition() const
{
    return position;
}

/// donner la colonne dans la grille
int Table::getCol() const
{
    return col;
}

/// donner la ligne dans la grille
int Table::getLig() const
{
    return lig;
}

/// recuperer la liste des chaises (lecture seule)
const Chaise* Table::getChaises() const
{
    return chaises;
}

/// recuperer la liste des chaises pour pouvoir les changer
Chaise* Table::getChaisesModifiables()
{
    return chaises;
}

/// donner le nombre de chaises autour de la table
int Table::getNbChaises() const
{
    return nbChaises;
}

/// chercher une chaise ou personne n'est assis
Chaise* Table::trouverChaiseLibre()
{
    for (int i = 0; i < nbChaises; i++)
    {
        if (!chaises[i].estOccupee())
            return &chaises[i];
    }
    return nullptr;
}

/// savoir si toutes les chaises de la table sont prises
bool Table::estPleine() const
{
    for (int i = 0; i < nbChaises; i++)
    {
        if (!chaises[i].estOccupee())
            return false;
    }
    return true;
}

/// calcul de la position exacte pour dessiner l'image a l'ecran
Coordonnee Table::getPositionAffichage() const
{
    return Coordonnee(position.getX() - 18, position.getY() - 24);
}

/// donner la largeur de l'image de la table
int Table::getLargeurAffichage() const
{
    return 135;
}

/// donner la hauteur de l'image de la table
int Table::getHauteurAffichage() const
{
    return 135;
}