#ifndef GRILLE_H
#define GRILLE_H

#include <vector>
#include "coordonnee.h"

/// structure de donnees gerant le decoupage spatial du cafe
class Grille {
private:
    /// dimensions du damier logique
    int nbColonnes;
    int nbLignes;
    /// dimension d'une cellule en pixels
    int tailleCase;

    /// coordonnees du coin superieur gauche sur l'ecran
    int origineX;
    int origineY;

    /// matrice representant l'etat et le type de chaque case
    std::vector<std::vector<int>> cases;

public:
    /// constantes definissant les types de zones et etats de collision
    static const int SALLE_LIBRE = 0;
    static const int OCCUPEE = 1;
    static const int CUISINE = 2;
    static const int FILE_ATTENTE = 3;

    /// initialisation des parametres de la grille
    Grille();
    /// configuration par defaut du plan du cafe
    void initialiser();

    /// accesseurs pour les dimensions et l'ancrage
    int getNbColonnes() const;
    int getNbLignes() const;
    int getTailleCase() const;
    int getOrigineX() const;
    int getOrigineY() const;

    /// tests de validite et de proprietes des cases
    bool caseValide(int col, int lig) const;
    bool caseLibre(int col, int lig) const;
    bool caseTraversableClient(int col, int lig) const;
    bool caseTraversableServeur(int col, int lig) const;
    bool caseCuisine(int col, int lig) const;
    bool caseFileAttente(int col, int lig) const;

    /// modification dynamique de l'etat d'une cellule (meubles, obstacles)
    void occuperCase(int col, int lig);
    void libererCase(int col, int lig);

    /// outils de dessin des zones reservees
    void definirCuisine(int colDebut, int ligDebut, int largeur, int hauteur);
    void definirFileAttente(int colDebut, int ligDebut, int largeur, int hauteur);

    /// fonctions de conversion entre espace ecran et espace logique
    void pixelVersCase(int x, int y, int& col, int& lig) const;
    Coordonnee caseVersPixel(int col, int lig) const;

    /// recherche de chemin simplifiee pour les entites mobiles
    std::vector<Coordonnee> calculerCheminPixels(const Coordonnee& depart, const Coordonnee& arrivee) const;
};

#endif