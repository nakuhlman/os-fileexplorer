#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include "CodeFile.h"
#include "Directory.h"
#include "Executable.h"
#include "FileEntry.h"
#include "Image.h"
#include "OtherFile.h"
#include "Video.h"

#define WIDTH 800
#define HEIGHT 600

// Structure containing all data needed for application
typedef struct AppData {
    TTF_Font *font;
    // use vectors here for multiple
    std::vector<SDL_Texture* > icons;
    std::vector<SDL_Texture* > phrases;
} AppData;

// Vector of FileEntries containing the various objects whose data will be rendered
std::vector<FileEntry> ExplorerEntries;

void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);
void getDirectoryEntries(std::string dirname, SDL_Renderer* renderer);

int main(int argc, char **argv)
{
    char *home = getenv("HOME");
    printf("HOME: %s\n", home);

    // initializing SDL as Video
    SDL_Init(SDL_INIT_VIDEO);
    // initialize PNG IMG library
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    // create window and renderer
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    // initialize and perform rendering loop
    AppData data;
    initialize(renderer, &data);
    render(renderer, &data);
    SDL_Event event;
    SDL_WaitEvent(&event);

    getDirectoryEntries("/home/nkuhlman/", renderer);
    while (event.type != SDL_QUIT)
    {
        SDL_WaitEvent(&event);
        render(renderer, &data);
    }

    // clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    return 0;
}

// Initializes the rendering color and gathers AppData
void initialize(SDL_Renderer* renderer, AppData* data_ptr)
{
    // set color of background when erasing frame
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    // load font
    data_ptr->font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 50);

    // ONLY NEED TO DO THE FOLLOWING ONE TIME 
    // load the folder icon


    // set phrase color to black
    /*
    SDL_Color phrase_color = {0, 0, 0}; 
    SDL_Surface *phrase_surf = TTF_RenderText_Solid(data_ptr->font, "Home", phrase_color);
    data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    SDL_FreeSurface(phrase_surf);
    */
}

// Uses AppData to render objects and phrases to the window
void render(SDL_Renderer* renderer, AppData* data_ptr)
{
    // reset render color to gray
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    // erase renderer content from the previous rendering
    SDL_RenderClear(renderer);
    
    // TODO: draw!

    // Draw the scroll bar across the right side of the window and color it purple
    SDL_Rect scroll_bar_container = {785, 0, 20, 600}; 
    SDL_SetRenderDrawColor(renderer, 81, 12, 118, 255);
    SDL_RenderFillRect(renderer, &scroll_bar_container);

    // Draw the upper window header across the top of the window and color it purple
    SDL_Rect scroll_bar = {785, 0, 20, 100}; 
    SDL_SetRenderDrawColor(renderer, 152, 153, 155, 255);
    SDL_RenderFillRect(renderer, &scroll_bar);

    // Create a container for icons: {x, y, width, height}
    SDL_Rect icon_container = {200, 100, 45, 45};

    // FOR THE CURRENT RENDER (that is, the current directory)
    /* while(all files in the current directory haven't been looped over) {
        for the current entry...

        switch(filetype) {
            case Directory
                // create a directory instance with name, size, permissions, path, etc.
                // add the directory instance to ExplorerEntries
                // break;

            case Executable
                // create an executable instance with name, size, permissions, path, etc.
                // add the executable instance to ExplorerEntries
                // break;

            case Image
                ...(same as above, except with images)

            case Video
                ...(same as above, except with videos)

            case Code
                ...(same as above, except with code)

            case Other
                ...(same as above, except with other files)

            default
        }



    }
    // Render and wait for another event


    */


    //SDL_RenderCopy(renderer, data_ptr->folder_icon, NULL, &icon_container);

    // move the y position down and render another icon
    icon_container.y = 150;
    //SDL_RenderCopy(renderer, data_ptr->folder_icon, NULL, &icon_container);

    SDL_Rect phrase_container = {50, 5, 75, 45};
    //SDL_QueryTexture(data_ptr->phrase, NULL, NULL, &(phrase_container.x), &(phrase_container.y));
    //SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &phrase_container);

    SDL_Rect sidebar = {175, 0, 5, 600};
    SDL_SetRenderDrawColor(renderer, 81, 12, 118, 255);
    SDL_RenderFillRect(renderer, &sidebar);


    // show rendered frame
    SDL_RenderPresent(renderer);
}

