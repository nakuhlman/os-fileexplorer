#ifndef __FILEENTRY_H_
#define __FILEENTRY_H_

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
    public:
        FileEntry();
        FileEntry(std::string name, int size, std::string path, std::string permissions, SDL_Renderer* renderer);
        virtual ~FileEntry();

        // Concrete methods
        void createNameTexture(std::string name, SDL_Renderer* renderer);
        void createSizeTexture(int size, SDL_Renderer* renderer);
        void createPermissionsTexture(std::string permissions, SDL_Renderer* renderer);
        // Purely virtual method that will be defined uniquely by each subclass of FileEntry
        virtual void setIcon() = 0;

    protected:
        SDL_Data data;
        int size_in_bytes;
        std::string filepath;
        std::vector<char> permissions;
};

#endif
