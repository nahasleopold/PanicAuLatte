#include "ControleurMenu.h"
#include "jeu.h"
#include "sdljeu.h"
#include "OutilsClic.h"

/// analyse des entrees utilisateur pour naviguer dans le menu
int ControleurMenu::gererEvenement(const EvenementLib& event, bool cafeFerme, bool& quit, const AffichageMenu& affichageMenu) const
{
    /// fermeture de la fenetre
    if (event.type == LIB_QUITTER)
    {
        quit = true;
        return 3;
    }

    /// gestion des interactions a la souris
    if (event.type == LIB_CLIC_GAUCHE)
    {
        int x = event.x;
        int y = event.y;
        
        /// clic sur lancer/reprendre la partie
        if (OutilsClic::pointDansRect(x, y, affichageMenu.getBoutonJouer())) return 1;
        /// clic sur boutique uniquement si le service est termine
        if (OutilsClic::pointDansRect(x, y, affichageMenu.getBoutonBoutique()) && cafeFerme) return 2;
        /// clic sur sortie definitive
        if (OutilsClic::pointDansRect(x, y, affichageMenu.getBoutonQuitter()))
        {
            quit = true;
            return 3;
        }
    }

    /// gestion des raccourcis clavier
    if (event.type == LIB_TOUCHE_ENFONCEE)
    {
        switch (event.touche)
        {
            case LIB_TOUCHE_RETURN: return 1;
            case LIB_TOUCHE_B: if (cafeFerme) return 2; break;
            case LIB_TOUCHE_ESCAPE:
            case LIB_TOUCHE_Q: quit = true; return 3;
            default: break;
        }
    }

    return 0;
}

/// boucle de mise a jour du menu principal
void ControleurMenu::update(SDLJeu& sdlJeu, Jeu& jeu, bool& quit, AffichageMenu& affichageMenu)
{
    EvenementLib event;
    while (AdaptLib::pollEvenement(event))
    {
        int choix = gererEvenement(event, jeu.cafeEstFerme(), quit, affichageMenu);
        
        /// transition vers l'ecran de jeu
        if (choix == 1)
        {
            jeu.setEtatInterface(Jeu::JEU);
            return;
        }
        /// transition vers la boutique d'amelioration
        else if (choix == 2)
        {
            jeu.setEtatInterface(Jeu::BOUTIQUE);
            return;
        }
        /// arret du programme
        else if (choix == 3)
        {
            quit = true;
            return;
        }
    }

    /// rafraichissement visuel de l'interface du menu
    affichageMenu.afficher(sdlJeu.getRenderer(), sdlJeu.getFont(), sdlJeu.getFondMenu(), jeu.cafeEstFerme());
    SDL_RenderPresent(sdlJeu.getRenderer());
}