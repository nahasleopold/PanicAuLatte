#include "Client.h"
#include "Chaise.h"

/// constructeur par defaut avec init de toutes les stats
Client::Client()
{
    idClient = 0;
    satisfaction = 100.0f;
    tempsAttente = 0;
    PositionC = Coordonnee(100, 100);
    cibleC = Coordonnee(100, 200);
    assis = false;
    tempsDebut = time(NULL);
    palfin = 0;
    vitesse = 2.0f;
    etat = EN_ATTENTE_FILE;
    commandeGeneree = false;
    commandeCliquee = false;
    chaiseAttribuee = nullptr;
    tickDebutConsommation = 0;
    dureeConsommationMs = 0;
    argentClique = false;
    penaliteReputationAppliquee = false;
}

/// liste des getters pour lire l'etat
int Client::getIdClient() const { return idClient; }
float Client::getSatisfaction() const { return satisfaction; }
int Client::getTempsAttente() const { return tempsAttente; }
Coordonnee Client::getPositionC() const { return PositionC; }
bool Client::getAssis() const { return assis; }
Coordonnee Client::getCibleC() const { return cibleC; }
EtatClient Client::getEtat() const { return etat; }
Chaise* Client::getChaiseAttribuee() const { return chaiseAttribuee; }

/// liste des setters pour modifier l'etat
void Client::setIdClient(unsigned int id) { idClient = id; }
void Client::setSatisfaction(float valeur) { satisfaction = valeur; }
void Client::setTempsAttente(int temps) { tempsAttente = temps; }
void Client::setPositionC(const Coordonnee& pos) { PositionC = pos; }
void Client::setAssis(bool valeur) { assis = valeur; }
void Client::setCibleC(const Coordonnee& cible) { cibleC = cible; }
void Client::setEtat(EtatClient nouvelEtat) { etat = nouvelEtat; }
void Client::setChaiseAttribuee(Chaise* chaise) { chaiseAttribuee = chaise; }

/// relai vers le composant d'animation pour le check d'arrivee
bool Client::estArriveCible() const
{
    return animation.estArriveCible(PositionC, cibleC);
}

/// deplacement rectiligne vers l'objectif
void Client::deplacerVersCible()
{
    animation.deplacerVersCible(PositionC, cibleC, vitesse);
}

/// recuperation de la carte du cafe pour choisir
void Client::setMenuDisponible(const std::vector<TypeProduit>& menu)
{
    menuDisponible = menu;
}

/// generation d'un panier aleatoire
void Client::genererCommande()
{
    commande = Commande::genererCommandeAleatoire(menuDisponible);
    commandeGeneree = true;
    commandeCliquee = false;
}

/// getters pour la commande
const Commande& Client::getCommande() const { return commande; }
bool Client::aUneCommande() const { return commandeGeneree; }
bool Client::getCommandeCliquee() const { return commandeCliquee; }
void Client::setCommandeCliquee(bool valeur) { commandeCliquee = valeur; }

/// interface avec le systeme de pathfinding
void Client::setChemin(const std::vector<Coordonnee>& chemin)
{
    animation.setChemin(cibleC, chemin);
}

bool Client::aUnChemin() const
{
    return animation.aUnChemin();
}

void Client::viderChemin()
{
    animation.viderChemin();
}

void Client::avancerSurChemin()
{
    animation.avancerSurChemin(PositionC, cibleC, vitesse);
}

/// top depart du chrono pour manger
void Client::demarrerConsommation()
{
    tickDebutConsommation = AdaptLib::getTicks();
    dureeConsommationMs = commande.getTempsTotalConsommation() * 1000;
    argentClique = false;
    etat = SERVI_EN_CONSOMMATION;
}

/// check de fin de repas
bool Client::consommationTerminee() const
{
    if (etat != SERVI_EN_CONSOMMATION) return false;
    return (AdaptLib::getTicks() - tickDebutConsommation) >= (Uint32)dureeConsommationMs;
}

/// gestion du paiement et des malus
bool Client::getArgentClique() const { return argentClique; }
void Client::setArgentClique(bool valeur) { argentClique = valeur; }
bool Client::getPenaliteReputationAppliquee() const { return penaliteReputationAppliquee; }
void Client::setPenaliteReputationAppliquee(bool valeur) { penaliteReputationAppliquee = valeur; }

/// calcul du chrono a afficher a l'ecran
int Client::getTempsConsommationRestantMs() const
{
    if (etat != SERVI_EN_CONSOMMATION)
        return 0;

    int ecoule = (int)(AdaptLib::getTicks() - tickDebutConsommation);
    int restant = dureeConsommationMs - ecoule;
    return restant > 0 ? restant : 0;
}

/// maj de la frame du sprite
void Client::animer(bool enMouvement)
{
    animation.animer(enMouvement);
}

/// cerveau logique execute a chaque boucle de jeu
void Client::update()
{
    time_t tempsActuel = time(NULL);
    tempsAttente = (int)difftime(tempsActuel, tempsDebut);

    /// simple avancee si dans la file
    if (etat == EN_ATTENTE_FILE)
    {
        deplacerVersCible();
    }
    /// suivi de parcours complexe vers une table ou la sortie
    else if (etat == VERS_CHAISE || etat == SORT_DU_CAFE)
    {
        if (aUnChemin())
        {
            avancerSurChemin();

            /// validation de l'arrivee
            if (!aUnChemin() && estArriveCible())
            {
                if (etat == VERS_CHAISE)
                {
                    assis = true;
                    etat = ASSIS_ATTEND_COMMANDE;

                    /// ajustement pile poil sur le siege
                    if (chaiseAttribuee != nullptr)
                    {
                        PositionC = chaiseAttribuee->getPositionAssise();
                        cibleC = chaiseAttribuee->getPositionAssise();
                    }

                    if (!commandeGeneree) genererCommande();
                }
                else if (etat == SORT_DU_CAFE)
                {
                    etat = PARTI;
                }
            }
        }
        else
        {
            /// fallback si pas de chemin actif (secu)
            deplacerVersCible();

            if (estArriveCible())
            {
                if (etat == VERS_CHAISE)
                {
                    assis = true;
                    etat = ASSIS_ATTEND_COMMANDE;

                    if (chaiseAttribuee != nullptr)
                    {
                        PositionC = chaiseAttribuee->getPositionAssise();
                        cibleC = chaiseAttribuee->getPositionAssise();
                    }

                    if (!commandeGeneree) genererCommande();
                }
                else if (etat == SORT_DU_CAFE)
                {
                    etat = PARTI;
                }
            }
        }
    }

    /// transition automatique post-repas
    if (etat == SERVI_EN_CONSOMMATION && consommationTerminee())
    {
        etat = ATTEND_ARGENT;
    }

    /// erosion de la patience
    if (tempsAttente > 30) satisfaction -= 0.5f;
    if (satisfaction < 0) satisfaction = 0;
}

/// trace console pour debug
void Client::afficher() const
{
    std::cout << "ID client : " << idClient << "\n";
    std::cout << "Satisfaction : " << satisfaction << "\n";
    std::cout << "Temps attente : " << tempsAttente << " sec\n";
    std::cout << "Position : (" << PositionC.getX() << ", " << PositionC.getY() << ")\n";
    std::cout << "Assis : " << assis << "\n";
    std::cout << "Commande generee : " << commandeGeneree << "\n";
    std::cout << "Commande cliquee : " << commandeCliquee << "\n";
    std::cout << "Argent clique : " << argentClique << "\n\n";
}