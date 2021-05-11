#ifndef __CODEFILE_H_
#define __CODEFILE_H_

#include "FileEntry.h"

class CodeFile : public FileEntry {
    public:
        using FileEntry::FileEntry;
        void setIcon(std::string name, SDL_Renderer* renderer);
};

#endif