#include "commande.h"
#include <cstdlib>

/// constructeur par defaut vide
Commande::Commande()
{
}

/// ajout d'un article a la commande
void Commande::ajouterProduit(const Produit& produit)
{
    produits.push_back(produit);
}

/// acces en lecture seule a la liste des articles
const std::vector<Produit>& Commande::getProduits() const
{
    return produits;
}

/// somme des temps de consommation individuels
int Commande::getDureeTotale() const
{
    int total = 0;
    for (size_t i = 0; i < produits.size(); i++)
        total += produits[i].getDureeConsommation();
    return total;
}

/// check si le panier est vide
bool Commande::estVide() const
{
    return produits.empty();
}

/// creation d'une commande random basee sur les machines debloquees
Commande Commande::genererCommandeAleatoire(const std::vector<TypeProduit>& produitsDisponibles)
{
    Commande commande;
    if (produitsDisponibles.empty()) return commande;

    /// On tire au sort 1 ou 2 produits (ex: 30% de chance d'en avoir 2)
    int nbProduits = (rand() % 100 < 30) ? 2 : 1;

    for (int i = 0; i < nbProduits; i++)
    {
        int index = rand() % produitsDisponibles.size();
        commande.ajouterProduit(Produit(produitsDisponibles[index]));
    }
    return commande;
}

/// calcul identique pour la duree du repas
int Commande::getTempsTotalConsommation() const
{
    int total = 0;
    for (size_t i = 0; i < produits.size(); i++)
        total += produits[i].getDureeConsommation();
    return total;
}

/// recup du type d'un produit precis par son index
TypeProduit Commande::getProduit(int index) const
{
    return produits[index].getType();
}

/// calcul du prix de vente total du plateau
float Commande::calculerPrixTotal() const
{
    float total = 0.0f;
    for (size_t i = 0; i < produits.size(); i++)
    {
        switch (produits[i].getType())
        {
            case CAFE: total += 2.5f; break;
            case MATCHA: total += 4.0f; break;
            case CARROT_CAKE: total += 4.5f; break;
            case JUS_ORANGE: total += 3.5f; break;
            case DONUT: total += 3.0f; break;
        }
    }
    return total;
}
