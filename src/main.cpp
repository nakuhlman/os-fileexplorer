#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define WIDTH 800
#define HEIGHT 600

// Structure containing all data needed for application
typedef struct AppData {
    TTF_Font *font;
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
    // surface is the intermediary
    SDL_Surface *surf = IMG_Load("resrc/images/folder_icon&48.png");
    // create a texture from the surface, then delete the surface (no longer needed)
    data_ptr->folder_icon = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

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