void getDirectoryEntries(std::string dirname, SDL_Renderer* renderer)
{
    // Struct containing the information about the current directory
    struct stat info;
    // Gather information about the current directory and store it in 'info'
    int err = stat(dirname.c_str(), &info);
    // As long as the passed-in dirname matched with a valid directory, start looking at that directory's contents
    if (err == 0 && S_ISDIR(info.st_mode))
    {
        // Stores information about the files in the current directory
        std::vector<std::string> files;
        DIR* dir = opendir(dirname.c_str());
        struct dirent *entry;
        // Read in the contents of the directory to the 'files' vector
        while ((entry = readdir(dir)) != NULL) {
            files.push_back(entry->d_name);
        }
        closedir(dir);

        // Sort the files in the files vector in alphabetical order using the sort() function
        std::sort(files.begin(), files.end());
        
        // Stores information about each individual entry (file or another directory) within the current directory
        struct stat file_info;

        // Loop through all the files that were read in
        for(int i = 0; i < files.size(); i++) {
            std::string curfile_name = files[i];
            std::string curfile_path = dirname + "/" + files[i].c_str();
            int curfile_size = file_info.st_size;
            std::string curfile_perms = "rwxrr"; // temporary test (need a function for finding perms, using stat + S_..)

            err = stat(curfile_path.c_str(), &file_info);
            if(err == 1) { std::cout << "DEBUG MSG: something went wrong obtaining file info" << std::endl;}

            /* THE CURRENT ENTRY IS A DIRECTORY */
            if(S_ISDIR(file_info.st_mode)) {

                // Create an instance of FileEntry::Directory, constructing it with values parsed from the current file

                // Add the FileEntry::Directory to the ExplorerEntries array 


            /* THE CURRENT ENTRY IS A REGULAR FILE */
            } else if(S_ISREG(file_info.st_mode)) {

                // Find the "." character in the current file name
                std::size_t pos = files[i].find(".");     
                // Make a substring of the current file's extension by making a substring from "." to the end of the string
                std::string extension = files[i].substr(pos);

                /* EXECUTABLE (the current user has execute permissions (what about groups and others?)) */
                if(file_info.st_mode & S_IXUSR) {
                    // Create an instance of FileEntry::Executable, constructing it with values parsed from the current file

                    // Add the FileEntry::Executable to the ExplorerEntries array 
                }

                /* IMAGE (file extension matches) */
                else if(extension == ".jpg" || extension == ".jpeg" || extension == ".png" ||
                        extension == ".tif" || extension == ".tiff" || extension == ".gif") {                

                    // Create an instance of FileEntry::Image, constructing it with values parsed from the current file
                    Image img (curfile_name, curfile_size, curfile_path, curfile_perms, renderer);

                    // Add the FileEntry::Image to the ExplorerEntries array 
                    ExplorerEntries.push_back(img);

                /* VIDEO (file extension matches) */
                } else if(extension == ".mp4" || extension == ".mov" || extension == ".mkv" ||
                          extension == ".avi" || extension == ".webm") {

                    // Create an instance of FileEntry::Video, constructing it with values parsed from the current file

                    // Add the FileEntry::Video to the ExplorerEntries array 

                /* CODE FILE (file extension matches) */
                } else if(extension == ".h" || extension == ".c"    || extension == ".cpp" ||
                          extension == ".py"|| extension == ".java" || extension == ".js") {

                    // Create an instance of FileEntry::CodeFile, constructing it with values parsed from the current file

                    // Add the FileEntry::CodeFile to the ExplorerEntries array 

                /* OTHER FILE (none of the above) */
                } else {

                    // Create an instance of FileEntry::OtherFile, constructing it with values parsed from the current file

                    // Add the FileEntry::OtherFile to the ExplorerEntries array 

                }


            /* THE CURRENT ENTRY IS NOT A DIRECTORY OR REGULAR FILE */
            } else { continue; }
        }
    }
    else
    {
        fprintf(stderr, "Error: directory argument passed into getDirectoryEntries '%s' not found\n", dirname.c_str());
    }
}





/***** THE FOLLOWING COULD BE HELPFUL FOR RECURSIVE FILE VIEWING LATER ********
//void listDirectory(std::string dirname, int indent = 0);
void listDirectory(std::string dirname, int indent)
{
	std::string space = "";
	for(int i = 0; i < indent; i++) { space += " "; }

    struct stat info;
    int err = stat(dirname.c_str(), &info);
    std::vector<std::string> files;
    if (err == 0 && S_ISDIR(info.st_mode)) {
        DIR* dir = opendir(dirname.c_str());		
		
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            files.push_back(entry->d_name);
        }
        closedir(dir);

	std::sort(files.begin(), files.end());
	
	int i, file_err;
	struct stat file_info;

	for(int i = 0; i < files.size(); i++) {
		std::string full_path = dirname + "/" + files[i];
		file_err = stat(full_path.c_str(), &file_info);
		if(file_err) {
			fprintf(stderr, "Uh, oh, shouldnt be here");
		} else if(S_ISDIR(file_info.st_mode)) {
			printf("%s%s (directory)\n", space.c_str(), files[i].c_str());
			if(files[i] != "." && files[i] != "..") {
				listDirectory(full_path, indent + 1);
			}				
		} else {
			printf("%s%s (directory)\n", space.c_str(), files[i].c_str());
		}
    }
}}*/