#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include "entries.h"

#define WIDTH 800   
#define HEIGHT 600

// Structure containing all data needed for application
typedef struct AppData {
    TTF_Font *font;
    TTF_Font *recursive_font;

    SDL_Texture* home;
    SDL_Texture* desktop;
    SDL_Texture* recursiveview;
    SDL_Texture* filename_header;
    SDL_Texture* size_header;
    SDL_Texture* permissions_header;

} AppData;

// Vector of FileEntries containing the various objects whose data will be rendered
std::vector<FileEntry*> ExplorerEntries;
std::vector<std::string> RecursiveEntries;

std::string user;
std::string home;

void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);
void renderRecursiveView(SDL_Renderer* renderer, AppData* data_ptr, std::string dirname);
void buildRecursiveEntries(std::string dirname, int indent);
std::string getDirectoryEntries(std::string dirname, SDL_Renderer* renderer, TTF_Font* font);
std::string parseMouseClick(int mouse_click_x, int mouse_click_y);
std::string getFilePermissions(struct stat info);

int main(int argc, char **argv)
{
    // initializing SDL as Video
    SDL_Init(SDL_INIT_VIDEO);
    // initialize PNG IMG library
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    // create window and renderer
    SDL_Renderer *renderer;    // ONLY NEED TO DO THE FOLLOWING ONE TIME 
    // load the folder icon

    SDL_Window *window;
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    // initialize and perform rendering loop
    AppData data;
    initialize(renderer, &data);

    std::string current_dir;
    current_dir = getDirectoryEntries(home, renderer, data.font);

    render(renderer, &data);
    SDL_Event event;
    SDL_WaitEvent(&event);

    bool recursive_flag = false;
    while (event.type != SDL_QUIT) 
    {   
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int mouse_click_x = event.button.x;
            int mouse_click_y = event.button.y;

            std::string next_element = parseMouseClick(mouse_click_x, mouse_click_y);
            std::string next_element_type = next_element.substr(next_element.find(',') + 1, std::string::npos);
            int delimiter_loc = next_element.find(',');
            if(delimiter_loc != -1) { next_element.erase(delimiter_loc, std::string::npos); }

            if(next_element == "NULL") {
                // do nothing for this click
            } else if(next_element == "R") {
                // render recursive viewing mode
                recursive_flag = !recursive_flag;
                if(recursive_flag) { buildRecursiveEntries(current_dir, 0); }
            } else if(next_element_type == "" || next_element_type == "dir" || next_element_type == "HOME" || next_element_type == "DESKTOP") {
                ExplorerEntries.clear();
                current_dir = getDirectoryEntries(next_element, renderer, data.font);
            } else {
                int pid = fork();
                std::string open_file_cmd = "xdg-open " + next_element;

                if(pid == -1){
                    //fork error
                    std::cout << "Fork Error\n";
                } else if(pid > 0) {
                    //parent waits for child
                    int status;
                    waitpid(pid, &status, 0);
                } else if (pid == 0) {
                    system(open_file_cmd.c_str());
                    exit(1);
                }
            }
        }
        SDL_WaitEvent(&event);
        if(recursive_flag) {
            renderRecursiveView(renderer, &data, current_dir); 
            
        } else {
            RecursiveEntries.clear();
            render(renderer, &data); 
        } 
        
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
    user = getenv("USER");
    home = getenv("HOME");
    // set color of background when erasing frame
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    // set the font
    data_ptr->font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 20);
    data_ptr->recursive_font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 15);

    SDL_Surface *surf = IMG_Load("resrc/images/home_icon.png");
    data_ptr->home = SDL_CreateTextureFromSurface(renderer, surf);
    surf = IMG_Load("resrc/images/desktop_icon.png");
    data_ptr->desktop = SDL_CreateTextureFromSurface(renderer, surf);
    surf = IMG_Load("resrc/images/recursive_icon.png");
    data_ptr->recursiveview = SDL_CreateTextureFromSurface(renderer, surf);
    
    SDL_Color header_color = {0, 0, 0}; 
    surf = TTF_RenderText_Solid(data_ptr->font, "NAME", header_color);
    data_ptr->filename_header = SDL_CreateTextureFromSurface(renderer, surf);
  
    surf = TTF_RenderText_Solid(data_ptr->font, "SIZE", header_color);
    data_ptr->size_header = SDL_CreateTextureFromSurface(renderer, surf);

    surf = TTF_RenderText_Solid(data_ptr->font, "PERMISSIONS", header_color);
    data_ptr->permissions_header = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FreeSurface(surf);
}

