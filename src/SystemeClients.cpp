#include "SystemeClients.h"
#include <iostream>

/// initialisation du systeme avec references aux composants core
SystemeClients::SystemeClients(Cafe& cafeRef, Grille& grilleRef, CalculChemin& calculRef)
    : cafe(cafeRef), grille(grilleRef), calculChemin(calculRef)
{
}

/// recuperation des coordonnees de l'entree du cafe
Coordonnee SystemeClients::getPositionPorte() const
{
    return grille.caseVersPixel(3, 0);
}

/// algorithme de recherche de la chaise la plus proche et accessible
Chaise* SystemeClients::trouverChaiseLibreAccessible(const Coordonnee& depart, std::vector<Coordonnee>& cheminTrouve)
{
    EnsembleMeubles& ensembleMeubles = cafe.getEnsembleMeubles();
    std::vector<Table>& tables = ensembleMeubles.getTables();

    Chaise* meilleureChaise = nullptr;
    std::vector<Coordonnee> meilleurChemin;

    /// parcours de tout le mobilier pour tester la disponibilite
    for (size_t i = 0; i < tables.size(); i++)
    {
        Chaise* chaises = tables[i].getChaisesModifiables();
        int nbChaises = tables[i].getNbChaises();

        for (int j = 0; j < nbChaises; j++)
        {
            if (chaises[j].estOccupee())
                continue;

            /// verification de l'accessibilite physique de la chaise
            std::vector<Coordonnee> acces = chaises[j].getPositionsAcces();

            for (size_t k = 0; k < acces.size(); k++)
            {
                std::vector<Coordonnee> chemin = calculChemin.calculerChemin(depart, acces[k]);

                if (chemin.empty())
                    continue;

                /// selection du chemin le plus court
                if (meilleureChaise == nullptr || chemin.size() < meilleurChemin.size())
                {
                    meilleureChaise = &chaises[j];
                    meilleurChemin = chemin;
                }
            }
        }
    }

    cheminTrouve = meilleurChemin;
    return meilleureChaise;
}

/// logique de generation des clients selon le temps ecoule et l'etat du cafe
void SystemeClients::gererArriveeClients(Uint32& dernierTickSpawnClient, bool cafeFerme)
{
    if (cafeFerme)
        return;

    Uint32 tickActuel = AdaptLib::getTicks();
    Uint32 ecoule = tickActuel - dernierTickSpawnClient;

    /// verification du delai base sur la reputation
    if (ecoule < calculerDelaiArriveeClients())
        return;

    EnsembleClients& ensembleClients = cafe.getEnsembleClients();
    const int maxFile = 4;

    /// creation et positionnement du nouveau client a l'entree
    Client nouveauClient;
    Coordonnee entree = getPositionPorte();
    nouveauClient.setPositionC(entree);
    nouveauClient.setCibleC(entree);
    nouveauClient.setAssis(false);

    std::vector<Coordonnee> cheminVersChaise;
    Chaise* chaise = trouverChaiseLibreAccessible(nouveauClient.getPositionC(), cheminVersChaise);

    /// gestion du refus si le cafe et la file sont satures
    if (chaise == nullptr && ensembleClients.getNombreClientsEnFile() >= maxFile)
    {
        std::cout << "File pleine (max " << maxFile << ")\n";
        dernierTickSpawnClient = tickActuel;
        return;
    }

    /// attribution d'une place ou mise en attente
    if (chaise != nullptr)
    {
        chaise->occuper();
        nouveauClient.setChaiseAttribuee(chaise);
        nouveauClient.setChemin(cheminVersChaise);

        if (!cheminVersChaise.empty())
            nouveauClient.setCibleC(cheminVersChaise.back());

        nouveauClient.setEtat(VERS_CHAISE);
    }
    else
    {
        nouveauClient.setEtat(EN_ATTENTE_FILE);
        ensembleClients.mettreAJourFileAttente();
    }

    /// definition des produits que le client peut commander selon l'equipement
    std::vector<TypeProduit> menu;
    if (cafe.produitDisponible(CAFE)) menu.push_back(CAFE);
    if (cafe.produitDisponible(MATCHA)) menu.push_back(MATCHA);
    if (cafe.produitDisponible(CARROT_CAKE)) menu.push_back(CARROT_CAKE);
    if (cafe.produitDisponible(JUS_ORANGE)) menu.push_back(JUS_ORANGE);
    if (cafe.produitDisponible(DONUT)) menu.push_back(DONUT);

    nouveauClient.setMenuDisponible(menu);
    ensembleClients.ajouterClient(nouveauClient);
    dernierTickSpawnClient = tickActuel;
}

