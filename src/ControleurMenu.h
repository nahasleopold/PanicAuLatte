#ifndef CONTROLEURMENU_H
#define CONTROLEURMENU_H

#include "AdaptLib.h"
#include "AffichageMenu.h"

class Jeu;
class SDLJeu;

/// pilotage des transitions d'etats depuis le menu principal
class ControleurMenu
{
public:
    /// analyse des clics et touches pour determiner l'action a mener
    int gererEvenement(const EvenementLib& event, bool cafeFerme, bool& quit, const AffichageMenu& affichageMenu) const;
    
    /// boucle de rafraichissement et aiguillage vers le jeu ou la boutique
    void update(SDLJeu& sdlJeu, Jeu& jeu, bool& quit, AffichageMenu& affichageMenu);
};

#endif