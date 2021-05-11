#ifndef __OTHERFILE_H_
#define __OTHERFILE_H_

#include "FileEntry.h"

class OtherFile : public FileEntry {
    public:
        using FileEntry::FileEntry;
        void setIcon(std::string name, SDL_Renderer* renderer);
};

#endif