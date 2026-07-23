#include "EnsemblePersonnel.h"

/// constructeur par defaut
EnsemblePersonnel::EnsemblePersonnel() {}

/// accesseurs pour la liste des serveurs (version constante et modifiable)
const std::vector<Serveur>& EnsemblePersonnel::getConstServeurs() const { return serveurs; }
std::vector<Serveur>& EnsemblePersonnel::getServeurs() { return serveurs; }

/// ajout d'un serveur avec vitesse par defaut
void EnsemblePersonnel::ajouterServeur(int skin) {
    ajouterServeur(skin, 2.0f);
}

/// creation et positionnement initial d'un nouveau serveur selon son skin
void EnsemblePersonnel::ajouterServeur(int skin, float vitesse) {
    Serveur s;
    s.setIdServeur((int)serveurs.size());
    s.setIdSkin(skin);
    s.setVitesse(vitesse);
    /// calcul d'un decalage pour ne pas superposer les serveurs au spawn
    int decalageX = 800 + ((int)serveurs.size() * 60);
    s.setPosition(Coordonnee(decalageX, 400));
    serveurs.push_back(s);
}

/// mise a jour logique de l'ensemble du personnel
void EnsemblePersonnel::update() {
    for (size_t i = 0; i < serveurs.size(); i++) {
        serveurs[i].update();
    }
}

/// affichage des infos du personnel dans la console pour le debug
void EnsemblePersonnel::afficher() const {
    std::cout << "Nombre de serveurs : " << serveurs.size() << "\n\n";
    for (size_t i = 0; i < serveurs.size(); i++) {
        std::cout << "Serveur " << i << " " << std::endl;
        serveurs[i].afficher();
    }
}

/// recuperation du nombre total d'employes
int EnsemblePersonnel::getNombreServeurs() const {
    return (int)serveurs.size();
}