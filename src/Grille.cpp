#include "Grille.h"
#include <queue>
#include <utility>
#include <algorithm>

/// configuration des dimensions et de l'ancrage de la grille de jeu
Grille::Grille()
{
    tailleCase = 50;
    origineX = 40;
    origineY = 270;
    nbColonnes = 33;
    nbLignes = 14;
    initialiser();
}

/// allocation de la memoire et marquage des zones speciales
void Grille::initialiser()
{
    cases = std::vector<std::vector<int>>(nbLignes, std::vector<int>(nbColonnes, 0));
    definirCuisine(20, 0, 7, 4);
    definirFileAttente(1, 0, 3, 8);
}

/// accesseurs pour les proprietes de structure
int Grille::getNbColonnes() const { return nbColonnes; }
int Grille::getNbLignes() const { return nbLignes; }
int Grille::getTailleCase() const { return tailleCase; }
int Grille::getOrigineX() const { return origineX; }
int Grille::getOrigineY() const { return origineY; }

/// verification des limites de la grille
bool Grille::caseValide(int col, int lig) const
{
    return (col >= 0 && col < nbColonnes && lig >= 0 && lig < nbLignes);
}

/// test de vacuite d'une cellule
bool Grille::caseLibre(int col, int lig) const
{
    if (!caseValide(col, lig)) return false;
    return cases[lig][col] == SALLE_LIBRE;
}

/// regles de collision specifiques aux clients (interdiction cuisine)
bool Grille::caseTraversableClient(int col, int lig) const
{
    if (!caseValide(col, lig)) return false;
    int valeur = cases[lig][col];
    return valeur == SALLE_LIBRE || valeur == FILE_ATTENTE;
}

/// regles de collision pour le personnel (acces total)
bool Grille::caseTraversableServeur(int col, int lig) const
{
    if (!caseValide(col, lig)) return false;
    int valeur = cases[lig][col];
    return valeur == SALLE_LIBRE || valeur == FILE_ATTENTE || valeur == CUISINE;
}

/// identification de la zone de preparation
bool Grille::caseCuisine(int col, int lig) const
{
    if (!caseValide(col, lig)) return false;
    return cases[lig][col] == CUISINE;
}

/// identification de la zone d'entree
bool Grille::caseFileAttente(int col, int lig) const
{
    if (!caseValide(col, lig)) return false;
    return cases[lig][col] == FILE_ATTENTE;
}

/// marquage d'une case comme obstacle dynamique (ex: meuble)
void Grille::occuperCase(int col, int lig)
{
    if (caseValide(col, lig) && cases[lig][col] != CUISINE) cases[lig][col] = OCCUPEE;
}

/// liberation d'une case pour le passage
void Grille::libererCase(int col, int lig)
{
    if (caseValide(col, lig) && cases[lig][col] != CUISINE) cases[lig][col] = SALLE_LIBRE;
}

/// delimitation rectangulaire de la zone reservee au personnel
void Grille::definirCuisine(int colDebut, int ligDebut, int largeur, int hauteur)
{
    for (int lig = ligDebut; lig < ligDebut + hauteur; lig++)
        for (int col = colDebut; col < colDebut + largeur; col++)
            if (caseValide(col, lig)) cases[lig][col] = CUISINE;
}

/// delimitation rectangulaire de la zone de queue
void Grille::definirFileAttente(int colDebut, int ligDebut, int largeur, int hauteur)
{
    for (int lig = ligDebut; lig < ligDebut + hauteur; lig++)
        for (int col = colDebut; col < colDebut + largeur; col++)
            if (caseValide(col, lig)) cases[lig][col] = FILE_ATTENTE;
}

/// conversion des coordonnees de l'ecran vers les index du tableau
void Grille::pixelVersCase(int x, int y, int& col, int& lig) const
{
    col = (x - origineX) / tailleCase;
    lig = (y - origineY) / tailleCase;
}

/// conversion des index du tableau vers les coordonnees du monde
Coordonnee Grille::caseVersPixel(int col, int lig) const
{
    return Coordonnee(origineX + col * tailleCase, origineY + lig * tailleCase);
}

/// implementation du pathfinding pour le deplacement sur grille
std::vector<Coordonnee> Grille::calculerCheminPixels(const Coordonnee& depart, const Coordonnee& arrivee) const
{
    int colDepart, ligDepart;
    int colArrivee, ligArrivee;

    pixelVersCase((int)depart.getX(), (int)depart.getY(), colDepart, ligDepart);
    pixelVersCase((int)arrivee.getX(), (int)arrivee.getY(), colArrivee, ligArrivee);

    std::vector<Coordonnee> cheminPixels;
    if (!caseValide(colDepart, ligDepart) || !caseValide(colArrivee, ligArrivee)) return cheminPixels;

    int nbL = getNbLignes();
    int nbC = getNbColonnes();

    std::vector<std::vector<bool>> visite(nbL, std::vector<bool>(nbC, false));
    std::vector<std::vector<std::pair<int,int>>> parent(nbL, std::vector<std::pair<int,int>>(nbC, {-1, -1}));

    std::queue<std::pair<int,int>> file;
    file.push({ligDepart, colDepart});
    visite[ligDepart][colDepart] = true;

    int dLig[4] = {-1, 1, 0, 0};
    int dCol[4] = {0, 0, -1, 1};
    bool trouve = false;

    while (!file.empty())
    {
        std::pair<int,int> courant = file.front();
        file.pop();

        int lig = courant.first;
        int col = courant.second;

        if (lig == ligArrivee && col == colArrivee)
        {
            trouve = true;
            break;
        }

        for (int k = 0; k < 4; k++)
        {
            int nLig = lig + dLig[k];
            int nCol = col + dCol[k];
            if (!caseValide(nCol, nLig)) continue;
            if (visite[nLig][nCol]) continue;
            if (!caseLibre(nCol, nLig) && !(nLig == ligArrivee && nCol == colArrivee)) continue;
            visite[nLig][nCol] = true;
            parent[nLig][nCol] = {lig, col};
            file.push({nLig, nCol});
        }
    }

    if (!trouve) return cheminPixels;

    /// reconstruction de la trajectoire a partir des donnees parents
    std::vector<std::pair<int,int>> cheminCases;
    int lig = ligArrivee;
    int col = colArrivee;
    while (!(lig == ligDepart && col == colDepart))
    {
        cheminCases.push_back({lig, col});
        std::pair<int,int> p = parent[lig][col];
        lig = p.first;
        col = p.second;
    }

    /// inversion du chemin et traduction finale en pixels
    std::reverse(cheminCases.begin(), cheminCases.end());
    for (size_t i = 0; i < cheminCases.size(); i++)
    {
        int ligCase = cheminCases[i].first;
        int colCase = cheminCases[i].second;
        cheminPixels.push_back(caseVersPixel(colCase, ligCase));
    }
    return cheminPixels;
}