// Uses AppData to render objects and phrases to the window
void render(SDL_Renderer* renderer, AppData* data_ptr)
{
    // reset render color to gray
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    // erase renderer content from the previous rendering
    SDL_RenderClear(renderer);

    //x = 85 
    SDL_Rect name_header = {135, 7};
    SDL_QueryTexture(data_ptr->filename_header, NULL, NULL, &(name_header.w), &(name_header.h));
    SDL_RenderCopy(renderer, data_ptr->filename_header, NULL, &name_header);

    SDL_Rect size_header = {475, 7};
    SDL_QueryTexture(data_ptr->size_header, NULL, NULL, &(size_header.w), &(size_header.h));
    SDL_RenderCopy(renderer, data_ptr->size_header, NULL, &size_header);

    SDL_Rect permissions_header = {620, 7};
    SDL_QueryTexture(data_ptr->permissions_header, NULL, NULL, &(permissions_header.w), &(permissions_header.h));
    SDL_RenderCopy(renderer, data_ptr->permissions_header, NULL, &permissions_header);
    
    // Draw the scroll bar across the right side of the window and color it purple
    SDL_Rect scroll_bar_container = {785, 0, 20, 600}; 
    SDL_SetRenderDrawColor(renderer, 81, 12, 118, 255);
    SDL_RenderFillRect(renderer, &scroll_bar_container);

    // Draw the upper window header across the top of the window and color it purple
    SDL_Rect scroll_bar = {785, 0, 20, 100}; 
    SDL_SetRenderDrawColor(renderer, 152, 153, 155, 255);
    SDL_RenderFillRect(renderer, &scroll_bar);

    SDL_Rect sidebar1 = {60, 0, 5, 600};
    SDL_SetRenderDrawColor(renderer, 81, 12, 118, 255);
    SDL_RenderFillRect(renderer, &sidebar1);

    SDL_Rect home = {8, 7, 40, 40};
    SDL_RenderCopy(renderer, data_ptr->home, NULL, &home);

    SDL_Rect desktop = {8, 67, 40, 40}; 
    SDL_RenderCopy(renderer, data_ptr->desktop, NULL, &desktop);

    SDL_Rect recursiveview = {8, 127, 40, 40};
    SDL_RenderCopy(renderer, data_ptr->recursiveview, NULL, &recursiveview);

    // Create a container for icons: {x, y, width, height}
    SDL_Rect icon_container = {75, 67, 35, 35};
    
    SDL_Rect name_container = {135, 67};
    SDL_Rect size_container = {475, 67};
    SDL_Rect perms_container = {620, 67};

    for(int i = 0; i < ExplorerEntries.size(); i++) {
        ExplorerEntries[i]->setCoordinates(icon_container.x, icon_container.x + icon_container.w, 
                                           icon_container.y, icon_container.y + icon_container.h, "icon");
        SDL_RenderCopy(renderer, ExplorerEntries[i]->data.icon, NULL, &icon_container);
        icon_container.y += 45;

        SDL_QueryTexture(ExplorerEntries[i]->data.name, NULL, NULL, &(name_container.w), &(name_container.h));
        ExplorerEntries[i]->setCoordinates(name_container.x, name_container.x + name_container.w, 
                                           name_container.y, name_container.y + name_container.h, "name");
        SDL_RenderCopy(renderer, ExplorerEntries[i]->data.name, NULL, &name_container);
        name_container.y += 45;

        if(ExplorerEntries[i]->entrytype != "dir") {
            SDL_QueryTexture(ExplorerEntries[i]->data.size, NULL, NULL, &(size_container.w), &(size_container.h));
            SDL_RenderCopy(renderer, ExplorerEntries[i]->data.size, NULL, &size_container);
        }
        size_container.y += 45;

        if(ExplorerEntries[i]->entrytype != "dir") {
            SDL_QueryTexture(ExplorerEntries[i]->data.permissions, NULL, NULL, &(perms_container.w), &(perms_container.h));
            SDL_RenderCopy(renderer, ExplorerEntries[i]->data.permissions, NULL, &perms_container);
        }
        perms_container.y += 45;
    }

    // show rendered frame
    SDL_RenderPresent(renderer);
}