/// tentative de placement pour les clients deja presents dans la file
void SystemeClients::assignerClientsAuxChaises()
{
    EnsembleClients& ensembleClients = cafe.getEnsembleClients();
    std::vector<Client>& clients = ensembleClients.getClients();

    for (size_t i = 0; i < clients.size(); i++)
    {
        Client& client = clients[i];

        if (client.getEtat() != EN_ATTENTE_FILE)
            continue;

        std::vector<Coordonnee> chemin;
        Chaise* chaise = trouverChaiseLibreAccessible(client.getPositionC(), chemin);

        if (chaise == nullptr)
            return;

        chaise->occuper();
        client.setChaiseAttribuee(chaise);
        client.setChemin(chemin);

        if (!chemin.empty())
            client.setCibleC(chemin.back());

        client.setEtat(VERS_CHAISE);
        client.setAssis(false);

        ensembleClients.mettreAJourFileAttente();
        return;
    }
}

/// expulsion de tous les clients lors de la fermeture
void SystemeClients::faireSortirClients()
{
    EnsembleClients& ensembleClients = cafe.getEnsembleClients();
    std::vector<Client>& clients = ensembleClients.getClients();
    Coordonnee sortie = getPositionPorte();

    for (size_t i = 0; i < clients.size(); i++)
    {
        Client& client = clients[i];

        if (client.getEtat() == PARTI || client.getEtat() == SORT_DU_CAFE)
            continue;

        std::vector<Coordonnee> chemin = calculChemin.calculerChemin(client.getPositionC(), sortie);

        if (!chemin.empty())
        {
            client.setCibleC(sortie);
            client.setChemin(chemin);
            client.setEtat(SORT_DU_CAFE);
            client.setAssis(false);
        }
    }
}

/// suppression des objets clients et liberation des chaises apres depart
void SystemeClients::nettoyerClientsPartis()
{
    EnsembleClients& ensembleClients = cafe.getEnsembleClients();
    std::vector<Client>& clients = ensembleClients.getClients();

    for (size_t i = 0; i < clients.size(); )
    {
        if (clients[i].getEtat() == PARTI)
        {
            if (clients[i].getChaiseAttribuee() != nullptr)
            {
                clients[i].getChaiseAttribuee()->liberer();
                clients[i].setChaiseAttribuee(nullptr);
            }

            clients.erase(clients.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

/// calcul dynamique du temps entre deux entrees base sur le score du cafe
Uint32 SystemeClients::calculerDelaiArriveeClients() const
{
    int reputation = cafe.getReputation();
    int palier = reputation / 20;

    Uint32 delai = 10000 - palier * 1000;

    if (delai < 4000)
        delai = 4000;

    return delai;
}

/// application de malus si les temps d'attente depassent les seuils de patience
void SystemeClients::gererReputationAttente()
{
    std::vector<Client>& clients = cafe.getEnsembleClients().getClients();

    for (size_t i = 0; i < clients.size(); i++)
    {
        Client& client = clients[i];

        if (client.getPenaliteReputationAppliquee())
            continue;

        EtatClient etat = client.getEtat();

        if (etat == EN_ATTENTE_FILE || etat == ASSIS_ATTEND_COMMANDE || etat == COMMANDE_EN_PREPARATION)
        {
            if (client.getTempsAttente() >= 30)
            {
                cafe.mettreAJourReputation(-1);
                client.setPenaliteReputationAppliquee(true);
            }
        }
    }
}