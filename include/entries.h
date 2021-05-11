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
} SDL_Data;

/* Base class for file explorer entries (an instance of this class cannot be created) */
class FileEntry {
    public:
        // Constructor
        FileEntry(std::string name, int size, std::string path, std::string permissions, SDL_Renderer* renderer);

        // Concrete methods
        void createNameTexture(std::string name, SDL_Renderer* renderer);
        void createSizeTexture(int size, SDL_Renderer* renderer);
        void createPermissionsTexture(std::string permissions, SDL_Renderer* renderer);
        virtual void setIcon(std::string, SDL_Renderer*) = 0;

    protected:
        SDL_Data data;
        int size_in_bytes;
        std::string filepath;
        std::vector<char> permissions;
};


class Directory : public FileEntry {
    public:
        using FileEntry::FileEntry;
        // Set the icon for a directory
        void setIcon(std::string name, SDL_Renderer* renderer) override;
};

class Executable : public FileEntry {
    public:
        using FileEntry::FileEntry;
        // Set the icon for an executable
        void setIcon(std::string name, SDL_Renderer* renderer) override;
};

class Image : public FileEntry {
    public:
        using FileEntry::FileEntry;
        void setIcon(std::string name, SDL_Renderer* renderer) override;
};

class Video : public FileEntry {
    public:
        using FileEntry::FileEntry;
        void setIcon(std::string name, SDL_Renderer* renderer) override;
};

class CodeFile : public FileEntry {
    public:
        using FileEntry::FileEntry;
        void setIcon(std::string name, SDL_Renderer* renderer) override;
};

class OtherFile : public FileEntry {
    public:
        using FileEntry::FileEntry;
        void setIcon(std::string name, SDL_Renderer* renderer) override;
};

#endif