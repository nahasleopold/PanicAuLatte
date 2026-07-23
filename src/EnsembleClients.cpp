#include "EnsembleClients.h"

/// init du compteur d'identifiants uniques
EnsembleClients::EnsembleClients() : prochainId(1) {}

/// accesseurs pour la liste complete des clients
const std::vector<Client>& EnsembleClients::getConstClients() const { return clients; }
std::vector<Client>& EnsembleClients::getClients() { return clients; }

/// enregistrement d'un nouveau client avec attribution d'id
void EnsembleClients::ajouterClient(const Client& client)
{
    Client copie = client;
    copie.setIdClient(prochainId++);
    clients.push_back(copie);
}

/// calcul des positions dans la file d'attente selon le rang
void EnsembleClients::mettreAJourFileAttente()
{
    int nombreEnFile = 0;
    for (size_t i = 0; i < clients.size(); i++)
        if (clients[i].getEtat() == EN_ATTENTE_FILE) nombreEnFile++;

    int rangFile = 0;
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].getEtat() == EN_ATTENTE_FILE)
        {
            /// espacement vertical des clients devant le comptoir
            float xFile = 200;
            float yFile = 400 + 95 * (nombreEnFile - 1 - rangFile);
            clients[i].setCibleC(Coordonnee(xFile, yFile));
            rangFile++;
        }
    }
}

/// maj logique de chaque client de la liste
void EnsembleClients::update() {
    for (size_t i = 0; i < clients.size(); i++) {
        clients[i].update();
    }
}

/// affichage des stats de population dans la console
void EnsembleClients::afficher() const {
    std::cout << "Nombre total de clients : " << clients.size() << "\n";
    std::cout << "Nombre de clients en file : " << getNombreClientsEnFile() << "\n\n";
    for (size_t i = 0; i < clients.size(); i++) {
        std::cout << "Client " << i << " " << std::endl;
        clients[i].afficher();
    }
}

/// suppression des clients ayant quitte l'ecran
void EnsembleClients::supprimerClientsPartis()
{
    for (size_t i = 0; i < clients.size(); )
    {
        if (clients[i].getEtat() == PARTI) clients.erase(clients.begin() + i);
        else i++;
    }
}

/// getters pour le denombrement des clients
int EnsembleClients::getNombreClients() const { return (int)clients.size(); }
int EnsembleClients::getNombreClientsEnFile() const {
    int nb = 0;
    for (size_t i = 0; i < clients.size(); i++)
        if (clients[i].getEtat() == EN_ATTENTE_FILE) nb++;
    return nb;
}

/// acces a un client specifique par son index
Client& EnsembleClients::getClient(int index) { return clients[index]; }