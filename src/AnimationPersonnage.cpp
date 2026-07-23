#include "AnimationPersonnage.h"

/// init des variables d'animation de base
AnimationPersonnage::AnimationPersonnage()
{
    direction = 4;
    frameAnim = 0;
    dernierTickAnim = AdaptLib::getTicks();
    vaAGauche = false;
}

/// maj de la frame courante si le perso bouge
void AnimationPersonnage::animer(bool enMouvement)
{
    /// retour position fixe si arret
    if (!enMouvement)
    {
        frameAnim = 0;
        return;
    }

    /// boucle sur 3 frames d'animation toutes les 150ms
    Uint32 tickActuel = AdaptLib::getTicks();
    if (tickActuel - dernierTickAnim > 150)
    {
        frameAnim = (frameAnim + 1) % 3;
        dernierTickAnim = tickActuel;
    }
}

/// verif si la destination exacte est atteinte
bool AnimationPersonnage::estArriveCible(const Coordonnee& position, const Coordonnee& cible) const
{
    return ((int)position.getX() == (int)cible.getX() &&
            (int)position.getY() == (int)cible.getY());
}

/// calcul du deplacement pas a pas sans depasser la cible
void AnimationPersonnage::deplacerVersCible(Coordonnee& position, const Coordonnee& cible, float vitesse)
{
    float x = position.getX();
    float y = position.getY();
    float cx = cible.getX();
    float cy = cible.getY();

    if (x < cx)
    {
        x += vitesse;
        if (x > cx) x = cx;
    }
    else if (x > cx)
    {
        x -= vitesse;
        if (x < cx) x = cx;
    }

    if (y < cy)
    {
        y += vitesse;
        if (y > cy) y = cy;
    }
    else if (y > cy)
    {
        y -= vitesse;
        if (y < cy) y = cy;
    }

    position.setX(x);
    position.setY(y);
}

/// affectation d'un nouveau trajet et de la premiere etape
void AnimationPersonnage::setChemin(Coordonnee& cible, const std::vector<Coordonnee>& chemin)
{
    cheminCourant = chemin;
    if (!cheminCourant.empty())
        cible = cheminCourant[0];
}

/// check si un trajet est en cours
bool AnimationPersonnage::aUnChemin() const
{
    return !cheminCourant.empty();
}

/// annulation du deplacement
void AnimationPersonnage::viderChemin()
{
    cheminCourant.clear();
}

/// progression le long du parcours defini
void AnimationPersonnage::avancerSurChemin(Coordonnee& position, Coordonnee& cible, float vitesse)
{
    if (cheminCourant.empty())
        return;

    cible = cheminCourant[0];
    deplacerVersCible(position, cible, vitesse);

    /// passage au noeud suivant une fois l'etape validee
    if (estArriveCible(position, cible))
    {
        cheminCourant.erase(cheminCourant.begin());

        if (!cheminCourant.empty())
            cible = cheminCourant[0];
    }
}

/// getters et setters pour le rendu visuel
int AnimationPersonnage::getDirection() const
{
    return direction;
}

int AnimationPersonnage::getFrameAnim() const
{
    return frameAnim;
}

bool AnimationPersonnage::getVaAGauche() const
{
    return vaAGauche;
}

void AnimationPersonnage::setDirection(int d)
{
    direction = d;
}

void AnimationPersonnage::setVaAGauche(bool g)
{
    vaAGauche = g;
}