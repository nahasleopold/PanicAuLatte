#include "ControleurBoutique.h"
#include "jeu.h"
#include "sdljeu.h"
#include "OutilsClic.h"

/// init des tarifs et des stats des employes dispos a l'achat
ControleurBoutique::ControleurBoutique()
{
    prixServeurs[0] = 30.0f;
    prixServeurs[1] = 40.0f;
    prixServeurs[2] = 50.0f;
    prixServeurs[3] = 60.0f;
    prixServeurs[4] = 70.0f;
    vitessesServeurs[0] = 2.0f;
    vitessesServeurs[1] = 2.3f;
    vitessesServeurs[2] = 2.6f;
    vitessesServeurs[3] = 2.9f;
    vitessesServeurs[4] = 3.2f;
    prixMachines[0] = 100.0f;
    prixMachines[1] = 150.0f;
    prixMachines[2] = 200.0f;
}

/// traduction des inputs utilisateur en codes d'action simples
int ControleurBoutique::gererEvenement(const EvenementLib& event, bool& quit, const AffichageBoutique& affichageBoutique) const
{
    if (event.type == LIB_QUITTER) { quit = true; return 3; }
    if (event.type == LIB_TOUCHE_ENFONCEE && event.touche == LIB_TOUCHE_ESCAPE) return 1;

    /// test des collisions souris avec les differentes zones cliquables
    if (event.type == LIB_CLIC_GAUCHE)
    {
        int x = event.x;
        int y = event.y;
        if (OutilsClic::pointDansRect(x, y, affichageBoutique.getBoutonTable())) return 2;
        for (int i = 0; i < 5; i++) if (OutilsClic::pointDansRect(x, y, affichageBoutique.getBoutonServeur(i))) return 10 + i;
        for (int i = 0; i < 3; i++) if (OutilsClic::pointDansRect(x, y, affichageBoutique.getBoutonMachine(i))) return 20 + i;
    }
    return 0;
}

/// boucle de gestion des achats avec verification des fonds
void ControleurBoutique::update(SDLJeu& sdlJeu, Jeu& jeu, bool& quit, AffichageBoutique& affichageBoutique)
{
    EvenementLib event;
    while (AdaptLib::pollEvenement(event))
    {
        int choix = gererEvenement(event, quit, affichageBoutique);
        
        /// annulation et retour a la gestion du cafe
        if (choix == 1)
        {
            jeu.setEtatInterface(Jeu::JEU);
            return;
        }
        /// process pour l'achat et le placement d'une nouvelle table
        else if (choix == 2)
        {
            if (jeu.getCafe().acheterElement(25.0f))
            {
                jeu.ajouterTableEnAttente();
                jeu.setEtatInterface(Jeu::JEU);
                return;
            }
        }
        /// embauche d'un nouveau serveur selon le skin clique
        else if (choix >= 10 && choix <= 14)
        {
            int skin = choix - 10;
            if (jeu.getCafe().acheterElement(prixServeurs[skin])) jeu.getCafe().recruterServeur(skin, vitessesServeurs[skin]);
        }
        /// commande d'une machine et bascule en mode pose dans la cuisine
        else if (choix >= 20 && choix <= 22)
        {
            int idx = choix - 20;
            if (jeu.getCafe().acheterElement(prixMachines[idx]))
            {
                jeu.ajouterMachineEnAttente(static_cast<TypeMachine>(idx + 1));
                jeu.setEtatInterface(Jeu::JEU);
                return;
            }
        }
    }

    /// declenchement du dessin de l'interface par dessus la logique
    affichageBoutique.afficher(sdlJeu.getRenderer(), sdlJeu.getFont(), sdlJeu.getSpriteTable(), sdlJeu.getSpriteServeurs(), jeu.getConstCafe().getArgent(), sdlJeu);
    SDL_RenderPresent(sdlJeu.getRenderer());
}