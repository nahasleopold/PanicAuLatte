#include "produit.h"

/// initialisation par defaut sur un produit standard
Produit::Produit() : type(CAFE), dureeConsommation(4)
{
}

/// definition du produit et attribution du temps de degustation specifique
Produit::Produit(TypeProduit t) : type(t), dureeConsommation(4)
{
    switch (type)
    {
        case CAFE:
            dureeConsommation = 3;
            break;
        case MATCHA:
            dureeConsommation = 5;
            break;
        case CARROT_CAKE:
            dureeConsommation = 6;
            break;
        case JUS_ORANGE:
            dureeConsommation = 4;
            break;
        case DONUT:
            dureeConsommation = 4;
            break;
    }
}

/// accesseur pour le type d'article
TypeProduit Produit::getType() const
{
    return type;
}

/// lecture du delai avant que le client ne libere sa table
int Produit::getDureeConsommation() const
{
    return dureeConsommation;
}

/// traduction du type en chaine de caracteres pour l'affichage
std::string Produit::getNom() const
{
    switch (type)
    {
        case CAFE: return "Cafe";
        case MATCHA: return "Matcha";
        case CARROT_CAKE: return "Carrot Cake";
        case JUS_ORANGE: return "Jus d'orange";
        case DONUT: return "Donut";
    }

    return "Inconnu";
}