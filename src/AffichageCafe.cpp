#include "AffichageCafe.h"
#include "AffichageBulles.h"
#include "AffichageTexte.h"
#include "jeu.h"
#include "cafe.h"
#include <string>

/// affichage de la grille pour aider au placement des meubles
void AffichageCafe::dessinerGrillePlacement(SDL_Renderer* renderer, const Jeu& jeu, SDLSprite& spriteFileAttente) const
{
    const Grille& grille = jeu.getConstGrille();
    int tailleCase = grille.getTailleCase();
    int origineX = grille.getOrigineX();
    int origineY = grille.getOrigineY();
    int colFile = 1;
    int ligFile = 1;
    int largeurFileCases = 4;
    int hauteurFileCases = 9;
    int xFile = origineX + colFile * tailleCase;
    int yFile = origineY + ligFile * tailleCase;
    int largeurFile = largeurFileCases * tailleCase;
    int hauteurFile = hauteurFileCases * tailleCase;

    /// coloration de la zone cuisine en rouge
    for (int lig = 0; lig < grille.getNbLignes(); lig++)
    {
        for (int col = 0; col < grille.getNbColonnes(); col++)
        {
            SDL_Rect rect;
            rect.x = origineX + col * tailleCase;
            rect.y = origineY + lig * tailleCase;
            rect.w = tailleCase;
            rect.h = tailleCase;
            if (grille.caseCuisine(col, lig))
            {
                SDL_SetRenderDrawColor(renderer, 255, 120, 120, 100);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    /// dessin du sprite de la file et des lignes de la grille
    spriteFileAttente.draw(renderer, xFile, yFile, largeurFile, hauteurFile);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 80);
    for (int x = 0; x <= grille.getNbColonnes() * tailleCase; x += tailleCase)
        SDL_RenderDrawLine(renderer, origineX + x, origineY, origineX + x, origineY + grille.getNbLignes() * tailleCase);
    for (int y = 0; y <= grille.getNbLignes() * tailleCase; y += tailleCase)
        SDL_RenderDrawLine(renderer, origineX, origineY + y, origineX + grille.getNbColonnes() * tailleCase, origineY + y);
}

/// fonction principale de rendu du cafe et de tous ses elements
void AffichageCafe::afficher(SDL_Renderer* renderer, TTF_Font* font, Jeu& jeu, AffichageBulles& affichageBulles, SDLSprite& fond, SDLSprite& fondNuit, SDLSprite& spriteClient, SDLSprite spriteServeurs[5], SDLSprite& spriteTable, SDLSprite& spriteChaiseGauche, SDLSprite& spriteChaiseDroite, SDLSprite& spriteFileAttente, SDLSprite& spriteMachineCafe, SDLSprite& spriteMachineMatcha, SDLSprite& spriteMachineJus, SDLSprite& spriteMachineDessert) const
{
    const int LARGEUR_FENETRE = 1700;
    const int HAUTEUR_FENETRE = 1000;
    
    /// choix du fond selon l'etat ouvert/ferme
    if (jeu.cafeEstFerme()) fondNuit.draw(renderer, 0, 0, LARGEUR_FENETRE + 2, HAUTEUR_FENETRE + 2);
    else fond.draw(renderer, 0, 0, LARGEUR_FENETRE + 2, HAUTEUR_FENETRE + 2);

    /// recup de toutes les donnees du cafe
    const Cafe& cafe = jeu.getConstCafe();
    const EnsembleClients& ensembleClients = cafe.getConstEnsembleClients();
    const EnsemblePersonnel& ensemblePersonnel = cafe.getConstEnsemblePersonnel();
    const EnsembleMeubles& ensembleMeuble = cafe.getConstEnsembleMeubles();
    const std::vector<Client>& clients = ensembleClients.getConstClients();
    const std::vector<Serveur>& serveurs = ensemblePersonnel.getConstServeurs();
    const std::vector<Table>& tables = ensembleMeuble.getConstTables();
    const std::vector<Machine>& machines = cafe.getConstEnsembleMachines().getConstMachines();
    const Grille& grille = jeu.getConstGrille();
    
    /// rendu de la file d'attente
    int tailleCase = grille.getTailleCase();
    int origineX = grille.getOrigineX();
    int origineY = grille.getOrigineY();
    int colFile = 1, ligFile = 1;
    int largeurFileCases = 4, hauteurFileCases = 9;
    int xFile = origineX + colFile * tailleCase;
    int yFile = origineY + ligFile * tailleCase;
    spriteFileAttente.draw(renderer, xFile, yFile, largeurFileCases * tailleCase, hauteurFileCases * tailleCase);

    /// dessin des tables et de leurs chaises
    for (size_t i = 0; i < tables.size(); i++)
    {
        const Chaise* chaises = tables[i].getChaises();
        int nbChaises = tables[i].getNbChaises();
        for (int j = 0; j < nbChaises; j++)
        {
            Coordonnee posAff = chaises[j].getPositionAffichage();
            if (chaises[j].getType() == CHAISE_GAUCHE) spriteChaiseGauche.draw(renderer, (int)posAff.getX(), (int)posAff.getY(), chaises[j].getLargeurAffichage(), chaises[j].getHauteurAffichage());
            else spriteChaiseDroite.draw(renderer, (int)posAff.getX(), (int)posAff.getY(), chaises[j].getLargeurAffichage(), chaises[j].getHauteurAffichage());
        }
        Coordonnee posAffTable = tables[i].getPositionAffichage();
        spriteTable.draw(renderer, (int)posAffTable.getX(), (int)posAffTable.getY(), tables[i].getLargeurAffichage(), tables[i].getHauteurAffichage());
    }

    /// rendu des machines de cuisine
    for (size_t i = 0; i < machines.size(); i++)
    {
        Coordonnee pos = machines[i].getPosition();
        switch (machines[i].getType())
        {
            case MACHINE_CAFE: spriteMachineCafe.draw(renderer, (int)pos.getX(), (int)pos.getY(), 80, 80); break;
            case MACHINE_MATCHA: spriteMachineMatcha.draw(renderer, (int)pos.getX(), (int)pos.getY(), 80, 80); break;
            case MACHINE_JUS: spriteMachineJus.draw(renderer, (int)pos.getX(), (int)pos.getY(), 80, 80); break;
            case MACHINE_DESSERT: spriteMachineDessert.draw(renderer, (int)pos.getX(), (int)pos.getY(), 80, 80); break;
        }
    }

    /// affichage des timers de preparation pour les serveurs
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color jaune = {255, 220, 80, 255};
    for (size_t i = 0; i < serveurs.size(); i++)
    {
        if (serveurs[i].getEtat() == SERVEUR_PREPARE_COMMANDE && serveurs[i].getMachineCible() != nullptr)
        {
            const Machine* machine = serveurs[i].getMachineCible();
            int restantSec = (serveurs[i].getTempsPreparationRestantMs() + 999) / 1000;
            Coordonnee pos = machine->getPosition();
            int xTexte = (int)pos.getX() - 5;
            int yTexte = (int)pos.getY() - 30;
            SDL_Rect bgRect = {xTexte - 5, yTexte - 2, 85, 32};
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
            SDL_RenderFillRect(renderer, &bgRect);
            SDL_SetRenderDrawColor(renderer, 255, 220, 80, 255);
            SDL_RenderDrawRect(renderer, &bgRect);
            AffichageTexte::dessinerTexte(renderer, font, "Prep: " + std::to_string(restantSec) + "s", xTexte, yTexte, jaune);
        }
    }

    /// rendu des clients et gestion de leurs animations selon l'etat
    for (size_t i = 0; i < clients.size(); i++)
    {
        Coordonnee pos = clients[i].getPositionC();
        EtatClient etat = clients[i].getEtat();
        int largeAff = 110;
        int hautAff = 110;
        int xAffBase = (int)pos.getX() - (largeAff / 2);
        int yAff = (int)pos.getY() - hautAff + 35;
        int ligne = 0;
        int colonne = 0;
        int ajustementX = 0;

        /// selection de la frame d'animation du client
        if (etat == VERS_CHAISE)
        {
            ligne = 0;
            colonne = (SDL_GetTicks() / 150) % 3;
        }
        else if (etat == SORT_DU_CAFE)
        {
            ligne = 3;
            colonne = (SDL_GetTicks() / 150) % 2;
        }
        else if (etat == EN_ATTENTE_FILE)
        {
            ligne = 0;
            colonne = 0;
        }
        else if (etat == ASSIS_ATTEND_COMMANDE || etat == COMMANDE_EN_PREPARATION || etat == SERVI_EN_CONSOMMATION || etat == ATTEND_ARGENT)
        {
            ligne = 4;
            Chaise* c = clients[i].getChaiseAttribuee();
            if (c != nullptr)
            {
                if (c->getType() == CHAISE_DROITE) { colonne = 2; ajustementX = -15; }
                else { colonne = 0; ajustementX = 0; }
            }
        }

        int xAffFinal = xAffBase + ajustementX;
        spriteClient.drawFrame(renderer, xAffFinal, yAff, largeAff, hautAff, ligne, colonne, 3, 5, false);

        /// affichage du timer quand le client mange
        if (etat == SERVI_EN_CONSOMMATION)
        {
            int restantSec = (clients[i].getTempsConsommationRestantMs() + 999) / 1000;
            int xTexte = (int)pos.getX() - 40;
            int yTexte = (int)pos.getY() - 135;
            SDL_Rect bgRect = {xTexte - 5, yTexte - 2, 100, 32};
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
            SDL_RenderFillRect(renderer, &bgRect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &bgRect);
            AffichageTexte::dessinerTexte(renderer, font, "Conso: " + std::to_string(restantSec) + "s", xTexte, yTexte, blanc);
        }
    }

    /// rendu des bulles d'interaction
    affichageBulles.afficherBullesCommandes(renderer, jeu.getConstCafe().getConstEnsembleClients());
    affichageBulles.afficherBullesArgent(renderer, jeu.getConstCafe().getConstEnsembleClients());

    /// rendu des serveurs et de leurs deplacements
    for (size_t i = 0; i < serveurs.size(); i++)
    {
        Coordonnee pos = serveurs[i].getPosition();
        Coordonnee cible = serveurs[i].getCibleCourante();
        int skinDuServeur = serveurs[i].getIdSkin();
        int xAff = (int)pos.getX() - 45;
        int yAff = (int)pos.getY() - 60;
        int ligne = serveurs[i].getDirection();
        int colonne = serveurs[i].getFrameAnim();
        bool miroir = serveurs[i].getVaAGauche();
        bool marche = (colonne != 0 && serveurs[i].aUnChemin());
        if (marche)
        {
            if (cible.getY() < pos.getY() - 5) { ligne = 1; colonne = ((SDL_GetTicks() / 150) % 2) * 2; }
            else if (cible.getY() > pos.getY() + 5) { ligne = 0; colonne = ((SDL_GetTicks() / 150) % 2) * 2; }
        }
        spriteServeurs[skinDuServeur].drawFrame(renderer, xAff, yAff, 90, 90, ligne, colonne, 3, 5, miroir);
    }

    /// previsualisation du placement d'une nouvelle table
    if (jeu.getTablesEnAttente() > 0)
    {
        dessinerGrillePlacement(renderer, jeu, spriteFileAttente);
        int sourisX, sourisY;
        SDL_GetMouseState(&sourisX, &sourisY);
        int col, lig;
        grille.pixelVersCase(sourisX, sourisY, col, lig);
        if (grille.caseValide(col, lig))
        {
            Table tablePreview(grille.caseVersPixel(col, lig), col, lig);
            Coordonnee posTableAff = tablePreview.getPositionAffichage();
            spriteTable.draw(renderer, (int)posTableAff.getX(), (int)posTableAff.getY(), tablePreview.getLargeurAffichage(), tablePreview.getHauteurAffichage());
            const Chaise* chaisesPreview = tablePreview.getChaises();
            for (int j = 0; j < tablePreview.getNbChaises(); j++)
            {
                Coordonnee posChaiseAff = chaisesPreview[j].getPositionAffichage();
                if (chaisesPreview[j].getType() == CHAISE_GAUCHE) spriteChaiseGauche.draw(renderer, (int)posChaiseAff.getX(), (int)posChaiseAff.getY(), chaisesPreview[j].getLargeurAffichage(), chaisesPreview[j].getHauteurAffichage());
                else spriteChaiseDroite.draw(renderer, (int)posChaiseAff.getX(), (int)posChaiseAff.getY(), chaisesPreview[j].getLargeurAffichage(), chaisesPreview[j].getHauteurAffichage());
            }
        }
        AffichageTexte::dessinerTexte(renderer, font, "Clique dans le cafe pour placer les deux tables (" + std::to_string(jeu.getTablesEnAttente()) + " restante(s))", 550, 200, blanc);
    }
    /// previsualisation du placement d'une machine en cuisine
    else if (jeu.getMachinesEnAttente(MACHINE_CAFE) > 0 || jeu.getMachinesEnAttente(MACHINE_MATCHA) > 0 || jeu.getMachinesEnAttente(MACHINE_JUS) > 0 || jeu.getMachinesEnAttente(MACHINE_DESSERT) > 0)
    {
        dessinerGrillePlacement(renderer, jeu, spriteFileAttente);
        int sourisX, sourisY;
        SDL_GetMouseState(&sourisX, &sourisY);
        int col, lig;
        grille.pixelVersCase(sourisX, sourisY, col, lig);
        if (grille.caseCuisine(col, lig))
        {
            Coordonnee posSnap = grille.caseVersPixel(col, lig);
            if (jeu.getMachinesEnAttente(MACHINE_CAFE) > 0) spriteMachineCafe.draw(renderer, (int)posSnap.getX(), (int)posSnap.getY(), 100, 100);
            else if (jeu.getMachinesEnAttente(MACHINE_MATCHA) > 0) spriteMachineMatcha.draw(renderer, (int)posSnap.getX(), (int)posSnap.getY(), 110, 110);
            else if (jeu.getMachinesEnAttente(MACHINE_JUS) > 0) spriteMachineJus.draw(renderer, (int)posSnap.getX(), (int)posSnap.getY(), 80, 80);
            else if (jeu.getMachinesEnAttente(MACHINE_DESSERT) > 0) spriteMachineDessert.draw(renderer, (int)posSnap.getX(), (int)posSnap.getY(), 80, 80);
        }
        AffichageTexte::dessinerTexte(renderer, font, "Posez la machine dans la zone rouge (CUISINE)", 550, 200, blanc);
    }
}