#ifndef __ENTRIES_H_
#define __ENTRIES_H_

// INCLUDES (may remove some of these if they are not needed)
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>

typedef struct SDL_Data {
    TTF_Font *font;
    SDL_Texture *icon;
    SDL_Texture *name;
    SDL_Texture *size;
    SDL_Texture *permissions;
} EntryData;

/* Base class for file explorer entries (an instance of this class cannot be created) */
class FileEntry {

    protected:
        FileEntry(std::string name, int size, std::string path, std::string permissions, SDL_Renderer* renderer);
        SDL_Data data;
        int size_in_bytes;
        std::string filepath;
        std::vector<char> permissions;

    public:
        // Concrete methods
        void createNameTexture(std::string name, SDL_Renderer* renderer);
        void createSizeTexture(int size, SDL_Renderer* renderer);
        void createPermissionsTexture(std::string permissions, SDL_Renderer* renderer);
        // Purely virtual methods that will be defined uniquely by each subclass of FileEntry
        virtual void setIcon() = 0;
        virtual void render() = 0;
};

#endif