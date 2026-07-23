#include "coordonnee.h"

/// init a zero par defaut
Coordonnee::Coordonnee() {
    x = 0;
    y = 0;
}

/// init avec des valeurs specifiques en flottants
Coordonnee::Coordonnee(float xValeur, float yValeur) {
    x = xValeur;
    y = yValeur;
}

/// recup de l'abscisse
float Coordonnee::getX() const {
    return x;
}

/// recup de l'ordonnee
float Coordonnee::getY() const {
    return y;
}

/// maj de la position horizontale
void Coordonnee::setX(float xValeur) {
    x = xValeur;
}

/// maj de la position verticale
void Coordonnee::setY(float yValeur) {
    y = yValeur;
}