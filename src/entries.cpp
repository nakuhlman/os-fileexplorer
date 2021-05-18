#include "entries.h"

// Constructor for a subclass of FileEntry (this con)
FileEntry::FileEntry(std::string name, std::string type, int size, std::string path, std::string permissions, SDL_Renderer* renderer, TTF_Font* font) {
    // Use the name parameter to create a texture for the name of the file explorer entry
    createNameTexture(name, renderer, font);
    // Use the size parameter to create a texture for the name of the file explorer entry
    createSizeTexture(size, renderer, font);
    // Set permissions
    createPermissionsTexture(permissions, renderer, font);
    entrytype = type;
    // Set path
    filepath = path;
}

// Creates and saves a name texture using the parameters passed into the FileEntry instance's constructor
void FileEntry::createNameTexture(std::string name, SDL_Renderer* renderer, TTF_Font* font) {
    filename = name;
    SDL_Color name_color = {0, 0, 0}; 
    SDL_Surface *surf = TTF_RenderText_Solid(font, name.c_str(), name_color);
    data.name = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}
        
// Creates and saves a size texture using the parameters passed into the FileEntry instance's constructor
void FileEntry::createSizeTexture(int size, SDL_Renderer* renderer, TTF_Font* font) {
    // 'size' needs to be a string
    char size_as_string[20];
    // Bytes
    if(size < 1024.0) {
        sprintf(size_as_string, "%d B", size);
    // Kilobytes
    } else if(size >= 1024.0 && size < 1048576.0) {
        double converted_size = size / 1024.0;
        sprintf(size_as_string, "%.2f KiB", converted_size);
    // Megabytes
    } else if(size >= 1048576.0 && size < 1073741824.0) {
        double converted_size = size / 1048576.0;
        sprintf(size_as_string, "%.2f MiB", converted_size);
    // Gigabytes
    } else if(size > 1073741824.0) {
        double converted_size = size / 1073741824.0;
        sprintf(size_as_string, "%.2f GiB", converted_size);
    }

    SDL_Color size_color = {0, 0, 0}; 
    SDL_Surface *surf = TTF_RenderText_Solid(font, size_as_string, size_color);
    data.size = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

// Creates and saves a permissions texture using the parameters passed into the FileEntry instance's constructor
void FileEntry::createPermissionsTexture(std::string permissions, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color permissions_color = {0, 0, 0}; 
    SDL_Surface *surf = TTF_RenderText_Solid(font, permissions.c_str(), permissions_color);
    data.permissions = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void FileEntry::setCoordinates(int x1, int x2, int y1, int y2, std::string element) {
    if(element == "name") {
        name_coordinates.push_back(x1);
        name_coordinates.push_back(x2);
        name_coordinates.push_back(y1);
        name_coordinates.push_back(y2);
    } else if(element == "icon") {
        icon_coordinates.push_back(x1);
        icon_coordinates.push_back(x2);
        icon_coordinates.push_back(y1);
        icon_coordinates.push_back(y2);
    } else {
        return;
    }
}

/**************************************************************/
/**           FILEENTRY SUBCLASSES/IMPLEMENTATIONS           **/
/**  these classes inherit from FileEntry, but also provide  **/
/**    specific implementations of purely virtual methods    **/
/**************************************************************/

// Set the icon for a directory
void Directory::setIcon(SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/folder_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

// Set the icon for an executable
void Executable::setIcon(SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/executable_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void Image::setIcon(SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/image_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void Video::setIcon(SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/video_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void CodeFile::setIcon(SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/codefile_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

void OtherFile::setIcon(SDL_Renderer* renderer) {
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/otherfile_icon.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data.icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}
