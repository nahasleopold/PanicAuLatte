#include "jeu.h"
#include "sdljeu.h"
#include "AffichageMenu.h"
#include "AffichageBoutique.h"
#include <iostream>

/// initialisation du moteur de jeu et des composants core
Jeu::Jeu() : enCours(true), calculChemin(grille), systemeClients(cafe, grille, calculChemin), systemeServeurs(cafe, grille, calculChemin), heure(7), minute(0), tablesEnAttente(2), etatInterface(MENU)
{
    dernierTickTemps = SDL_GetTicks();
    dernierTickSpawnClient = SDL_GetTicks();

    /// initialise l'inventaire des machines à 0 
    machinesEnAttente[MACHINE_CAFE] = 1; 
    machinesEnAttente[MACHINE_MATCHA] = 0;
    machinesEnAttente[MACHINE_JUS] = 0;
    machinesEnAttente[MACHINE_DESSERT] = 0;
}

/// gestion du stock de mobilier en attente de pose
int Jeu::getTablesEnAttente() const {
    return tablesEnAttente;
}
void Jeu::ajouterTableEnAttente() {
    tablesEnAttente++;
}

/// acces a la logique metier du cafe
const Cafe& Jeu::getConstCafe() const {
    return cafe;
}

/// gestion de l'inventaire des machines achetees non placees
int Jeu::getMachinesEnAttente(TypeMachine type) const { 
    return machinesEnAttente[type]; 
}

void Jeu::ajouterMachineEnAttente(TypeMachine type) { 
    machinesEnAttente[type]++; 
}

void Jeu::decrementerMachineEnAttente(TypeMachine type) { 
    if (machinesEnAttente[type] > 0) machinesEnAttente[type]--; 
}

Cafe& Jeu::getCafe() {
    return cafe;
}

/// acces a la structure spatiale du cafe
const Grille& Jeu::getConstGrille() const {
    return grille;
}

Grille& Jeu::getGrille() {
    return grille;
}

/// getters pour l'horloge du jeu
int Jeu::getHeure() const {
    return heure;
}

int Jeu::getMinute() const {
    return minute;
}

/// gestion des etats de l'interface (jeu, menu, boutique)
Jeu::EtatInterface Jeu::getEtatInterface() const
{
    return etatInterface;
}

void Jeu::setEtatInterface(EtatInterface nouvelEtat)
{
    etatInterface = nouvelEtat;
}

/// logique d'ouverture et de fermeture automatique
bool Jeu::cafeEstFerme() const {
    return (heure >= 22 || heure < 8);
}

bool Jeu::cafeEstOuvert() const
{
    return !cafeEstFerme();
}

/// commandes manuelles pour le debogage ou evenements speciaux
void Jeu::forcerOuvertureCafe()
{
    heure = 8;
    minute = 0;
}

void Jeu::forcerFermetureCafe()
{
    heure = 22;
    minute = 0;
    systemeClients.faireSortirClients();
}

/// verification de la place disponible pour une table et ses chaises
bool Jeu::zoneTableLibre(int col, int lig) const
{
    ///ici a la place de dx et dy on pouvais mettre des boucles directement 2 pour lignes et colonnes
    ///or si on veut changer la taille dune cahise ou dune chaise ceci nous permet detre felxible
    ///donc par rapport a la case col, lig
    ///exemple sur cahier
    for (int dy = 0; dy < TABLE_LIGS; dy++)
    {
        for (int dx = 0; dx < TABLE_COLS; dx++)
        {
            if (!grille.caseLibre(col + dx, lig + dy))
                return false;
        }
    }

    for (int dy = 0; dy < CHAISE_LIGS; dy++)
    {
        if (!grille.caseLibre(col - 1, lig + dy))
            return false;
    }

    for (int dy = 0; dy < CHAISE_LIGS; dy++)
    {
        if (!grille.caseLibre(col + 2, lig + dy))
            return false;
    }

    return true;
}

