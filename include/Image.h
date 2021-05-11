#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "FileEntry.h"

class Image : public FileEntry {
    public:
        using FileEntry::FileEntry;
        void setIcon(std::string name, SDL_Renderer* renderer);
};

#endif