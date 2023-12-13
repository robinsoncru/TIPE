#include "update.h"
<<<<<<< HEAD

// called every frame.
// updates the memory, useful to manage stuff that must be done
// or displayed for a certain time
void update(Game *g, GraphicCache *cache)
{

    // increments all chronometers
    for (int i = 0; i < TICKS_ARRAY_LENGTH; i++)
    {
        if (cache->ticksArray[i] > 0)
        {
=======
#include <stdio.h>

//called every frame.
//updates the memory, useful to manage stuff that must be done
//or displayed for a certain time
void update(Game *g, GraphicCache *cache){

    //increments all chronometers
    for (int i = 0; i < TICKS_ARRAY_LENGTH; i++) {
        if (cache->ticksArray[i] > 0) {
>>>>>>> interface-struct
            cache->ticksArray[i]++;
        }
    }

<<<<<<< HEAD
    // creates a transition between the turns
    if (cache->ticksArray[CHANGE_TICKS] > MAX_TICKS)
    {
=======
    //creates a transition between the turns
    if (cache->ticksArray[CHANGE_TICKS] > MAX_TICKS) {
>>>>>>> interface-struct
        cache->ticksArray[CHANGE_TICKS] = 0;

        change_damier(g);
        cache->txtMessage->display = false;
        g->indCheck = IND_NORMAL;
    }

<<<<<<< HEAD
    // tells if the player has not selected a pawn and tries
    // to move no matter what
    if (cache->ticksArray[ERROR_TICKS] > MAX_TICKS)
    {
=======
    //tells if the player has not selected a pawn and tries
    //to move no matter what
    if (cache->ticksArray[ERROR_TICKS] > MAX_TICKS) {
>>>>>>> interface-struct
        cache->ticksArray[ERROR_TICKS] = 0;

        cache->txtMessage->display = false;
        g->indCheck = IND_NORMAL;
    }

<<<<<<< HEAD
    // Control pannel
    if (SDL_PollEvent(&(cache->event)) == 1)
    {
        switch (cache->event.type)
        {

        case SDL_QUIT:
            // Exit the game
            cache->is_playing = false;
            break;

        default:
            // called if any input is made
            onInput(g, cache, cache->event);
            break;
        }
    }
    else
    {
        // Autoplay take over
        if (cache->autoplay)
            // play_a_move(PAWNMOVERIGHT, random_index(g), g, cache);
            play_a_move(BIDEPL, random_index(g), g, cache);
    }
=======
    //Control pannel
    if (SDL_PollEvent(&(cache->event)) == 1) {
        switch (cache->event.type) {
        
            case SDL_QUIT:
                //Exit the game
                cache->is_playing = false;
            break;

            default:
                //called if any input is made
                onInput(g, cache, cache->event);
            break;

        }
    }
>>>>>>> interface-struct
}