/// processus de pose d'une table sur la grille via clic souris
bool Jeu::placerTableDepuisPixel(int x, int y)
{
    int col, lig;
    grille.pixelVersCase(x, y, col, lig);

    if (!zoneTableLibre(col, lig))
        return false;
    ///on veut pos pour faire ajoutetable on abesoin du point origine
    Coordonnee pos = grille.caseVersPixel(col, lig);
    cafe.ajouterTablePosition(pos, col, lig);

    /// marquage des cases comme occupees dans la grille de pathfinding
    for (int dy = 0; dy < TABLE_LIGS; dy++)
    {
        for (int dx = 0; dx < TABLE_COLS; dx++)
        {
            grille.occuperCase(col + dx, lig + dy);
        }
    }

    for (int dy = 0; dy < CHAISE_LIGS; dy++)
    {
        grille.occuperCase(col - 1, lig + dy);
    }

    for (int dy = 0; dy < CHAISE_LIGS; dy++)
    {
        grille.occuperCase(col + 2, lig + dy);
    }

    return true;
}

/// installation d'une machine dans la zone cuisine
bool Jeu::placerMachineDepuisPixel(int x, int y, TypeMachine type)
{
    int col, lig;
    grille.pixelVersCase(x, y, col, lig);

    ///Vérif
    if (!grille.caseCuisine(col, lig)) {
        return false;
    }

    Coordonnee pos = grille.caseVersPixel(col, lig);

    
    /// la liste de toutes les machines déjà posées
    const std::vector<Machine>& machines = cafe.getConstEnsembleMachines().getConstMachines();
    
    /// On vérif une par une si elle n'est pas déjà sur cette case
    for (size_t i = 0; i < machines.size(); i++) {
        if (machines[i].getPosition().getX() == pos.getX() && 
            machines[i].getPosition().getY() == pos.getY()) {
            return false; /// il y a deja une machine ici, on annule 
        }
    }

    ///  on ajoute la machine
    int tempsPrep = 3; 
    if (type == MACHINE_MATCHA) tempsPrep = 4;
    else if (type == MACHINE_DESSERT) tempsPrep = 5;

    cafe.ajouterMachine(type, pos, tempsPrep);

    /// (On a retiré grille.occuperCase(col, lig) car on gère ça nous-mêmes maintenant)

    return true;
}

/// mise a jour de la logique globale du monde
void Jeu::update() {
    cafe.update();
    systemeServeurs.update();

    /// gestion de l'ecoulement du temps (1s reelle = 2min jeu)
    Uint32 tickActuel = SDL_GetTicks();
    Uint32 ecoule = tickActuel - dernierTickTemps;

    if (ecoule >= 1000) {
        int nbSecondes = ecoule / 1000;
        minute += nbSecondes * 2;

        if (minute >= 60) {
            heure += minute / 60;
            minute = minute % 60;
        }

        if (heure >= 24) {
            heure = heure % 24;
        }

        dernierTickTemps += nbSecondes * 1000;
    }

    /// gestion des clients selon l'etat du cafe
    if (cafeEstFerme())
    {
        systemeClients.faireSortirClients();
    }
    else
    {
        systemeClients.gererArriveeClients(dernierTickSpawnClient, cafeEstFerme());
        systemeClients.assignerClientsAuxChaises();
    }

    systemeClients.nettoyerClientsPartis();

    if (!cafeEstFerme())
    {
        cafe.getEnsembleClients().mettreAJourFileAttente();
    }
}

/// aiguillage des mises a jour selon l'interface active
void Jeu::updateInterfaceSDL(SDLJeu& sdlJeu, AffichageMenu& affichageMenu, AffichageBoutique& affichageBoutique, bool& quit)
{
    if (etatInterface == MENU)
    {
        controleurMenu.update(sdlJeu, *this, quit, affichageMenu);
    }
    else if (etatInterface == BOUTIQUE)
    {
        controleurBoutique.update(sdlJeu, *this, quit, affichageBoutique);
    }
    else if (etatInterface == JEU)
    {
        updateJeuSDL(sdlJeu, quit);
    }
}

