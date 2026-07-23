#include "Machine.h"

/// initialisation par defaut d'une machine standard
Machine::Machine()
    : idMachine(0), type(MACHINE_CAFE), position(0, 0), occupee(false), dureePreparation(3)
{
}

/// creation d'une machine avec parametres personnalises
Machine::Machine(int id, TypeMachine t, const Coordonnee& pos, int dureePrep)
    : idMachine(id), type(t), position(pos), occupee(false), dureePreparation(dureePrep)
{
}

/// accesseurs pour l'identifiant et le type de materiel
int Machine::getIdMachine() const
{
    return idMachine;
}

TypeMachine Machine::getType() const
{
    return type;
}

/// recuperation de l'emplacement et de l'etat d'utilisation
const Coordonnee& Machine::getPosition() const
{
    return position;
}

bool Machine::getOccupee() const
{
    return occupee;
}

/// lecture du temps necessaire a la fabrication d'une commande
int Machine::getDureePreparation() const
{
    return dureePreparation;
}

/// mutateurs pour la configuration de l'unite de production
void Machine::setIdMachine(int id)
{
    idMachine = id;
}

void Machine::setType(TypeMachine t)
{
    type = t;
}

void Machine::setPosition(const Coordonnee& pos)
{
    position = pos;
}

/// gestion de la disponibilite de la machine
void Machine::setOccupee(bool valeur)
{
    occupee = valeur;
}

void Machine::setDureePreparation(int duree)
{
    dureePreparation = duree;
}

/// table de correspondance entre la machine et les recettes compatibles
bool Machine::peutPreparer(TypeProduit produit) const
{
    switch (type)
    {
        case MACHINE_CAFE:
            return produit == CAFE;

        case MACHINE_MATCHA:
            return produit == MATCHA;

        case MACHINE_JUS:
            return produit == JUS_ORANGE;

        case MACHINE_DESSERT:
            return produit == DONUT || produit == CARROT_CAKE;

        default:
            return false;
    }
}

/// traduction du type de machine en texte lisible
std::string Machine::getNom() const
{
    switch (type)
    {
        case MACHINE_CAFE: return "Machine cafe";
        case MACHINE_MATCHA: return "Machine matcha";
        case MACHINE_JUS: return "Machine jus";
        case MACHINE_DESSERT: return "Machine dessert";
        default: return "Machine inconnue";
    }
}