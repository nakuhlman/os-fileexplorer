#include "entries.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>

class FileEntry {

    protected:

    FileEntry(std::string name, uint32_t size, std::string path, std::string permissions, SDL_Renderer* renderer) {
        // Set the font to the font in the resrc folder that will be used by all file explorer entry instances
        data.font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 25);
        // Use the name parameter to create a texture for the name of the file explorer entry
        createNameTexture(name, renderer);
        // Use the name parameter to create a texture for the name of the file explorer entry
        createSizeTexture(size, renderer);
        // Set permissions
        createPermissionsTexture(permissions, renderer);
        // Set path
        filepath = path;
        // Set permissions


    }

    // The data for the entry
    SDL_Data data;
    // The size of the entry
    uint32_t size_in_bytes;
    // The path to the entry in the filesystem
    std::string filepath;
    // Contains characters representing permissions for the entry ([r,w,x] for user, group, and everyone, respectfully)
    std::vector<char> permissions;

    public:


    /* Sets the name for the file explorer entry based on a string parameter. Implementation is the same for all file entry
     * types, so it is a concrete method. */
    void createNameTexture(std::string name, SDL_Renderer* renderer) {
        SDL_Color name_color = {0, 0, 0}; 
        SDL_Surface *surf = TTF_RenderText_Solid(data.font, name.c_str(), name_color);
        data.name = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
    }

    void createSizeTexture(uint32_t size, SDL_Renderer* renderer) {
        char size_as_string[20];
        sprintf(size_as_string, "%u", size);

        SDL_Color size_color = {0, 0, 0}; 
        SDL_Surface *surf = TTF_RenderText_Solid(data.font, size_as_string, size_color);
        data.size = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
    }

    void createPermissionsTexture(uint32_t size, SDL_Renderer* renderer) {
        SDL_Color size_color = {0, 0, 0}; 
        SDL_Surface *surf = TTF_RenderText_Solid(data.font, itoa(size), size_color);
        data.size = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
    }

        
    virtual void setIcon() = 0;
    virtual void render() = 0;
};


/* Directory implementation of FileEntry */
class Directory : public FileEntry {
    public:
        ~Directory();


        void setName(std::string name, SDL_Renderer* renderer) {
            SDL_Color name_color = {0, 0, 0}; 
            SDL_Surface *surf = TTF_RenderText_Solid(data.font, name.c_str(), name_color);
            data.name = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_FreeSurface(surf);
        }
        void render();  
};



class Executable : public FileEntry {


};

class Image : public FileEntry {
    private:

    public:
};

class Video : public FileEntry {
    private:

    public:
};

class CodeFile : public FileEntry {
    private:

    public:
};

class OtherFile : public FileEntry {
};
