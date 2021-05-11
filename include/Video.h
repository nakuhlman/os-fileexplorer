#ifndef __VIDEO_H_
#define __VIDEO_H_

#include "FileEntry.h"

class Video : public FileEntry {
    public:
        using FileEntry::FileEntry;
        void setIcon(std::string name, SDL_Renderer* renderer);
};

#endif
