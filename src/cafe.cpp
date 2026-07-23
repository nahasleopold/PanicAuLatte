#include "cafe.h"

/// init des stats de base et embauche des 2 premiers serveurs
Cafe::Cafe() : argent(100), reputation(0), clientsServis(0) {
    ///ensembleMachines.ajouterMachine(MACHINE_CAFE, Coordonnee(1150, 320), 3);


    /// le serveur du depart
    recruterServeur(0);
    recruterServeur(1);
}

/// getters pour les stats globales
float Cafe::getArgent() const {
    return argent;
}

int Cafe::getReputation() const {
    return reputation;
}

/// acces en lecture seule aux listes d'entites
const EnsembleClients& Cafe::getConstEnsembleClients() const {
    return ensembleClients;
}

const EnsemblePersonnel& Cafe::getConstEnsemblePersonnel() const {
    return ensemblePersonnel;
}

const EnsembleMeubles& Cafe::getConstEnsembleMeubles() const {
    return ensembleMeubles;
}

/// acces en ecriture aux listes d'entites
EnsembleClients& Cafe::getEnsembleClients() {
    return ensembleClients;
}

EnsemblePersonnel& Cafe::getEnsemblePersonnel() {
    return ensemblePersonnel;
}

EnsembleMeubles& Cafe::getEnsembleMeubles() {
    return ensembleMeubles;
}

/// ajout d'un nouveau client dans le cafe
void Cafe::ajouterClient(const Client& client) {
    ensembleClients.ajouterClient(client);
}

/// embauche d'un serveur par defaut
void Cafe::recruterServeur(int skin) {
    ensemblePersonnel.ajouterServeur(skin);
}

/// embauche d'un serveur avec stats specifiques
void Cafe::recruterServeur(int skin, float vitesse) {
    ensemblePersonnel.ajouterServeur(skin, vitesse);
}

/// maj du score de reputation sans descendre sous zero
void Cafe::mettreAJourReputation(int valeur) {
    reputation += valeur;
    if (reputation < 0)
        reputation = 0;
}

/// systeme de recompense tous les 5 clients servis
void Cafe::enregistrerClientServi() {
    clientsServis++;
    if (clientsServis % 5 == 0)
        mettreAJourReputation(1);
}

/// deleguation de l'ajout des tables initiales
void Cafe::ajouterTables() {
    ensembleMeubles.ajouterTables();
}

/// placement manuel d'une table sur la grille
void Cafe::ajouterTablePosition(const Coordonnee& pos, int col, int lig) {
    ensembleMeubles.ajouterTablePosition(pos, col, lig);
}

/// getters pour la cuisine et les machines
const EnsembleMachines& Cafe::getConstEnsembleMachines() const
{
    return ensembleMachines;
}

EnsembleMachines& Cafe::getEnsembleMachines()
{
    return ensembleMachines;
}

/// check de la capacite a faire une recette
bool Cafe::produitDisponible(TypeProduit produit) const
{
    return ensembleMachines.produitDisponible(produit);
}

/// installation d'un nouvel equipement
void Cafe::ajouterMachine(TypeMachine type, const Coordonnee& position, int dureePreparation)
{
    ensembleMachines.ajouterMachine(type, position, dureePreparation);
}

/// check du solde bancaire
bool Cafe::peutDepenserArgent(float montant) const
{
    return argent >= montant;
}

/// debit de l'argent avec secu
bool Cafe::depenserArgent(float montant)
{
    if (!peutDepenserArgent(montant))
        return false;

    argent -= montant;
    return true;
}

/// gain d'argent dans la caisse
void Cafe::ajouterArgent(float montant)
{
    argent += montant;
}

/// expulsion de tous les clients vers la porte a la fermeture
void Cafe::faireSortirClients()
{
    EnsembleClients& ensembleClients = getEnsembleClients();

    for (int i = 0; i < ensembleClients.getNombreClients(); i++)
    {
        Client& client = ensembleClients.getClient(i);

        if (client.getEtat() != PARTI && client.getEtat() != SORT_DU_CAFE)
        {
            client.setEtat(SORT_DU_CAFE);

            /// position fixe de la porte
            client.setCibleC(Coordonnee(50, 200));
            client.setAssis(false);
        }
    }
}

/// Dans cafe.cpp
/// validation et paiement d'un item en boutique
bool Cafe::acheterElement(float prix) {
    if (argent >= prix) {
        argent -= prix;
        return true;
    }
    return false;
}

/// cascade des maj logiques a chaque frame
void Cafe::update() {
    ensembleClients.update();
    ensemblePersonnel.update();
    
    ///cuisine.update();
}

/// trace console pour le debug
void Cafe::afficher() const {
    std::cout << "Argent : " << argent << "\n";
    std::cout << "Reputation : " << reputation << "\n\n";

    ensembleClients.afficher();
    std::cout << std::endl;

    ensemblePersonnel.afficher();
    std::cout << std::endl;

    ///cuisine.afficher();
    ///std::cout << std::endl;
}