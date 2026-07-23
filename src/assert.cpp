#include <iostream>
#include <cassert>
#include <vector>

#include "Grille.h"
#include "cafe.h"
#include "jeu.h"
#include "Serveur.h"
#include "Client.h"
#include "CalculChemin.h"
#include "AnimationPersonnage.h"

using namespace std;

void test_grille() {
    cout << "Grille-----------------------------------------------" << endl;
    Grille g;
    
    assert(g.getNbColonnes() == 33);
    assert(g.getNbLignes() == 14);
    
    assert(g.caseValide(0, 0) == true);
    assert(g.caseValide(-1, 5) == false);
    assert(g.caseValide(100, 100) == false);

    assert(g.caseLibre(5, 5) == true);
    g.occuperCase(5, 5);
    assert(g.caseLibre(5, 5) == false);
    g.libererCase(5, 5);
    assert(g.caseLibre(5, 5) == true);

}

void test_cafe() {
    cout << "Cafe-----------------------------------------------" << endl;
    Cafe c;
    
    c.ajouterArgent(500.0f);
    assert(c.depenserArgent(200.0f) == true);
    assert(c.depenserArgent(1000.0f) == false);
}

void test_serveur() {
    cout << "Serveur-----------------------------------------------" << endl;
    Serveur s;
    
    assert(s.getEtat() == SERVEUR_LIBRE);
    assert(s.getVitesse() == 2.0f);
    assert(s.getOccupe() == false);
    
    s.setIdServeur(1);
    assert(s.getIdServeur() == 1);

    s.setOccupe(true);
    s.setExperience(19);
    s.update(); 
    
    assert(s.getExperience() == 20);
    assert(s.getVitesse() > 2.0f);

    s.setEtat(SERVEUR_VA_PRENDRE_COMMANDE);
    assert(s.getEtat() == SERVEUR_VA_PRENDRE_COMMANDE);
    
    s.reinitialiserMission();
    assert(s.getEtat() == SERVEUR_LIBRE);
    assert(s.getOccupe() == false);
}

void test_client() {
    cout << "Client-----------------------------------------------" << endl;
    Client c;
    
    assert(c.getSatisfaction() == 100.0f);
    assert(c.getEtat() == EN_ATTENTE_FILE);
    assert(c.getAssis() == false);
    
    c.setEtat(VERS_CHAISE);
    assert(c.getEtat() == VERS_CHAISE);
    
    c.setSatisfaction(50.0f);
    assert(c.getSatisfaction() == 50.0f);
}

void test_jeu() {
    cout << "Jeu-----------------------------------------------" << endl;
    Jeu j;
    
    assert(j.getEtatInterface() == Jeu::MENU);
    assert(j.getHeure() == 7);
    assert(j.cafeEstFerme() == true);
    
    j.forcerOuvertureCafe();
    assert(j.getHeure() == 8);
    assert(j.cafeEstOuvert() == true);
    assert(j.cafeEstFerme() == false);

    j.forcerFermetureCafe();
    assert(j.getHeure() == 22);
    assert(j.cafeEstFerme() == true);

    j.setEtatInterface(Jeu::BOUTIQUE);
    assert(j.getEtatInterface() == Jeu::BOUTIQUE);
}

void test_calcul_chemin() {
    cout << "CalculChemin-----------------------------------------------" << endl;
    Grille g; 
    CalculChemin calc(g);
    
    Coordonnee depart = g.caseVersPixel(5, 5);
    Coordonnee arrivee = g.caseVersPixel(8, 5);

    vector<Coordonnee> chemin = calc.calculerCheminServeur(depart, arrivee);
    assert(!chemin.empty());
    assert(chemin.back().getX() == arrivee.getX());
    assert(chemin.back().getY() == arrivee.getY());
}

void test_animation() {
    cout << "AnimationPersonnage-----------------------------------------------" << endl;
    AnimationPersonnage anim;
    
    anim.setDirection(3);
    assert(anim.getDirection() == 3);
    
    anim.setVaAGauche(true);
    assert(anim.getVaAGauche() == true);
    
    anim.setVaAGauche(false);
    assert(anim.getVaAGauche() == false);
}

void test_sdl_jeu() {
    cout << "SDLJeu-----------------------------------------------" << endl;
}

int main() {
    test_grille();
    test_cafe();
    test_serveur();
    test_client();
    test_jeu();
    test_calcul_chemin();
    test_animation();
    test_sdl_jeu();
    
    cout << endl;
    cout << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "TOUS LES TESTS SONT PASSES" << endl;
    return 0;
}