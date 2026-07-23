#include "SystemeServeurs.h"

/// initialisation du systeme avec les references structurelles du jeu
SystemeServeurs::SystemeServeurs(Cafe& cafeRef, Grille& grilleRef, CalculChemin& calculRef)
    : cafe(cafeRef), grille(grilleRef), calculChemin(calculRef)
{
}

/// recherche d'un client precis dans la base de donnees du cafe
Client* SystemeServeurs::trouverClientParId(unsigned int idClient)
{
    std::vector<Client>& clients = cafe.getEnsembleClients().getClients();

    for (size_t i = 0; i < clients.size(); i++)
    {
        if ((unsigned int)clients[i].getIdClient() == idClient)
            return &clients[i];
    }

    return nullptr;
}

/// identification de l'automate capable de traiter un produit specifique
Machine* SystemeServeurs::trouverMachinePourProduit(TypeProduit produit)
{
    return cafe.getEnsembleMachines().trouverMachinePourProduit(produit);
}

/// selection de l'employe disponible le plus proche pour optimiser le service
Serveur* SystemeServeurs::trouverServeurLibreLePlusProche(const Coordonnee& positionClient)
{
    std::vector<Serveur>& serveurs = cafe.getEnsemblePersonnel().getServeurs();
    Serveur* meilleurServeur = nullptr;
    float meilleureDistance = 0.0f;

    for (size_t i = 0; i < serveurs.size(); i++)
    {
        if (serveurs[i].getEtat() != SERVEUR_LIBRE)
            continue;

        float distance = distanceCarree(serveurs[i].getPosition(), positionClient);

        if (meilleurServeur == nullptr || distance < meilleureDistance)
        {
            meilleurServeur = &serveurs[i];
            meilleureDistance = distance;
        }
    }

    return meilleurServeur;
}

/// utilitaire de calcul de distance euclidienne simplifiee
float SystemeServeurs::distanceCarree(const Coordonnee& a, const Coordonnee& b) const
{
    float dx = a.getX() - b.getX();
    float dy = a.getY() - b.getY();
    return dx * dx + dy * dy;
}

/// test de proximite pour valider l'arrivee a destination
bool SystemeServeurs::positionsProches(const Coordonnee& a, const Coordonnee& b) const
{
    return distanceCarree(a, b) <= 36.0f;
}

/// verification de l'ecoulement d'un temps d'action
bool SystemeServeurs::delaiTermine(Uint32 tickDebut, Uint32 dureeMs) const
{
    return AdaptLib::getTicks() - tickDebut >= dureeMs;
}

/// planification d'un trajet et changement d'etat de l'ia
void SystemeServeurs::lancerDeplacement(Serveur& serveur, const Coordonnee& cible, EtatServeur nouvelEtat)
{
    std::vector<Coordonnee> chemin = calculChemin.calculerCheminServeur(serveur.getPosition(), cible);
    serveur.setChemin(chemin);

    if (!chemin.empty())
        serveur.setCibleCourante(chemin[0]);
    else
        serveur.setCibleCourante(cible);

    serveur.setEtat(nouvelEtat);
}

/// execution du mouvement image par image avec gestion des directions de sprites
bool SystemeServeurs::updateDeplacement(Serveur& serveur, const Coordonnee& cible)
{
    Coordonnee posAvant = serveur.getPosition(); 

    if (serveur.aUnChemin())
        serveur.avancerSurChemin();
    else
        serveur.deplacerVersCible();

    Coordonnee posApres = serveur.getPosition();

    float dx = posApres.getX() - posAvant.getX();
    float dy = posApres.getY() - posAvant.getY();
    
    /// determination de l'etat de marche et de l'orientation visuelle
    bool enMouvement = (std::abs(dx) > 0.1 || std::abs(dy) > 0.1);

    if (enMouvement) {
        /// application d'une priorite a l'axe vertical pour la stabilite visuelle
        if (std::abs(dy) * 2.0 > std::abs(dx)) {
            if (dy > 0) serveur.setDirection(4); /// vue de face
            else serveur.setDirection(1);        /// vue de dos
        } else {
            serveur.setDirection(0); /// vue de profil
            
            /// gestion de l'effet miroir pour les deplacements lateraux
            if (dx > 0.5) serveur.setVaAGauche(true); 
            else if (dx < -0.5) serveur.setVaAGauche(false);
        }
    }
    
    serveur.animer(enMouvement);

    /// validation de la fin du trajet
    if ((!serveur.aUnChemin() && serveur.estArriveCible()) || positionsProches(serveur.getPosition(), cible))
    {
        serveur.setPosition(cible);
        return true;
    }

    return false;
}

