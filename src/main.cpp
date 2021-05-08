#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>

#define WIDTH 800
#define HEIGHT 600

// Structure containing all data needed for application
typedef struct AppData {
    TTF_Font *font;
    // use vectors here for multiple
    SDL_Texture *folder_icon;
    SDL_Texture *phrase;

} AppData;

void initialize(SDL_Renderer *renderer, AppData *data_ptr);
void render(SDL_Renderer *renderer, AppData *data_ptr);

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
    while (event.type != SDL_QUIT)
    {
        SDL_WaitEvent(&event);
        render(renderer, &data);
    }

    // clean up
    SDL_DestroyTexture(data.folder_icon);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    return 0;
}

void initialize(SDL_Renderer *renderer, AppData *data_ptr)
{
    // set color of background when erasing frame
    SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
    // load font
    data_ptr->font = TTF_OpenFont("resrc/OpenSans-Regular.ttf", 50);

    // ONLY NEED TO DO THE FOLLOWING ONE TIME 
    // load the folder icon


    // set phrase color to black
    SDL_Color phrase_color = {0, 0, 0}; 
    SDL_Surface *phrase_surf = TTF_RenderText_Solid(data_ptr->font, "Home", phrase_color);
    data_ptr->phrase = SDL_CreateTextureFromSurface(renderer, phrase_surf);
    SDL_FreeSurface(phrase_surf);
}

void render(SDL_Renderer *renderer, AppData *data_ptr)
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
    SDL_RenderCopy(renderer, data_ptr->folder_icon, NULL, &icon_container);

    // move the y position down and render another icon
    icon_container.y = 150;
    SDL_RenderCopy(renderer, data_ptr->folder_icon, NULL, &icon_container);

    SDL_Rect phrase_container = {50, 5, 75, 45};
    //SDL_QueryTexture(data_ptr->phrase, NULL, NULL, &(phrase_container.x), &(phrase_container.y));
    SDL_RenderCopy(renderer, data_ptr->phrase, NULL, &phrase_container);

    SDL_Rect sidebar = {175, 0, 5, 600};
    SDL_SetRenderDrawColor(renderer, 81, 12, 118, 255);
    SDL_RenderFillRect(renderer, &sidebar);


    // show rendered frame
    SDL_RenderPresent(renderer);
}



//void listDirectory(std::string dirname, int indent = 0);

/*
int main(int argc, char **argv)
{
    // if no command line paramater exists, list contents of current working directory
    // else, list contents of directory specified
    std::string dirname = ".";
    if (argc >= 2)
    {
        dirname = argv[1];
    }

    listDirectory(dirname);

    return 0;
}
*/

/*
void listDirectory(std::string dirname, int indent)
{
	std::string space = "";
	for(int i = 0; i < indent; i++) { space += " "; }

    struct stat info;
    int err = stat(dirname.c_str(), &info);
    if (err == 0 && S_ISDIR(info.st_mode)) {
	std::vector<std::string> files;
		
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
}
*/