void renderRecursiveView(SDL_Renderer* renderer, AppData* data_ptr, std::string dirname) {
    // reset render color to gray
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    // erase renderer content from the previous rendering
    SDL_RenderClear(renderer);

    SDL_Rect sidebar1 = {60, 0, 5, 600};
    SDL_SetRenderDrawColor(renderer, 81, 12, 118, 255);
    SDL_RenderFillRect(renderer, &sidebar1);

    SDL_Rect recursiveview = {8, 127, 40, 40};
    SDL_RenderCopy(renderer, data_ptr->recursiveview, NULL, &recursiveview);

    std::vector<SDL_Texture*> RecursiveTextures;
    
    // Transform the string vector into renderable textures, store them in RecursiveTextures
    SDL_Color name_color = {0, 0, 0}; 
    SDL_Surface *surf;

    for(int i = 0; i < RecursiveEntries.size(); i++) {
        surf = TTF_RenderText_Solid(data_ptr->recursive_font, RecursiveEntries[i].c_str(), name_color);
        RecursiveTextures.push_back(SDL_CreateTextureFromSurface(renderer, surf));
    }

    surf = TTF_RenderText_Solid(data_ptr->recursive_font, dirname.c_str(), name_color);
    SDL_Texture* dir_texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    SDL_Rect starting_row = {75, 10};
    SDL_QueryTexture(dir_texture, NULL, NULL, &(starting_row.w), &(starting_row.h));
    SDL_RenderCopy(renderer, dir_texture, NULL, &starting_row);
    starting_row.x += 25;
    starting_row.y += 25;

    for(int i = 0; i < RecursiveTextures.size(); i++) {
        SDL_QueryTexture(RecursiveTextures[i], NULL, NULL, &(starting_row.w), &(starting_row.h));
        SDL_RenderCopy(renderer, RecursiveTextures[i], NULL, &starting_row);
        starting_row.y += 25;
    }
    SDL_RenderPresent(renderer);
}

std::string getDirectoryEntries(std::string dirname, SDL_Renderer* renderer, TTF_Font* font)
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
            // Get the file name; ignore the "." directory by skipping the current iteration
            std::string curfile_name = files[i].c_str();
            if(curfile_name == ".") { continue; }
            // Get the file path and size
            std::string curfile_path = dirname + "/" + files[i].c_str();
            err = stat(curfile_path.c_str(), &file_info);
            int curfile_size = file_info.st_size;
            std::string curfile_perms = getFilePermissions(file_info);
            if(err == 1) { std::cout << "DEBUG MSG: something went wrong obtaining file info" << std::endl;}
            
            /************************************/
            /* THE CURRENT ENTRY IS A DIRECTORY */
            /************************************/
            if(S_ISDIR(file_info.st_mode)) {
                // Create an instance of FileEntry::Directory, constructing it with values parsed from the current file
                Directory* dir = new Directory(curfile_name, "dir", curfile_size, curfile_path, curfile_perms, renderer, font);
                dir->setIcon(renderer);
                // Add the FileEntry::Directory to the ExplorerEntries array 
                ExplorerEntries.push_back(dir);

            /***************************************/
            /* THE CURRENT ENTRY IS A REGULAR FILE */
            /***************************************/
            } else if(S_ISREG(file_info.st_mode)) {
                // Find the "." character in the current file name
                int pos = files[i].find(".", 1);
                std::string extension;
                // Make a substring of the current file's extension by making a substring from "." to the end of the string
                if(pos == -1) { extension = "noext"; } else { extension = files[i].substr(pos); }

                /* EXECUTABLE (the current user has execute permissions (what about groups and others?)) */
                if(file_info.st_mode & S_IXUSR) {
                    // Create an instance of FileEntry::Executable, constructing it with values parsed from the current file
                    Executable* exe = new Executable(curfile_name, "exe", curfile_size, curfile_path, curfile_perms, renderer, font);
                    exe->setIcon(renderer);
                    // Add the FileEntry::Executable to the ExplorerEntries array 
                    ExplorerEntries.push_back(exe);
                }

                /* IMAGE (file extension matches) */
                else if(extension == ".jpg" || extension == ".jpeg" || extension == ".png" ||
                        extension == ".tif" || extension == ".tiff" || extension == ".gif") {                
                    // Create an instance of FileEntry::Image, constructing it with values parsed from the current file
                    Image* img = new Image(curfile_name, "img", curfile_size, curfile_path, curfile_perms, renderer, font);
                    img->setIcon(renderer);
                    // Add the FileEntry::Image to the ExplorerEntries array 
                    ExplorerEntries.push_back(img);

                /* VIDEO (file extension matches) */
                } else if(extension == ".mp4" || extension == ".mov" || extension == ".mkv" ||
                          extension == ".avi" || extension == ".webm") {

                    // Create an instance of FileEntry::Video, constructing it with values parsed from the current file
                    Video* vid = new Video(curfile_name, "vid", curfile_size, curfile_path, curfile_perms, renderer, font);
                    vid->setIcon(renderer);
                    // Add the FileEntry::Video to the ExplorerEntries array 
                    ExplorerEntries.push_back(vid);

                /* CODE FILE (file extension matches) */
                } else if(extension == ".h" || extension == ".c"    || extension == ".cpp" ||
                          extension == ".py"|| extension == ".java" || extension == ".js") {

                    // Create an instance of FileEntry::CodeFile, constructing it with values parsed from the current file
                    CodeFile* code = new CodeFile(curfile_name, "code", curfile_size, curfile_path, curfile_perms, renderer, font);
                    code->setIcon(renderer);
                    // Add the FileEntry::CodeFile to the ExplorerEntries array 
                    ExplorerEntries.push_back(code);

                /* OTHER FILE (none of the above) */
                } else {

                    // Create an instance of FileEntry::OtherFile, constructing it with values parsed from the current file
                    OtherFile* other = new OtherFile(curfile_name, "other", curfile_size, curfile_path, curfile_perms, renderer, font);
                    other->setIcon(renderer);
                    // Add the FileEntry::OtherFile to the ExplorerEntries array 
                    ExplorerEntries.push_back(other);
                }

            /* THE CURRENT ENTRY IS NOT A DIRECTORY OR REGULAR FILE */
            } else { continue; }
        }
    }
    else
    {
        fprintf(stderr, "Error: directory argument passed into getDirectoryEntries '%s' not found\n", dirname.c_str());
    }
    return dirname;
}

