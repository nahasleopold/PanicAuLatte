# Panic au Latte

[English version](README.md)

**Panic au Latte** est une simulation de gestion de café en 2D développée en C++ avec SDL2. Le joueur organise son café, place le mobilier et les machines, recrute des serveurs et coordonne le service afin de satisfaire les clients, améliorer la réputation de l'établissement et développer son activité.

Ce projet a été réalisé dans le cadre de l'UE **LIFAPCD - Conception et développement d'applications** à l'Université Claude Bernard Lyon 1.

## Contributeurs

- Leopold Nahas
- Serine Laidouci
- Imane Oulebsir

Les numéros étudiants ont volontairement été retirés du dépôt public.

## Fonctionnalités principales

- Jeu interactif de gestion de café en 2D
- Placement du mobilier et des machines sur une grille
- Gestion de la file d'attente, des places, commandes, services, paiements et départs
- Coordination des tâches et progression des serveurs
- Calcul de chemin par parcours en largeur pour contourner les obstacles
- Menu dynamique selon les machines possédées
- Boutique permettant d'acheter des tables, des machines et du personnel
- Gestion des horaires, de l'argent et de la réputation
- Affichage SDL2 avec sprites, menus et HUD
- Tests par assertions sur les comportements essentiels du jeu

## Vue technique

Le projet sépare les entités et systèmes du jeu de la couche d'affichage SDL2. Les fichiers sources sont actuellement regroupés directement dans `src/` :

- `Jeu`, `Cafe`, `Client`, `Serveur`, `Machine`, `Table` et `Chaise` représentent le modèle métier.
- `Grille` et `CalculChemin` gèrent les collisions et le pathfinding.
- `SystemeClients` et `SystemeServeurs` coordonnent les comportements.
- Les classes `Affichage*` et `Controleur*` gèrent le rendu et les interactions.
- `SDLJeu`, `SDLSprite` et `AdaptLib` assurent l'intégration avec SDL2.

```text
.
├── .github/workflows/   # Intégration continue
├── data/                # Images et police utilisées par le jeu
├── doc/                 # Diagrammes, présentation et configuration Doxygen
├── src/                 # Sources et en-têtes C++
├── third_party/         # Licences des composants tiers
├── Makefile
├── README.md
├── README.fr.md
├── SECURITY.md
└── THIRD_PARTY_NOTICES.md
```

## Compiler et lancer le jeu

Depuis la racine du dépôt :

```bash
make
./sdl
```

## Lancer les tests

```bash
make test
```

Le workflow GitHub Actions compile également le projet et lance les tests sous Ubuntu à chaque push et pull request.

## Générer la documentation

Après avoir installé Doxygen :

```bash
make docs
```

La documentation générée est enregistrée dans `doc/generated/` et n'est pas suivie par Git.

## Comment jouer

1. Démarrer une partie depuis le menu principal.
2. Pendant la phase de placement, installer les tables dans la salle et les machines dans la cuisine.
3. Lorsqu'un client est assis, cliquer sur sa bulle de commande pour déclencher le service.
4. Lorsqu'il a terminé, cliquer sur l'icône de paiement afin qu'un serveur l'encaisse.
5. Utiliser la boutique lorsque le café est fermé pour acheter du matériel et recruter du personnel.
6. Le café fonctionne de 8 h à 22 h ; l'horloge permet de forcer sa fermeture.

## Documentation du projet

- [Présentation du projet](doc/PanicAuLattePresentation.pdf)
- [Diagramme de classes](doc/DiagrammeDesClasses.png)
- [Diagramme de Gantt](doc/DiagrammeDeGantt.png)

## Travail de sécurité et de confidentialité effectué

- Suppression des numéros étudiants dans la documentation publique.
- Suppression des métadonnées d'auteur, EXIF, XMP et d'éditeur dans les ressources publiées.
- Remplacement de la police Arial fournie par la police redistribuable DejaVu Sans, avec sa licence.
- Interdiction de copier accidentellement la classe `SDLSprite`, qui possède des pointeurs de ressources SDL.
- Libération des textures avant la destruction du renderer SDL.
- Ajout d'exclusions Git pour les fichiers compilés, archives, journaux, éditeurs et documentations générées.
- Ajout d'une compilation et de tests automatiques avec GitHub Actions.

## Limites connues

- L'application graphique a surtout été conçue et testée dans des environnements de type Unix.
- La fenêtre et plusieurs positions de l'interface sont fixes pour une résolution de 1700 x 1000.
- Certains fichiers sources anciens restent présents dans `src/`, mais ne sont pas compilés par le Makefile actif.
- L'origine et les droits de redistribution de chaque image PNG doivent encore être documentés par les contributeurs.
- Aucune licence logicielle commune n'a encore été choisie par l'ensemble des contributeurs.

## Licence

Aucune licence logicielle n'est actuellement incluse. Tant que les contributeurs n'ont pas choisi de licence et confirmé les droits sur les ressources visuelles, la réutilisation et la redistribution ne sont pas automatiquement autorisées.
