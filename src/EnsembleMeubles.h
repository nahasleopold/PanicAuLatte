#ifndef ENSEMBLEMEUBLES_H
#define ENSEMBLEMEUBLES_H

#include <vector>
#include <iostream>
#include "Table.h"

/// gestionnaire de la collection de mobilier du cafe
class EnsembleMeubles {
private:
    /// stockage de toutes les tables installees
    std::vector<Table> tables;

public:
    /// initialisation de la collection
    EnsembleMeubles();

    /// acces aux tables en lecture seule ou modification
    const std::vector<Table>& getConstTables() const;
    std::vector<Table>& getTables();

    /// methodes d'ajout de mobilier avec ou sans position precise
    void ajouterTables();
    void ajouterTablePosition(const Coordonnee& pos, int col, int lig);

    /// outil de recherche global pour l'attribution d'un siege a un client
    Chaise* trouverChaiseLibre();

    /// outils de diagnostic et de denombrement
    void afficher() const;
    int getNombreTables() const;
};

#endif