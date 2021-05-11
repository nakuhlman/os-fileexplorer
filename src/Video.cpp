#include "FileEntry.h"
#include "Video.h"

void Video::setIcon(std::string name, SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/video_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}