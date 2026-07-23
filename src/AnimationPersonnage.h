#ifndef ANIMATIONPERSONNAGE_H
#define ANIMATIONPERSONNAGE_H

#include <vector>
#include "AdaptLib.h"
#include "coordonnee.h"

/// gestion logique des mouvements et des sprites des personnages
class AnimationPersonnage
{
private:
    /// infos pour le rendu visuel (sens, frame en cours, chrono)
    int direction;
    int frameAnim;
    Uint32 dernierTickAnim;
    bool vaAGauche;
    
    /// liste des etapes du parcours a effectuer
    std::vector<Coordonnee> cheminCourant;

public:
    /// init des parametres de base
    AnimationPersonnage();

    /// maj du sprite selon l'etat de mouvement
    void animer(bool enMouvement);

    /// check d'arrivee sur la position exacte
    bool estArriveCible(const Coordonnee& position, const Coordonnee& cible) const;
    
    /// calcul mathematique du pas vers le point vise
    void deplacerVersCible(Coordonnee& position, const Coordonnee& cible, float vitesse);
    
    /// assignation d'un nouvel itineraire
    void setChemin(Coordonnee& cible, const std::vector<Coordonnee>& chemin);
    
    /// verif si un trajet est actif
    bool aUnChemin() const;
    
    /// reset complet de l'itineraire
    void viderChemin();
    
    /// suivi et validation des etapes du parcours
    void avancerSurChemin(Coordonnee& position, Coordonnee& cible, float vitesse);

    /// getters pour la recup des infos visuelles
    int getDirection() const;
    int getFrameAnim() const;
    bool getVaAGauche() const;

    /// setters pour forcer l'orientation
    void setDirection(int d);
    void setVaAGauche(bool g);
};

#endif