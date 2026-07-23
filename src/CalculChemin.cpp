#include "CalculChemin.h"
#include "Grille.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <utility>

namespace {
/// fonction interne de pathfinding (algo bfs) pour eviter la duplication de code
std::vector<Coordonnee> calculerCheminInterne(const Grille& grille,
                                             const Coordonnee& depart,
                                             const Coordonnee& arrivee,
                                             bool pourServeur)
{
    int colDepart, ligDepart;
    int colArrivee, ligArrivee;

    /// conversion des coordonnees pixels en index de grille
    grille.pixelVersCase((int)depart.getX(), (int)depart.getY(), colDepart, ligDepart);
    grille.pixelVersCase((int)arrivee.getX(), (int)arrivee.getY(), colArrivee, ligArrivee);

    std::vector<Coordonnee> cheminPixels;
    /// secu pour eviter de chercher en dehors de la map
    if (!grille.caseValide(colDepart, ligDepart) || !grille.caseValide(colArrivee, ligArrivee))
        return cheminPixels;

    int nbL = grille.getNbLignes();
    int nbC = grille.getNbColonnes();

    /// setup des structures pour l'algo (cases visitees et sauvegarde du chemin parent)
    std::vector<std::vector<bool>> visite(nbL, std::vector<bool>(nbC, false));
    std::vector<std::vector<std::pair<int, int>>> parent(
        nbL, std::vector<std::pair<int, int>>(nbC, std::make_pair(-1, -1))
    );

    /// init de la file pour l'exploration
    std::queue<std::pair<int, int>> file;
    file.push(std::make_pair(ligDepart, colDepart));
    visite[ligDepart][colDepart] = true;

    /// vecteurs de deplacement (haut, bas, gauche, droite)
    int dLig[4] = {-1, 1, 0, 0};
    int dCol[4] = {0, 0, -1, 1};
    bool trouve = false;

    /// boucle principale du parcours en largeur
    while (!file.empty())
    {
        std::pair<int, int> courant = file.front();
        file.pop();

        int lig = courant.first;
        int col = courant.second;

        /// check de victoire si on est sur la case d'arrivee
        if (lig == ligArrivee && col == colArrivee)
        {
            trouve = true;
            break;
        }

        /// test des 4 cases adjacentes
        for (int k = 0; k < 4; k++)
        {
            int nLig = lig + dLig[k];
            int nCol = col + dCol[k];
            
            /// filtres de validite et de non-repetition
            if (!grille.caseValide(nCol, nLig)) continue;
            if (visite[nLig][nCol]) continue;

            /// check des collisions selon le type d'entite (client ou serveur)
            bool traversable = pourServeur
                ? grille.caseTraversableServeur(nCol, nLig)
                : grille.caseTraversableClient(nCol, nLig);

            /// tolerance pour cibler un objet non traversable (comme une table ou une machine)
            if (!traversable && !(nLig == ligArrivee && nCol == colArrivee))
                continue;

            /// marquage de la case et ajout a la file
            visite[nLig][nCol] = true;
            parent[nLig][nCol] = std::make_pair(lig, col);
            file.push(std::make_pair(nLig, nCol));
        }
    }

    /// retour liste vide si aucun chemin possible
    if (!trouve) return cheminPixels;

    /// remontee de l'historique pour reconstruire le chemin a l'envers
    std::vector<std::pair<int, int>> cheminCases;
    int lig = ligArrivee;
    int col = colArrivee;
    while (!(lig == ligDepart && col == colDepart))
    {
        cheminCases.push_back(std::make_pair(lig, col));
        std::pair<int, int> p = parent[lig][col];
        lig = p.first;
        col = p.second;
    }

    /// remise a l'endroit et reconversion en pixels pour le deplacement fluide
    std::reverse(cheminCases.begin(), cheminCases.end());
    for (size_t i = 0; i < cheminCases.size(); i++)
    {
        int ligCase = cheminCases[i].first;
        int colCase = cheminCases[i].second;
        cheminPixels.push_back(grille.caseVersPixel(colCase, ligCase));
    }

    return cheminPixels;
}
}

/// recup de la reference vers la map du jeu
CalculChemin::CalculChemin(const Grille& g) : grille(g) {}

/// appel du bfs avec les regles de collision des clients
std::vector<Coordonnee> CalculChemin::calculerChemin(const Coordonnee& depart,
                                                     const Coordonnee& arrivee) const
{
    return calculerCheminInterne(grille, depart, arrivee, false);
}

/// appel du bfs avec les regles de collision des serveurs (acces cuisine)
std::vector<Coordonnee> CalculChemin::calculerCheminServeur(const Coordonnee& depart,
                                                            const Coordonnee& arrivee) const
{
    return calculerCheminInterne(grille, depart, arrivee, true);
}