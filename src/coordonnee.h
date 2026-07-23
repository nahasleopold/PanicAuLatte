#ifndef COORDONNEE_H
#define COORDONNEE_H

/// structure de base pour le positionnement 2d
class Coordonnee {
private:
    /// stockage des positions en flottants pour la precision des deplacements
    float x;
    float y;

public:
    /// init a zero ou avec des valeurs precises
    Coordonnee();
    Coordonnee(float xValeur, float yValeur);

    /// lecture des positions horizontales et verticales
    float getX() const;
    float getY() const;

    /// modification des coordonnees
    void setX(float xValeur);
    void setY(float yValeur);
};

#endif