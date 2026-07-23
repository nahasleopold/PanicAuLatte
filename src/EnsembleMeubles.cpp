#include "EnsembleMeubles.h"

/// constructeur par defaut
EnsembleMeubles::EnsembleMeubles() {
}

/// accesseurs pour la liste des tables (version constante)
const std::vector<Table>& EnsembleMeubles::getConstTables() const {
    return tables;
}

/// accesseurs pour la liste des tables (version modifiable)
std::vector<Table>& EnsembleMeubles::getTables() {
    return tables;
}

/// ajout d'une table par defaut a une position fixe
void EnsembleMeubles::ajouterTables() {
    tables.push_back(Table(Coordonnee(200, 200), 0, 0));
}

/// ajout d'une table avec coordonnees et index de grille precis
void EnsembleMeubles::ajouterTablePosition(const Coordonnee& pos, int col, int lig) {
    tables.push_back(Table(pos, col, lig));
}

/// recherche d'un siege disponible parmi toutes les tables du cafe
Chaise* EnsembleMeubles::trouverChaiseLibre() {
    for (size_t i = 0; i < tables.size(); i++) {
        Chaise* chaise = tables[i].trouverChaiseLibre();
        if (chaise != nullptr) {
            return chaise;
        }
    }
    return nullptr;
}

/// affichage des infos de positionnement dans la console
void EnsembleMeubles::afficher() const {
    std::cout << "Nombre de tables : " << tables.size() << "\n";
    for (size_t i = 0; i < tables.size(); i++) {
        Coordonnee pos = tables[i].getPosition();
        std::cout << "Table " << i << " : (" << pos.getX() << ", " << pos.getY() << ")\n";
    }
}

/// denombrement des tables installees
int EnsembleMeubles::getNombreTables() const {
    return (int)tables.size();
}