#ifndef PRODUIT_H
#define PRODUIT_H

#include <string>

enum TypeProduit
{
    CAFE,
    MATCHA,
    CARROT_CAKE,
    JUS_ORANGE,
    DONUT
};

class Produit
{
private:
    TypeProduit type;
    int dureeConsommation; /// en secondes

public:
    Produit();
    Produit(TypeProduit t);

    TypeProduit getType() const;
    int getDureeConsommation() const;
    std::string getNom() const;
};

#endif