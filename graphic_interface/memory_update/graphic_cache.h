#ifndef CACHE_INTERFACE
#define CACHE_INTERFACE

#include "../interface/interface.h"
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

//time management
#define TICKS_ARRAY_LENGTH 2 //number of chronometers
//ticks for text displaying
#define CHANGE_TICKS 0
#define ERROR_TICKS 1

typedef struct
{
    //RNG will be handled by another system
    //but will be initialized with the cache

    //Frame rate management
    Uint32 last_time;
    Uint32 time_now;

    //text displaying
    text* txtMessage;
    char** availableTexts;

    //window
    SDL_Window* window;
    SDL_Renderer *draw;
    SDL_Event event;
    bool is_playing;

    //rafle tree management
    bool display_tree;
    //PathTree* cacheTree;

    //error handling
    int statut;

    //ticks and time measurement
    //j'ai choisit cette implementation pour aisement
    //pouvoir rajouter des ticks de mesure si besoin
    Uint32 ticksArray[TICKS_ARRAY_LENGTH];
} GraphicCache;

//graphic cache functions

//initializes the cache.
//In case of an error, interrupts the process
//and returns a cache with EXIT_FAILURE as a status
GraphicCache* initCache();

void freeCache(GraphicCache* cache);
//called every tick to display stuff on the screen
//depending on the state of the cache
//DOES NOT modify the cache
void display(Game* g, GraphicCache* cache);

//displays an alert on the screen
//and does other stuff depending of the chosen timer
void alert(GraphicCache* cache, int alertIndex, int timerIndex);
#endif //CACHE_INTERFACE