/// aiguillage vers la preparation du produit suivant ou le service final
void SystemeServeurs::lancerProchaineEtapeCommande(Serveur& serveur, Client& client)
{
    if (!serveur.aEncoreDesProduitsAMettreEnPreparation())
    {
        lancerDeplacement(serveur, client.getPositionC(), SERVEUR_VA_SERVIR);
        return;
    }

    int indexProduit = serveur.getProduitCourantIndex();
    if (indexProduit < 0)
    {
        lancerDeplacement(serveur, client.getPositionC(), SERVEUR_VA_SERVIR);
        return;
    }

    TypeProduit produit = serveur.getCommandeEnCours().getProduit(indexProduit);
    Machine* machine = trouverMachinePourProduit(produit);

    /// gestion du cas ou l'equipement requis est absent
    if (machine == nullptr)
    {
        client.setCommandeCliquee(false);
        client.setEtat(ASSIS_ATTEND_COMMANDE);
        serveur.reinitialiserMission();
        return;
    }

    serveur.setMachineCible(machine);
    lancerDeplacement(serveur, machine->getPosition(), SERVEUR_VA_MACHINE);
}

/// localisation de la porte de sortie
Coordonnee SystemeServeurs::getPositionSortie() const
{
    return grille.caseVersPixel(3, 0);
}

/// affectation d'une mission de prise de commande apres clic joueur
bool SystemeServeurs::assignerMissionCommande(Client& client)
{
    if (!client.getAssis())
        return false;
    if (client.getEtat() != ASSIS_ATTEND_COMMANDE)
        return false;
    if (!client.aUneCommande())
        return false;
    if (client.getCommandeCliquee())
        return false;

    Serveur* serveur = trouverServeurLibreLePlusProche(client.getPositionC());
    if (serveur == nullptr)
        return false;

    serveur->initialiserMissionCommande((unsigned int)client.getIdClient(), client.getCommande());
    lancerDeplacement(*serveur, client.getPositionC(), SERVEUR_VA_PRENDRE_COMMANDE);
    client.setCommandeCliquee(true);
    return true;
}

/// affectation d'une mission d'encaissement apres clic sur le paiement
bool SystemeServeurs::assignerMissionEncaissement(Client& client)
{
    if (client.getEtat() != ATTEND_ARGENT)
        return false;
    if (client.getArgentClique())
        return false;

    Serveur* serveur = trouverServeurLibreLePlusProche(client.getPositionC());
    if (serveur == nullptr)
        return false;

    serveur->reinitialiserMission();
    serveur->setOccupe(true);
    serveur->setClientCibleId((unsigned int)client.getIdClient());
    lancerDeplacement(*serveur, client.getPositionC(), SERVEUR_VA_ENCAISSER);
    client.setArgentClique(true);
    return true;
}

/// gestion de la phase d'approche vers le client assis
void SystemeServeurs::gererServeurVaPrendreCommande(Serveur& serveur, Client& client)
{
    if (updateDeplacement(serveur, client.getPositionC()))
    {
        serveur.setEtat(SERVEUR_PREND_COMMANDE);
        serveur.setTickDebutAction(AdaptLib::getTicks());
    }
}

/// temps d'attente pour simuler la lecture du bon de commande
void SystemeServeurs::gererServeurPrendCommande(Serveur& serveur, Client& client)
{
    if (!delaiTermine(serveur.getTickDebutAction(), 500))
        return;

    client.setEtat(COMMANDE_EN_PREPARATION);
    lancerProchaineEtapeCommande(serveur, client);
}

/// mouvement vers l'automate de cuisine selectionne
void SystemeServeurs::gererServeurVaMachine(Serveur& serveur, Client& client)
{
    Machine* machine = serveur.getMachineCible();

    if (machine == nullptr)
    {
        client.setCommandeCliquee(false);
        client.setEtat(ASSIS_ATTEND_COMMANDE);
        serveur.reinitialiserMission();
        return;
    }

    if (updateDeplacement(serveur, machine->getPosition()))
    {
        machine->setOccupee(true);
        serveur.setEtat(SERVEUR_PREPARE_COMMANDE);
        serveur.setTickDebutAction(AdaptLib::getTicks());
    }
}

