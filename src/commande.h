#ifndef COMMANDE_H
#define COMMANDE_H

#include <vector>
#include "produit.h"

/// gestion du panier et de la facture d'un client
class Commande
{
private:
    /// liste des articles choisis
    std::vector<Produit> produits;

public:
    /// init d'un panier vide
    Commande();

    /// ajout d'un item a la liste
    void ajouterProduit(const Produit& produit);
    
    /// acces en lecture a la liste des achats
    const std::vector<Produit>& getProduits() const;

    /// somme des temps de consommation des items
    int getDureeTotale() const;
    
    /// verif si la liste est vide
    bool estVide() const;
    
    /// creation d'une commande random selon le menu
    static Commande genererCommandeAleatoire(const std::vector<TypeProduit>& produitsDisponibles);
    
    /// calcul de la duree totale a table
    int getTempsTotalConsommation() const;
    
    /// recup du type d'un article precis
    TypeProduit getProduit(int index) const;
    
    /// calcul du prix de vente global
    float calculerPrixTotal() const;
};

#endif