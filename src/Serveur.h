#ifndef SERVEUR_H
#define SERVEUR_H

#include <iostream>
#include <vector>
#include "AnimationPersonnage.h"
#include "coordonnee.h"
#include "commande.h"

class Machine;

/// differents etats de la machine a etats pour le comportement de l'ia
enum EtatServeur
{
    SERVEUR_LIBRE,
    SERVEUR_VA_PRENDRE_COMMANDE,
    SERVEUR_PREND_COMMANDE,
    SERVEUR_VA_MACHINE,
    SERVEUR_PREPARE_COMMANDE,
    SERVEUR_VA_SERVIR,
    SERVEUR_SERVE_CLIENT,
    SERVEUR_VA_ENCAISSER,
    SERVEUR_ENCAISSE
};

/// representation d'un employe du cafe et de sa logique de travail
class Serveur
{
private:
    /// caracteristiques personnelles et statistiques
    int idServeur;
    int experience;
    float vitesse;
    Coordonnee position;
    bool occupe;
    int idSkin;
    EtatServeur etat;

    /// suivi de la mission actuelle et du client cible
    unsigned int clientCibleId;

    /// details de la commande en cours de traitement
    Commande commandeEnCours;
    bool aCommandeEnCours;

    /// donnees de navigation et de deplacement
    Coordonnee cibleCourante;
    Coordonnee anciennePosition;

    /// gestion de la file de production en cuisine
    std::vector<int> indicesProduitsRestants;
    int indexProduitActuel;

    /// reference vers l'equipement de cuisine utilise
    Machine* machineCible;

    /// compteurs de temps et moteur d'animation interne
    Uint32 tickDebutAction;
    AnimationPersonnage animation;

public:
    /// constructeur et accesseurs pour l'etat et les positions
    Serveur();
    Coordonnee getAnciennePosition() const;
    int getIdServeur() const;
    int getExperience() const;
    float getVitesse() const;
    Coordonnee getPosition() const;
    bool getOccupe() const;
    EtatServeur getEtat() const;
    unsigned int getClientCibleId() const;
    const Commande& getCommandeEnCours() const;
    bool getACommandeEnCours() const;
    Coordonnee getCibleCourante() const;
    const std::vector<int>& getIndicesProduitsRestants() const;
    int getIndexProduitActuel() const;
    Machine* getMachineCible() const;
    Uint32 getTickDebutAction() const;
    int getTempsPreparationRestantMs() const;
    int getIdSkin() const;

    /// mutateurs pour la configuration du serveur
    void setIdSkin(int id);
    void setIdServeur(int id);
    void setExperience(int exp);
    void setVitesse(float v);
    void setPosition(const Coordonnee& pos);
    void setOccupe(bool valeur);
    void setEtat(EtatServeur nouvelEtat);
    void setClientCibleId(unsigned int idClient);
    void setCommandeEnCours(const Commande& commande);
    void setACommandeEnCours(bool valeur);
    void setCibleCourante(const Coordonnee& cible);
    void setIndicesProduitsRestants(const std::vector<int>& indices);
    void setIndexProduitActuel(int index);
    void setMachineCible(Machine* machine);
    void setTickDebutAction(Uint32 tick);

    /// pilotage de la navigation et du pathfinding
    void setChemin(const std::vector<Coordonnee>& chemin);
    bool aUnChemin() const;
    void viderChemin();
    bool estArriveCible() const;
    void deplacerVersCible();
    void avancerSurChemin();

    /// gestionnaire de missions et de commandes
    void initialiserMissionCommande(unsigned int idClient, const Commande& commande);
    bool aEncoreDesProduitsAMettreEnPreparation() const;
    int getProduitCourantIndex() const;
    void passerAuProduitSuivant();

    /// reinitialisation de l'ia apres une tache
    void reinitialiserMission();

    /// interfaces de delegation vers le systeme d'animation
    int getDirection() const { return animation.getDirection(); }
    int getFrameAnim() const { return animation.getFrameAnim(); }
    bool getVaAGauche() const { return animation.getVaAGauche(); }

    void setDirection(int dir) { animation.setDirection(dir); }
    void setVaAGauche(bool gauche) { animation.setVaAGauche(gauche); }

    void animer(bool enMouvement);

    /// mise a jour de la logique et affichage console
    void update();
    void afficher() const;
};

#endif
