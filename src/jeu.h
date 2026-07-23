#ifndef JEU_H
#define JEU_H

#include "cafe.h"
#include "Grille.h"
#include "CalculChemin.h"
#include "SystemeServeurs.h"
#include "SystemeClients.h"
#include "ControleurMenu.h"
#include "ControleurBoutique.h"
#include "ControleurBulles.h"
#include "AdaptLib.h"

class SDLJeu;
class AffichageMenu;
class AffichageBoutique;
class Chaise;

/// noyau central coordonnant la logique, le temps et les interfaces
class Jeu {
private:
    /// indicateur de fonctionnement du programme
    bool enCours;
    /// donnees metier du cafe (meubles, personnel, clients)
    Cafe cafe;
    /// structure spatiale et collisions
    Grille grille;
    /// outil de calcul d'itineraires
    CalculChemin calculChemin;
    /// logique de flux des clients
    SystemeClients systemeClients;
    /// logique d'ia du personnel
    SystemeServeurs systemeServeurs;
    /// modules de controle pour les differentes vues
    ControleurMenu controleurMenu;
    ControleurBoutique controleurBoutique;
    ControleurBulles controleurBulles;

    /// compteurs de temps pour la simulation
    Uint32 dernierTickTemps;
    Uint32 dernierTickSpawnClient;

    /// horloge interne du jeu
    int heure;
    int minute;

    /// parametres de dimensionnement pour le mobilier
    static const int TABLE_COLS = 2;
    static const int TABLE_LIGS = 2;
    static const int CHAISE_LIGS = 2;
    
    /// inventaire temporaire avant placement sur la grille
    int tablesEnAttente;
    int machinesEnAttente[4]; /// 0=Cafe, 1=Matcha, 2=Jus, 3=Dessert

public:
    /// definition des differents ecrans de l'application
    enum EtatInterface { MENU, BOUTIQUE, JEU };

private:
    /// etat actuel de l'affichage
    EtatInterface etatInterface;

public:
    /// initialisation de l'ensemble des systemes
    Jeu();
    
    /// gestion du mobilier et des equipements en attente de pose
    int getTablesEnAttente() const;
    void ajouterTableEnAttente();
    const Cafe& getConstCafe() const;
    Cafe& getCafe();

    /// acces aux donnees de la grille
    const Grille& getConstGrille() const;
    Grille& getGrille();

    /// consultation du temps simule
    int getHeure() const;
    int getMinute() const;

    /// navigation entre les menus et le jeu
    EtatInterface getEtatInterface() const;
    void setEtatInterface(EtatInterface nouvelEtat);

    /// outils de gestion du cycle jour/nuit
    bool cafeEstFerme() const;
    bool cafeEstOuvert() const;
    void forcerOuvertureCafe();
    void forcerFermetureCafe();

    /// verification et placement des entites sur le plan
    bool zoneTableLibre(int col, int lig) const;
    bool placerTableDepuisPixel(int x, int y);

    /// gestion de l'inventaire des machines speciales
    int getMachinesEnAttente(TypeMachine type) const;
    void ajouterMachineEnAttente(TypeMachine type);
    void decrementerMachineEnAttente(TypeMachine type);
    bool placerMachineDepuisPixel(int x, int y, TypeMachine type);

    /// mise a jour de la simulation
    void update();
    
    /// gestion des boucles d'evenements selon l'interface
    void updateInterfaceSDL(SDLJeu& sdlJeu, AffichageMenu& affichageMenu, AffichageBoutique& affichageBoutique, bool& quit);
    void updateJeuSDL(SDLJeu& sdlJeu, bool& quit);

    /// methodes pour le mode console
    void gererEntrees();
    void afficher();
    void bouclePrincipale();

    /// verification de l'activite du jeu
    bool estEnCours() const;
};

#endif