/// boucle de gestion des evenements sdl et interactions joueur
void Jeu::updateJeuSDL(SDLJeu& sdlJeu, bool& quit)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            int x = event.button.x;
            int y = event.button.y;
            bool placementEffectue = false;

            /// --- PHASE DE PLACEMENT ---
            if (tablesEnAttente > 0)
            {
                if (placerTableDepuisPixel(x, y))
                {
                    tablesEnAttente--;
                }
                placementEffectue = true;
            }
            else
            {
                TypeMachine typesMachines[4] = {
                    MACHINE_CAFE,
                    MACHINE_MATCHA,
                    MACHINE_JUS,
                    MACHINE_DESSERT
                };

                for (int i = 0; i < 4 && !placementEffectue; i++)
                {
                    TypeMachine type = typesMachines[i];

                    if (machinesEnAttente[type] > 0)
                    {
                        if (placerMachineDepuisPixel(x, y, type))
                        {
                            decrementerMachineEnAttente(type);
                        }
                        placementEffectue = true;
                    }
                }
            }

            /// --- PHASE DE JEU NORMALE ---
            if (!placementEffectue)
            {
                /// clic sur l'horloge pour changer l'etat d'ouverture
                SDL_Rect rectEtat = sdlJeu.getRectEtatCafe();
                bool clicSurEtat = (x >= rectEtat.x && x <= rectEtat.x + rectEtat.w &&
                                    y >= rectEtat.y && y <= rectEtat.y + rectEtat.h);

                if (clicSurEtat)
                {
                    if (cafeEstFerme()) forcerOuvertureCafe();
                    else forcerFermetureCafe();
                }

                /// gestion des commandes via les bulles
                Client* clientCommande = controleurBulles.getClientCliqueCommande(
                    x, y, cafe.getEnsembleClients(), sdlJeu.getAffichageBulles()
                );

                if (clientCommande != nullptr)
                {
                    systemeServeurs.assignerMissionCommande(*clientCommande);
                }

                /// gestion de la collecte d'argent
                Client* clientArgent = controleurBulles.getClientCliqueArgent(
                    x, y, cafe.getEnsembleClients(), sdlJeu.getAffichageBulles()
                );

                if (clientArgent != nullptr)
                {
                    systemeServeurs.assignerMissionEncaissement(*clientArgent);
                }
            }
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    etatInterface = MENU;
                    break;
                case SDLK_q:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
    }

    update();
    sdlJeu.sdlAff();
}

/// gestion des entrees console (version texte)
void Jeu::gererEntrees() {
    char input;
    std::cout << "\nLISTES TOUCHES (c=client, s=serveur, t=table, q=quitter) : ";
    std::cin >> input;

    switch (input) {
        case 'q':
            enCours = false;
            break;

        default:
            std::cout << "Cette touche ne fait rien...\n";
            break;
    }
}

/// affichage console de l'etat du monde
void Jeu::afficher() {
    std::cout << "\n----- ETAT DU CAFE -----\n";
    std::cout << "Heure : ";
    if (heure < 10) std::cout << '0';
    std::cout << heure << ":";
    if (minute < 10) std::cout << '0';
    std::cout << minute << "\n";

    std::cout << "Etat : " << (cafeEstFerme() ? "FERME" : "OUVERT") << "\n";

    cafe.afficher();
}

/// boucle principale pour le mode console
void Jeu::bouclePrincipale() {
    while (enCours) {
        afficher();
        gererEntrees();
        update();
    }

    std::cout << "Fermeture du jeu...\n";
}

bool Jeu::estEnCours() const {
    return enCours;
}