/// attente pendant que la machine fabrique le produit
void SystemeServeurs::gererServeurPrepareCommande(Serveur& serveur, Client& client)
{
    Machine* machine = serveur.getMachineCible();

    if (machine == nullptr)
    {
        serveur.reinitialiserMission();
        return;
    }

    Uint32 dureePreparationMs = (Uint32)machine->getDureePreparation() * 1000;
    if (!delaiTermine(serveur.getTickDebutAction(), dureePreparationMs))
        return;

    machine->setOccupee(false);
    serveur.passerAuProduitSuivant();
    lancerProchaineEtapeCommande(serveur, client);
}

/// trajet de retour vers le client avec le plateau
void SystemeServeurs::gererServeurVaServir(Serveur& serveur, Client& client)
{
    if (updateDeplacement(serveur, client.getPositionC()))
    {
        serveur.setEtat(SERVEUR_SERVE_CLIENT);
        serveur.setTickDebutAction(AdaptLib::getTicks());
    }
}

/// validation de la livraison de la commande
void SystemeServeurs::gererServeurSertClient(Serveur& serveur, Client& client)
{
    if (!delaiTermine(serveur.getTickDebutAction(), 500))
        return;

    client.demarrerConsommation();
    serveur.reinitialiserMission();
}

/// approche finale pour recuperer les gains
void SystemeServeurs::gererServeurVaEncaisser(Serveur& serveur, Client& client)
{
    if (updateDeplacement(serveur, client.getPositionC()))
    {
        serveur.setEtat(SERVEUR_ENCAISSE);
        serveur.setTickDebutAction(AdaptLib::getTicks());
    }
}

/// traitement du paiement et declenchement du depart du client
void SystemeServeurs::gererServeurEncaisse(Serveur& serveur, Client& client)
{
    if (!delaiTermine(serveur.getTickDebutAction(), 400))
        return;

    cafe.ajouterArgent(client.getCommande().calculerPrixTotal());
    cafe.enregistrerClientServi();

    std::vector<Coordonnee> cheminSortie = calculChemin.calculerChemin(client.getPositionC(), getPositionSortie());
    client.setArgentClique(false);
    client.setChemin(cheminSortie);
    client.setCibleC(getPositionSortie());
    client.setEtat(SORT_DU_CAFE);
    client.setAssis(false);

    serveur.reinitialiserMission();
}

/// boucle de mise a jour de tous les employes actifs
void SystemeServeurs::update()
{
    std::vector<Serveur>& serveurs = cafe.getEnsemblePersonnel().getServeurs();

    for (size_t i = 0; i < serveurs.size(); i++)
    {
        Serveur& serveur = serveurs[i];

        if (serveur.getEtat() == SERVEUR_LIBRE)
            continue;

        Client* client = trouverClientParId(serveur.getClientCibleId());
        if (client == nullptr)
        {
            serveur.reinitialiserMission();
            continue;
        }

        /// aiguillage des comportements selon l'etat de la mission
        switch (serveur.getEtat())
        {
            case SERVEUR_VA_PRENDRE_COMMANDE:
                gererServeurVaPrendreCommande(serveur, *client);
                break;
            case SERVEUR_PREND_COMMANDE:
                gererServeurPrendCommande(serveur, *client);
                break;
            case SERVEUR_VA_MACHINE:
                gererServeurVaMachine(serveur, *client);
                break;
            case SERVEUR_PREPARE_COMMANDE:
                gererServeurPrepareCommande(serveur, *client);
                break;
            case SERVEUR_VA_SERVIR:
                gererServeurVaServir(serveur, *client);
                break;
            case SERVEUR_SERVE_CLIENT:
                gererServeurSertClient(serveur, *client);
                break;
            case SERVEUR_VA_ENCAISSER:
                gererServeurVaEncaisser(serveur, *client);
                break;
            case SERVEUR_ENCAISSE:
                gererServeurEncaisse(serveur, *client);
                break;
            default:
                break;
        }
    }
}