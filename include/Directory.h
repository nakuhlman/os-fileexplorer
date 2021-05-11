#ifndef __DIRECTORY_H_
#define __DIRECTORY_H_

#include "FileEntry.h"

class Directory : public FileEntry {
    public:
        using FileEntry::FileEntry;
        // Set the icon for a directory
        void setIcon(std::string name, SDL_Renderer* renderer);
};

#endif