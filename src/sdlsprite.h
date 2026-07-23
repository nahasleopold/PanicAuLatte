#ifndef SDLSPRITE_H
#define SDLSPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class SDLSprite {
private:
    SDL_Surface* m_surface;
    SDL_Texture* m_texture;

public:
    SDLSprite();
    ~SDLSprite();

    SDLSprite(const SDLSprite&) = delete;
    SDLSprite& operator=(const SDLSprite&) = delete;

    /// Libere explicitement les ressources SDL possedees.
    void release();

    void loadFromFile(const char* filename, SDL_Renderer* renderer);
    void draw(SDL_Renderer* renderer, int x, int y, int w = -1, int h = -1) const;
    SDL_Texture* getTexture() const;
    /// Permet de dessiner une seule case de l'image (pour l'animation)
    void drawFrame(SDL_Renderer* renderer, int x, int y, int w, int h, 
                   int ligne, int colonne, 
                   int totalColonnes = 5, int totalLignes = 3, 
                   bool flipHorizontal = false) const;
};

#endif