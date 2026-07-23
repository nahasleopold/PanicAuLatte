#include "Serveur.h"
#include "Machine.h"

/// initialisation d'un nouvel employe avec ses stats de base
Serveur::Serveur()
{
    idServeur = 0;
    experience = 0;
    vitesse = 2.0f;
    position = Coordonnee(800, 800);
    occupe = false;
    idSkin = 0;
    etat = SERVEUR_LIBRE;
    clientCibleId = 0;
    aCommandeEnCours = false;

    cibleCourante = position;
    indexProduitActuel = 0;
    machineCible = nullptr;
    tickDebutAction = 0;
}

/// accesseurs pour l'etat civil et les coordonnees
Coordonnee Serveur::getAnciennePosition() const { return anciennePosition; }
int Serveur::getIdServeur() const { return idServeur; }
int Serveur::getExperience() const { return experience; }
float Serveur::getVitesse() const { return vitesse; }
Coordonnee Serveur::getPosition() const { return position; }
bool Serveur::getOccupe() const { return occupe; }
EtatServeur Serveur::getEtat() const { return etat; }
unsigned int Serveur::getClientCibleId() const { return clientCibleId; }
const Commande& Serveur::getCommandeEnCours() const { return commandeEnCours; }
bool Serveur::getACommandeEnCours() const { return aCommandeEnCours; }
Coordonnee Serveur::getCibleCourante() const { return cibleCourante; }
const std::vector<int>& Serveur::getIndicesProduitsRestants() const { return indicesProduitsRestants; }
int Serveur::getIndexProduitActuel() const { return indexProduitActuel; }
Machine* Serveur::getMachineCible() const { return machineCible; }
Uint32 Serveur::getTickDebutAction() const { return tickDebutAction; }
int Serveur::getIdSkin() const { return idSkin; }

/// mutateurs pour la gestion de carriere et de mission
void Serveur::setIdSkin(int id) { idSkin = id; }
void Serveur::setIdServeur(int id) { idServeur = id; }
void Serveur::setExperience(int exp) { experience = exp; }
void Serveur::setVitesse(float v) { vitesse = v; }
void Serveur::setPosition(const Coordonnee& pos) { position = pos; }
void Serveur::setOccupe(bool valeur) { occupe = valeur; }
void Serveur::setEtat(EtatServeur nouvelEtat) { etat = nouvelEtat; }
void Serveur::setClientCibleId(unsigned int idClient) { clientCibleId = idClient; }
void Serveur::setCommandeEnCours(const Commande& commande) { commandeEnCours = commande; }
void Serveur::setACommandeEnCours(bool valeur) { aCommandeEnCours = valeur; }
void Serveur::setCibleCourante(const Coordonnee& cible) { cibleCourante = cible; }
void Serveur::setIndicesProduitsRestants(const std::vector<int>& indices) { indicesProduitsRestants = indices; }
void Serveur::setIndexProduitActuel(int index) { indexProduitActuel = index; }
void Serveur::setMachineCible(Machine* machine) { machineCible = machine; }
void Serveur::setTickDebutAction(Uint32 tick) { tickDebutAction = tick; }

/// mise a jour des frames d'animation
void Serveur::animer(bool enMouvement)
{
    animation.animer(enMouvement);
}

/// calcul du temps de preparation en cuisine en millisecondes
int Serveur::getTempsPreparationRestantMs() const
{
    if (etat != SERVEUR_PREPARE_COMMANDE || machineCible == nullptr)
        return 0;

    int ecoule = (int)(AdaptLib::getTicks() - tickDebutAction);
    int total = machineCible->getDureePreparation() * 1000;
    int restant = total - ecoule;
    return restant > 0 ? restant : 0;
}

/// pilotage de la navigation par points de passage
void Serveur::setChemin(const std::vector<Coordonnee>& chemin)
{
    animation.setChemin(cibleCourante, chemin);
}

bool Serveur::aUnChemin() const
{
    return animation.aUnChemin();
}

void Serveur::viderChemin()
{
    animation.viderChemin();
}

/// tests de positionnement relatif a la cible
bool Serveur::estArriveCible() const
{
    return animation.estArriveCible(position, cibleCourante);
}

/// deplacement simple vers le point courant
void Serveur::deplacerVersCible()
{
    anciennePosition = position;
    animation.deplacerVersCible(position, cibleCourante, vitesse);
}

/// progression automatique sur l'ensemble de l'itineraire
void Serveur::avancerSurChemin()
{
    animation.avancerSurChemin(position, cibleCourante, vitesse);
}

/// parametrage d'un nouveau cycle de service pour un client
void Serveur::initialiserMissionCommande(unsigned int idClient, const Commande& commande)
{
    clientCibleId = idClient;
    commandeEnCours = commande;
    aCommandeEnCours = true;
    occupe = true;
    etat = SERVEUR_VA_PRENDRE_COMMANDE;

    indicesProduitsRestants.clear();

    /// remplissage de la liste des taches a effectuer en cuisine
    for (size_t i = 0; i < commande.getProduits().size(); i++)
    {
        indicesProduitsRestants.push_back((int)i);
    }

    indexProduitActuel = 0;
    machineCible = nullptr;
    tickDebutAction = 0;
    viderChemin();
}

/// verification du reste de la commande a traiter
bool Serveur::aEncoreDesProduitsAMettreEnPreparation() const
{
    return !indicesProduitsRestants.empty();
}

int Serveur::getProduitCourantIndex() const
{
    if (indicesProduitsRestants.empty())
        return -1;

    return indicesProduitsRestants[0];
}

/// passage a l'item suivant dans le bon de commande
void Serveur::passerAuProduitSuivant()
{
    if (!indicesProduitsRestants.empty())
        indicesProduitsRestants.erase(indicesProduitsRestants.begin());
}

/// reinitialisation complete apres service ou encaissement
void Serveur::reinitialiserMission()
{
    occupe = false;
    etat = SERVEUR_LIBRE;
    clientCibleId = 0;
    aCommandeEnCours = false;
    viderChemin();
    cibleCourante = position;
    indicesProduitsRestants.clear();
    indexProduitActuel = 0;
    machineCible = nullptr;
    tickDebutAction = 0;
}

/// mise a jour de la progression et de la vitesse du personnel
void Serveur::update()
{
    if (occupe)
        experience++;

    /// paliers de progression pour l'amelioration de la vitesse
    switch (experience)
    {
        case 20: vitesse += 0.3f; break;
        case 40: vitesse += 0.3f; break;
        case 60: vitesse += 0.3f; break;
        default: break;
    }
}

/// debug console de l'etat de l'employe
void Serveur::afficher() const
{
    std::cout << "ID serveur : " << idServeur << "\n";
    std::cout << "Experience : " << experience << "\n";
    std::cout << "Vitesse : " << vitesse << "\n";
    std::cout << "Position : (" << position.getX() << ", " << position.getY() << ")\n";
    std::cout << "Occupe : " << occupe << "\n";
    std::cout << "Etat : " << etat << "\n";
    std::cout << std::endl;
}