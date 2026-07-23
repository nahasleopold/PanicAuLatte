#ifndef CONTROLEURBOUTIQUE_H
#define CONTROLEURBOUTIQUE_H

#include "AdaptLib.h"
#include "Machine.h"
#include "AffichageBoutique.h"

class SDLJeu;
class Jeu;

/// gestion des interactions et de la logique d'achat de la boutique
class ControleurBoutique
{
private:
    /// grille tarifaire pour l'embauche du personnel
    float prixServeurs[5];
    /// caracteristiques de vitesse selon le niveau du serveur
    float vitessesServeurs[5];
    /// couts d'achat pour les nouveaux equipements
    float prixMachines[3];

public:
    /// initialisation des prix et des stats des elements en vente
    ControleurBoutique();
    
    /// traduction des clics utilisateur en actions d'interface
    int gererEvenement(const EvenementLib& event, bool& quit, const AffichageBoutique& affichageBoutique) const;
    
    /// boucle principale traitant les achats et l'affichage
    void update(SDLJeu& sdlJeu, Jeu& jeu, bool& quit, AffichageBoutique& affichageBoutique);
};

#endif