#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <vector>
#include <ctime>
#include "coordonnee.h"
#include "commande.h"
#include "AnimationPersonnage.h"

class Chaise;

/// enumeration des differentes phases du parcours client
enum EtatClient {
    EN_ATTENTE_FILE,
    VERS_CHAISE,
    ASSIS_ATTEND_COMMANDE,
    COMMANDE_EN_PREPARATION,
    SERVI_EN_CONSOMMATION,
    ATTEND_ARGENT,
    SORT_DU_CAFE,
    PARTI
};

/// entite gerant la logique individuelle d'un client
class Client {
private:
    /// stats generales, chronos et coordonnees
    unsigned int idClient;
    float satisfaction;
    int tempsAttente;
    Coordonnee PositionC;
    bool assis;
    time_t tempsDebut;
    int palfin;
    Coordonnee cibleC;
    float vitesse;

    /// gestion de la phase d'achat
    EtatClient etat;
    Commande commande;
    bool commandeGeneree;
    bool commandeCliquee;
    std::vector<TypeProduit> menuDisponible;

    /// module gerant les calculs de deplacement et de sprite
    AnimationPersonnage animation;

    /// lien vers le siege reserve
    Chaise* chaiseAttribuee;

    /// flags et timers pour la phase de repas et de facturation
    Uint32 tickDebutConsommation;
    int dureeConsommationMs;
    bool argentClique;
    bool penaliteReputationAppliquee;

public:
    /// init avec stats de base
    Client();

    /// getters pour l'acces en lecture aux stats
    int getIdClient() const;
    float getSatisfaction() const;
    int getTempsAttente() const;
    Coordonnee getPositionC() const;
    bool getAssis() const;
    Coordonnee getCibleC() const;
    EtatClient getEtat() const;
    Chaise* getChaiseAttribuee() const;

    /// setters pour modifier l'etat interne
    void setIdClient(unsigned int id);
    void setSatisfaction(float valeur);
    void setTempsAttente(int temps);
    void setPositionC(const Coordonnee& pos);
    void setAssis(bool valeur);
    void setCibleC(const Coordonnee& cible);
    void setEtat(EtatClient nouvelEtat);
    void setChaiseAttribuee(Chaise* chaise);

    /// deplacement lineaire simple
    bool estArriveCible() const;
    void deplacerVersCible();

    /// creation du panier d'achat
    void genererCommande();
    void setMenuDisponible(const std::vector<TypeProduit>& menu);
    const Commande& getCommande() const;
    bool aUneCommande() const;

    /// statuts d'interaction ui
    bool getCommandeCliquee() const;
    void setCommandeCliquee(bool valeur);

    /// methodes relais pour le pathfinding complexe
    void setChemin(const std::vector<Coordonnee>& chemin);
    bool aUnChemin() const;
    void viderChemin();
    void avancerSurChemin();

    /// gestion du chrono de repas
    void demarrerConsommation();
    bool consommationTerminee() const;

    /// getters et setters post-repas
    bool getArgentClique() const;
    void setArgentClique(bool valeur);
    int getTempsConsommationRestantMs() const;
    bool getPenaliteReputationAppliquee() const;
    void setPenaliteReputationAppliquee(bool valeur);

    /// raccourcis vers l'etat d'animation
    int getDirection() const { return animation.getDirection(); }
    int getFrameAnim() const { return animation.getFrameAnim(); }
    bool getVaAGauche() const { return animation.getVaAGauche(); }

    void setDirection(int d) { animation.setDirection(d); }
    void setVaAGauche(bool g) { animation.setVaAGauche(g); }

    /// trigger de l'update visuel
    void animer(bool enMouvement);

    /// fonctions coeurs appelees a chaque frame
    void update();
    void afficher() const;
};

#endif
