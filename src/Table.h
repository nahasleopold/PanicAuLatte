#ifndef TABLE_H
#define TABLE_H

#include "Chaise.h"
#include "coordonnee.h"

class Table
{
private:
    Coordonnee position;   /// position logique en haut-gauche de la table
    int col;
    int lig;

    Chaise chaises[2];
    int nbChaises;

public:
    Table();
    Table(const Coordonnee &pos, int colonne, int ligne);

    Coordonnee getPosition() const;
    int getCol() const;
    int getLig() const;

    const Chaise* getChaises() const;
    Chaise* getChaisesModifiables();
    int getNbChaises() const;

    Chaise* trouverChaiseLibre();
    bool estPleine() const;

    /// position d'affichage du sprite table
    Coordonnee getPositionAffichage() const;

    int getLargeurAffichage() const;
    int getHauteurAffichage() const;
};

#endif