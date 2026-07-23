#ifndef DEPLACEMENTPERSONNAGE_H
#define DEPLACEMENTPERSONNAGE_H

#include <vector>
#include "coordonnee.h"

/// fonctions statiques pour la gestion des trajectoires
class DeplacementPersonnage
{
public:
    /// verification de la superposition entre position et cible
    static bool estArriveCible(const Coordonnee& position, const Coordonnee& cible)
    {
        return ((int)position.getX() == (int)cible.getX() && (int)position.getY() == (int)cible.getY());
    }

    /// calcul du mouvement rectiligne avec lissage sur la destination
    static void deplacerVersCible(Coordonnee& position, const Coordonnee& cible, float vitesse)
    {
        float x = position.getX();
        float y = position.getY();
        float cx = cible.getX();
        float cy = cible.getY();

        if (x < cx) { x += vitesse; if (x > cx) x = cx; }
        else if (x > cx) { x -= vitesse; if (x < cx) x = cx; }

        if (y < cy) { y += vitesse; if (y > cy) y = cy; }
        else if (y > cy) { y -= vitesse; if (y < cy) y = cy; }

        position.setX(x);
        position.setY(y);
    }

    /// initialisation d'un nouveau parcours et de la premiere etape
    static void setChemin(std::vector<Coordonnee>& cheminCourant, Coordonnee& cible, const std::vector<Coordonnee>& chemin)
    {
        cheminCourant = chemin;
        if (!cheminCourant.empty()) cible = cheminCourant[0];
    }

    /// test de presence d'etapes restantes
    static bool aUnChemin(const std::vector<Coordonnee>& cheminCourant)
    {
        return !cheminCourant.empty();
    }

    /// reinitialisation de la liste des points de passage
    static void viderChemin(std::vector<Coordonnee>& cheminCourant)
    {
        cheminCourant.clear();
    }

    /// progression automatique le long du vecteur de chemin
    static void avancerSurChemin(std::vector<Coordonnee>& cheminCourant, Coordonnee& position, Coordonnee& cible, float vitesse)
    {
        if (cheminCourant.empty()) return;
        cible = cheminCourant[0];
        deplacerVersCible(position, cible, vitesse);

        /// passage a l'etape suivante apres validation de la cible actuelle
        if (estArriveCible(position, cible))
        {
            cheminCourant.erase(cheminCourant.begin());
            if (!cheminCourant.empty()) cible = cheminCourant[0];
        }
    }
};

#endif