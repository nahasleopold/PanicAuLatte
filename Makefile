CXX ?= g++

CPPFLAGS := -Isrc $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf 2>/dev/null)
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
LDFLAGS ?=
SDL_LIBS := $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf 2>/dev/null)
ifeq ($(strip $(SDL_LIBS)),)
SDL_LIBS := -lSDL2 -lSDL2_image -lSDL2_ttf
endif

COMMON_SOURCES := \
	src/sdljeu.cpp \
	src/sdlsprite.cpp \
	src/AffichageMenu.cpp \
	src/AffichageBoutique.cpp \
	src/AffichageBulles.cpp \
	src/AffichageHUD.cpp \
	src/AffichageCafe.cpp \
	src/ControleurMenu.cpp \
	src/ControleurBoutique.cpp \
	src/ControleurBulles.cpp \
	src/AdaptLib.cpp \
	src/AnimationPersonnage.cpp \
	src/jeu.cpp \
	src/cafe.cpp \
	src/EnsembleClients.cpp \
	src/EnsemblePersonnel.cpp \
	src/EnsembleMeubles.cpp \
	src/Table.cpp \
	src/Chaise.cpp \
	src/Client.cpp \
	src/Serveur.cpp \
	src/coordonnee.cpp \
	src/commande.cpp \
	src/produit.cpp \
	src/Grille.cpp \
	src/CalculChemin.cpp \
	src/Machine.cpp \
	src/EnsembleMachines.cpp \
	src/SystemeClients.cpp \
	src/SystemeServeurs.cpp

COMMON_OBJECTS := $(patsubst src/%.cpp,obj/%.o,$(COMMON_SOURCES))
APP_OBJECT := obj/mainSDL.o
TEST_OBJECT := obj/assert.o

.PHONY: all test docs clean veryclean

all: sdl bin/tests

sdl: $(APP_OBJECT) $(COMMON_OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(SDL_LIBS)

bin/tests: $(TEST_OBJECT) $(COMMON_OBJECTS) | bin
	$(CXX) $(LDFLAGS) $^ -o $@ $(SDL_LIBS)

obj/%.o: src/%.cpp | obj
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

obj bin:
	mkdir -p $@


test: bin/tests
	./bin/tests

docs:
	doxygen doc/doxyfile

clean:
	rm -rf obj bin sdl

veryclean: clean
	rm -rf doc/generated
