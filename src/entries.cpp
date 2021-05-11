#include "entries.h"

// Constructor for a subclass of FileEntry (this con)
FileEntry::FileEntry(std::string name, int size, std::string path, std::string permissions, SDL_Renderer* renderer) {
    // Set the font to the font in the resrc folder that will be used by all file explorer entry instances
    data.font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 25);
    // Use the name parameter to create a texture for the name of the file explorer entry
    createNameTexture(name, renderer);
    // Use the size parameter to create a texture for the name of the file explorer entry
    createSizeTexture(size, renderer);
    // Set permissions
    createPermissionsTexture(permissions, renderer);
    // Set path
    filepath = path;
}

// Creates and saves a name texture using the parameters passed into the FileEntry instance's constructor
void FileEntry::createNameTexture(std::string name, SDL_Renderer* renderer) {
    SDL_Color name_color = {0, 0, 0}; 
    SDL_Surface *surf = TTF_RenderText_Solid(data.font, name.c_str(), name_color);
    data.name = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}
        
// Creates and saves a size texture using the parameters passed into the FileEntry instance's constructor
void FileEntry::createSizeTexture(int size, SDL_Renderer* renderer) {
    // 'size' needs to be a string
    char size_as_string[20];
    sprintf(size_as_string, "%u", size);

    SDL_Color size_color = {0, 0, 0}; 
    SDL_Surface *surf = TTF_RenderText_Solid(data.font, size_as_string, size_color);
    data.size = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

// Creates and saves a permissions texture using the parameters passed into the FileEntry instance's constructor
void FileEntry::createPermissionsTexture(std::string permissions, SDL_Renderer* renderer) {
    SDL_Color permissions_color = {0, 0, 0}; 
    SDL_Surface *surf = TTF_RenderText_Solid(data.font, permissions.c_str(), permissions_color);
    data.permissions = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

/**************************************************************/
/**           FILEENTRY SUBCLASSES/IMPLEMENTATIONS           **/
/**  these classes inherit from FileEntry, but also provide  **/
/**    specific implementations of purely virtual methods    **/
/**************************************************************/

// Set the icon for a directory
void Directory::setIcon(std::string name, SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/folder_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

// Set the icon for an executable
void Executable::setIcon(std::string name, SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/executable_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void Image::setIcon(std::string name, SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/image_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void Video::setIcon(std::string name, SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/video_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void CodeFile::setIcon(std::string name, SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/codefile_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void OtherFile::setIcon(std::string name, SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/otherfile_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}
