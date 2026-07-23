#include "sdljeu.h"
#include <iostream>

using namespace std;

/// dimensions de l'affichage et parametres de cadrage des entites
const int LARGEUR_FENETRE = 1700;
const int HAUTEUR_FENETRE = 1000;

/// configuration du contexte graphique et chargement des ressources
SDLJeu::SDLJeu() : jeu(), window(nullptr), renderer(nullptr), quit(false), font(nullptr)
{
    cout << "Initialisation SDL..." << endl;

    /// demarrage des systemes video de la sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Erreur SDL_Init : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    /// demarrage du moteur de rendu de texte
    if (TTF_Init() == -1)
    {
        cout << "Erreur TTF_Init : " << TTF_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    /// creation de la fenetre principale du cafe
    window = SDL_CreateWindow(
        "Cafe 2D",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGEUR_FENETRE,
        HAUTEUR_FENETRE,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr)
    {
        cout << "Erreur creation fenetre : " << SDL_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    /// mise en place du moteur de rendu accelere
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr)
    {
        cout << "Erreur creation renderer : " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    /// chargement de la police de caracteres pour l'interface
    font = TTF_OpenFont("data/DejaVuSans.ttf", 20);
    if (font == nullptr)
    {
        cout << "Erreur chargement police : " << TTF_GetError() << endl;
        cout << "Verifie que data/DejaVuSans.ttf existe." << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    /// importation massive des textures du jeu
    fond.loadFromFile("data/fond3.png", renderer);
    fondNuit.loadFromFile("data/fondNuit.png", renderer);
    fondMenu.loadFromFile("data/fondmenu.png", renderer);
    spriteClient.loadFromFile("data/client.png", renderer);
    spriteServeurs[0].loadFromFile("data/serveur1.png", renderer);
    spriteServeurs[1].loadFromFile("data/serveur2.png", renderer);
    spriteServeurs[2].loadFromFile("data/serveur3.png", renderer);
    spriteServeurs[3].loadFromFile("data/serveur4.png", renderer);
    spriteServeurs[4].loadFromFile("data/serveur5.png", renderer);
    spriteTable.loadFromFile("data/tabletest.png", renderer);
    spriteChaiseGauche.loadFromFile("data/chaise_gauche.png", renderer);
    spriteChaiseDroite.loadFromFile("data/chaise_droite.png", renderer);
    spriteHeure.loadFromFile("data/heure.png", renderer);
    spriteOuvert.loadFromFile("data/ouvert.png", renderer);
    spriteFerme.loadFromFile("data/ferme.png", renderer);
    spriteReputation.loadFromFile("data/reputation.png", renderer);
    spriteTitre.loadFromFile("data/titre.png", renderer);
    spriteArgentHud.loadFromFile("data/argenthud.png", renderer);
    spriteFileAttente.loadFromFile("data/fileattente.png", renderer);
    spriteMachineCafe.loadFromFile("data/machinecafe.png", renderer);
    spriteMachineMatcha.loadFromFile("data/machinematcha.png", renderer);
    spriteMachineJus.loadFromFile("data/machinejus.png", renderer);
    spriteMachineDessert.loadFromFile("data/machinecake.png", renderer);
    spriteFondBoutique.loadFromFile("data/fond_boutique.png", renderer);
    
    /// deleguer le chargement aux sous-systemes d'affichage
    affichageBulles.chargerSprites(renderer);
    affichageMenu.chargerSprites(renderer);

    /// definition de la zone de clic pour l'etat d'ouverture
    rectEtatCafe.x = 400;
    rectEtatCafe.y = 40;
    rectEtatCafe.w = 210;
    rectEtatCafe.h = 60;

    cout << "SDL initialisee." << endl;
}

/// liberation des ressources memoire a la fermeture
SDLJeu::~SDLJeu()
{
    /// Les textures doivent etre liberees avant le renderer qui les possede.
    affichageMenu.libererSprites();
    affichageBulles.libererSprites();

    fond.release();
    fondNuit.release();
    fondMenu.release();
    spriteClient.release();
    for (SDLSprite& spriteServeur : spriteServeurs) spriteServeur.release();
    spriteTable.release();
    spriteChaiseGauche.release();
    spriteChaiseDroite.release();
    spriteFileAttente.release();
    spriteMachineCafe.release();
    spriteMachineMatcha.release();
    spriteMachineJus.release();
    spriteMachineDessert.release();
    spriteFondBoutique.release();
    spriteHeure.release();
    spriteOuvert.release();
    spriteFerme.release();
    spriteReputation.release();
    spriteTitre.release();
    spriteArgentHud.release();

    if (font != nullptr)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    TTF_Quit();
    SDL_Quit();
}

/// recuperation du rectangle d'interaction de l'horloge
SDL_Rect SDLJeu::getRectEtatCafe() const
{
    return rectEtatCafe;
}

/// procedure de rendu regroupant le cafe et le hud
void SDLJeu::sdlAff()
{
    affichageCafe.afficher(renderer, font, jeu, affichageBulles, fond, fondNuit, spriteClient, spriteServeurs, spriteTable, spriteChaiseGauche, spriteChaiseDroite, spriteFileAttente, spriteMachineCafe, spriteMachineMatcha, spriteMachineJus, spriteMachineDessert);
    affichageHUD.afficher(renderer, font, jeu, jeu.getConstCafe(), spriteTitre, spriteHeure, spriteOuvert, spriteFerme, spriteArgentHud, spriteReputation);
    SDL_RenderPresent(renderer);
}

/// accesseurs pour les composants de rendu
SDL_Renderer* SDLJeu::getRenderer()
{
    return renderer;
}

TTF_Font* SDLJeu::getFont()
{
    return font;
}

AffichageBulles& SDLJeu::getAffichageBulles()
{
    return affichageBulles;
}

SDLSprite& SDLJeu::getSpriteTable()
{
    return spriteTable;
}

SDLSprite* SDLJeu::getSpriteServeurs() {
    return spriteServeurs;
}

SDLSprite& SDLJeu::getFondMenu()
{
    return fondMenu;
}

Jeu& SDLJeu::getJeu()
{
    return jeu;
}

/// boucle de rafraichissement callee sur environ 60 images par seconde
void SDLJeu::sdlBoucle()
{
    while (!quit)
    {
        /// aiguillage des mises a jour selon le menu actuel
        jeu.updateInterfaceSDL(*this, affichageMenu, affichageBoutique, quit);
        SDL_Delay(16);
    }
}