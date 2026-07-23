#include "sdlsprite.h"
#include <iostream>
#include <string>

using namespace std;

/// initialisation des pointeurs de ressources a nul
SDLSprite::SDLSprite() : m_surface(nullptr), m_texture(nullptr)
{
}

/// liberation de la memoire video et systeme
SDLSprite::~SDLSprite()
{
    release();
}

void SDLSprite::release()
{
    if (m_texture != nullptr)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    if (m_surface != nullptr)
    {
        SDL_FreeSurface(m_surface);
        m_surface = nullptr;
    }
}

/// chargement d'un fichier image et conversion en texture gpu
void SDLSprite::loadFromFile(const char* filename, SDL_Renderer* renderer)
{
    release();
    m_surface = IMG_Load(filename);

    /// tentative de chemin alternatif si le premier echoue
    if (m_surface == nullptr)
    {
        string alt = string("../") + filename;
        m_surface = IMG_Load(alt.c_str());
    }

    /// arret du programme en cas d'absence de ressource critique
    if (m_surface == nullptr)
    {
        cout << "Erreur chargement image : " << filename << endl;
        exit(1);
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);

    if (m_texture == nullptr)
    {
        cout << "Erreur creation texture : " << filename << endl;
        SDL_FreeSurface(m_surface);
        m_surface = nullptr;
        exit(1);
    }
}

/// affichage simple de l'image entiere
void SDLSprite::draw(SDL_Renderer* renderer, int x, int y, int w, int h) const
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;

    /// utilisation des dimensions originales si aucune taille n'est specifiee
    if (w < 0) rect.w = m_surface->w;
    else rect.w = w;

    if (h < 0) rect.h = m_surface->h;
    else rect.h = h;

    SDL_RenderCopy(renderer, m_texture, nullptr, &rect);
}

/// affichage d'une portion specifique de l'image pour les animations
void SDLSprite::drawFrame(SDL_Renderer* renderer, int x, int y, int w, int h, 
                          int ligne, int colonne, 
                          int totalColonnes, int totalLignes, 
                          bool flipHorizontal) const
{
    if (m_texture == nullptr) return; /// CORRIGÉ : m_texture au lieu de texture

    /// 1. On demande à la SDL la taille totale de l'image (les fameux 408x612)
    int texW, texH;
    SDL_QueryTexture(m_texture, nullptr, nullptr, &texW, &texH); /// CORRIGÉ : m_texture

    /// 2. On calcule la taille d'une seule frame (81x204)
    int frameW = texW / totalColonnes;
    int frameH = texH / totalLignes;

    /// 3. Le "Cache" (Rectangle Source) : On dit à la SDL quelle partie découper
    SDL_Rect srcRect;
    srcRect.x = colonne * frameW;
    srcRect.y = ligne * frameH;
    srcRect.w = frameW;
    srcRect.h = frameH;

    /// 4. L'écran (Rectangle Destination) : Où on le dessine, et avec quelle taille
    SDL_Rect destRect = {x, y, w, h};

    /// 5. L'effet Miroir : Super utile pour faire marcher le perso vers la gauche !
    SDL_RendererFlip flip = flipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    /// 6. On dessine !
    SDL_RenderCopyEx(renderer, m_texture, &srcRect, &destRect, 0.0, nullptr, flip); /// CORRIGÉ : m_texture
}

/// recuperation de l'identifiant de texture sdl
SDL_Texture* SDLSprite::getTexture() const
{
    return m_texture;
}