void buildRecursiveEntries(std::string dirname, int indent) {

    std::string space = "          ";
	for(int i = 0; i < indent; i++) { space += "          "; }
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
    }
    
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
            RecursiveEntries.push_back(space + files[i]);
			if(files[i] != "." && files[i] != "..") {
				buildRecursiveEntries(full_path, indent + 1);
			}				
		} else {
            RecursiveEntries.push_back(space + files[i]);
			printf("%s%s \n", space.c_str(), files[i].c_str());
		}
    }
}

std::string parseMouseClick(int mouse_click_x, int mouse_click_y) {

    /**** CLICKED ON HOME ICON ****/
    if((mouse_click_x >= 8 && mouse_click_x <= 48) && (mouse_click_y >= 7 && mouse_click_y <= 47)) {
        // return to the home directory
        return home + ",HOME";
    }
    /**** CLICKED ON ***/
    else if((mouse_click_x >= 8 && mouse_click_x <= 48) && (mouse_click_y >= 67 && mouse_click_y <= 107))  {
        return home + "/Desktop/,DESKTOP";

    }
    /**** CLICKED ON RECURSIVE VIEW ****/
    else if((mouse_click_x >= 8 && mouse_click_x <= 48) && (mouse_click_y >= 127 && mouse_click_y <= 167))  {
        return "R";
        // render the current directory in recursive view
        // set recursive mode flag
    } else {
        for(int i = 0; i < ExplorerEntries.size(); i++) {
            if((mouse_click_x >= ExplorerEntries[i]->icon_coordinates[0] && mouse_click_x <= ExplorerEntries[i]->icon_coordinates[1]) &&
            (mouse_click_y >= ExplorerEntries[i]->icon_coordinates[2] && mouse_click_y <= ExplorerEntries[i]->icon_coordinates[3])) {
                    return ExplorerEntries[i]->filepath + "," + ExplorerEntries[i]->entrytype;
            }

            if((mouse_click_x >= ExplorerEntries[i]->name_coordinates[0] && mouse_click_x <= ExplorerEntries[i]->name_coordinates[1]) &&
            (mouse_click_y >= ExplorerEntries[i]->name_coordinates[2] && mouse_click_y <= ExplorerEntries[i]->name_coordinates[3])) {
                    return ExplorerEntries[i]->filepath + "," + ExplorerEntries[i]->entrytype;
            }
        }
    }
    return "NULL";
}

std::string getFilePermissions(struct stat info) {
    std::string permissions = "";
    if(info.st_mode & S_IRUSR) { permissions.append("r"); } else permissions.append("-");
    if(info.st_mode & S_IWUSR) { permissions.append("w"); } else permissions.append("-");
    if(info.st_mode & S_IXUSR) { permissions.append("x"); } else permissions.append("-");
    if(info.st_mode & S_IRGRP) { permissions.append("r"); } else permissions.append("-");
    if(info.st_mode & S_IWGRP) { permissions.append("w"); } else permissions.append("-");
    if(info.st_mode & S_IXGRP) { permissions.append("x"); } else permissions.append("-");
    if(info.st_mode & S_IROTH) { permissions.append("r"); } else permissions.append("-");
    if(info.st_mode & S_IWOTH) { permissions.append("w"); } else permissions.append("-");
    if(info.st_mode & S_IXOTH) { permissions.append("x"); } else permissions.append("-");

    return permissions;
}
