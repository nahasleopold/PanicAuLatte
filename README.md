# Panic au Latte

[Version française](README.fr.md)

**Panic au Latte** is a 2D café-management simulation developed in C++ with SDL2. The player organises the café, places furniture and machines, hires servers, and coordinates service to satisfy customers, improve the café's reputation, and grow the business.

This project was created for the **LIFAPCD - Application Design and Development** course at Université Claude Bernard Lyon 1.

## Contributors

- Leopold Nahas
- Serine Laidouci
- Imane Oulebsir

Student identification numbers were intentionally removed from the public repository.

## Main features

- Interactive 2D café-management gameplay
- Furniture and machine placement on a tile grid
- Customer queue, seating, ordering, service, payment, and departure states
- Server task coordination and progression
- Breadth-first search pathfinding around obstacles
- Dynamic product availability based on owned machines
- In-game shop for tables, machines, and staff
- Café opening hours, money, and reputation systems
- SDL2-based rendering, sprites, menus, and HUD
- Automated C++ assertions for core game behaviour

## Technical overview

The project separates game entities and systems from the SDL2 presentation layer. The current source files are stored in a flat `src/` directory:

- `Jeu`, `Cafe`, `Client`, `Serveur`, `Machine`, `Table`, and `Chaise` implement the domain model.
- `Grille` and `CalculChemin` manage collision rules and pathfinding.
- `SystemeClients` and `SystemeServeurs` coordinate entity behaviour.
- `Affichage*` and `Controleur*` classes manage rendering and interaction.
- `SDLJeu`, `SDLSprite`, and `AdaptLib` provide the SDL2 integration layer.

```text
.
├── .github/workflows/   # Continuous integration
├── data/                # Images and font used by the game
├── doc/                 # Diagrams, presentation, and Doxygen configuration
├── src/                 # C++ source and header files
├── third_party/         # Third-party license notices
├── Makefile
├── README.md
├── README.fr.md
├── SECURITY.md
└── THIRD_PARTY_NOTICES.md
```

## Build and run

From the repository root:

```bash
make
./sdl
```

## Run the tests

```bash
make test
```

The GitHub Actions workflow also builds the project and runs the tests on Ubuntu for each push and pull request.

## Generate the documentation

Install Doxygen and run:

```bash
make docs
```

Generated documentation is written to `doc/generated/` and is excluded from Git.

## How to play

1. Start from the main menu and open the game.
2. During the placement phase, place tables in the dining area and machines in the kitchen.
3. When customers are seated, click their order bubble to assign service.
4. When a customer has finished, click the payment icon to send a server to collect payment.
5. Use the shop while the café is closed to purchase equipment and recruit staff.
6. The café operates from 08:00 to 22:00; the clock control can force closing.

## Project documentation

- [Project presentation](doc/PanicAuLattePresentation.pdf)
- [Class diagram](doc/DiagrammeDesClasses.png)
- [Gantt chart](doc/DiagrammeDeGantt.png)

## Security and privacy work completed for publication

- Removed student identification numbers from public documentation.
- Removed editor, EXIF, XMP, and document-author metadata from published assets.
- Replaced the bundled Arial file with the redistributable DejaVu Sans font and included its license notice.
- Prevented accidental copying of the SDL texture-owning `SDLSprite` class.
- Ensured textures are released before their SDL renderer is destroyed.
- Added repository exclusions for build products, archives, editor files, logs, and generated documentation.
- Added an automated build-and-test workflow.

## Known limitations

- The graphical application has primarily been designed and tested for Unix-like environments.
- The window size and several interface positions are fixed for a 1700 x 1000 layout.
- Some legacy source files remain in `src/` but are not part of the active Makefile build.
- The origin and redistribution rights of every PNG asset still need to be documented by the contributors.
- No repository-wide software license has been selected by all contributors yet.

## License

No software license is currently included. Until all contributors agree on a license and confirm the rights for the visual assets, reuse and redistribution are not automatically granted.
