#ifndef __EXECUTABLE_H_
#define __EXECUTABLE_H_

#include "FileEntry.h"

class Executable : public FileEntry {
    public:
        using FileEntry::FileEntry;
        void setIcon(std::string name, SDL_Renderer* renderer);
};

#endif