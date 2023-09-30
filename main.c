#include "fundamental_functions/game_functions_draughts.h"
#include "fundamental_functions/interface_jeu_dames.h"

// Run the game

int main(int argc, char *argv[])
{

    // Init game

    Case damier[NB_CASE_LG][NB_CASE_LG];
    int ind_move = NEUTRAL_IND;
    bool is_white = true;
    pawn allPawns[2][NB_PAWNS];
    int allMoves[4][2] = {{LEFT_FORWARD, LEFT_BACK}, {LEFT_BACK, LEFT_FORWARD}, {RIGHT_FORWARD, RIGHT_BACK}, {RIGHT_BACK, RIGHT_FORWARD}};

    init_damier(damier);

    init_pawns(allPawns[1], damier, true);
    init_pawns(allPawns[0], damier, false);
    // Index 1 is for white pawns
    // Index 0 is for black pawns

    // print_pawns(whites);
    // print_pawns(blacks);
    // print_damier(damier);
    // printf("Lg case %d\n", LG_CASE);

    // Init text
    text *txtMessage = malloc(sizeof(text));
    txtMessage->font = NULL;
    txtMessage->surface = NULL;
    txtMessage->texture = NULL;
    txtMessage->rect = malloc(sizeof(SDL_Rect));
    txtMessage->color = red;

    // Init window

    int statut = EXIT_FAILURE;
    SDL_Window *window = NULL;
    SDL_Renderer *draw = NULL;
    SDL_Event event;

    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    if (0 != TTF_Init())
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        goto Quit;
    }

    if (0 != SDL_CreateWindowAndRenderer(LG_WINDOW, LG_WINDOW, SDL_WINDOW_RESIZABLE, &window, &draw))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        goto Quit;
    }

    txtMessage->font = TTF_OpenFont("arial.ttf", 48);

    if (txtMessage->font == NULL)
    {
        printf("Font pb");
        goto Quit;
    }

    bool is_playing = true;
    Uint32 last_time = SDL_GetTicks();
    Uint32 time_now;
    Uint32 change_ticks = 0;
    Uint32 error_ticks = 0;

    // Start the game

    while (is_playing)
    {

        time_now = SDL_GetTicks();

        if (time_now - last_time > FRAME)
        {
            last_time = time_now;

            SDL_RenderClear(draw); // Clear the window

            // Drawing part

            // For each turn, need to change the screen's display
            if (change_ticks > MAX_TICKS)
            {
                is_white = !is_white;
                change_damier(damier, is_white);
                change_ticks = 0;
            }

            if (error_ticks > MAX_TICKS)
                error_ticks = 0;

            // Draw the board in the screen
            display_damier(draw, damier, allPawns);

            // Create a transition effect
            if (change_ticks > 0)
            {
                change_ticks++;
                // display the text
                SDL_RenderCopy(draw, txtMessage->texture, NULL, txtMessage->rect);
            }

            if (error_ticks > 0)
            {
                SDL_RenderCopy(draw, txtMessage->texture, NULL, txtMessage->rect);
                error_ticks++;
            }

            /* Rafraichit l'écran */
            SDL_RenderPresent(draw);

            // Control pannel

            if (SDL_PollEvent(&event) == 1)
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    is_playing = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (ind_move == NEUTRAL_IND)
                        ind_move = selectPawn(damier, event.button.x, event.button.y, is_white);
                    else if (ind_move > -1 && allPawns[is_white][ind_move].queen)
                        ind_move = queen_move(event.button.x, event.button.y, is_white, allPawns[is_white], damier, ind_move);
                    if (ind_move == NEUTRAL_IND)
                        printf("No pawn selected");
                    // printf("ind_move %d", ind_move);
                    break;

                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_LEFT)
                        ind_move = pawn_move(allPawns[is_white], damier, ind_move, true);
                    else if (event.key.keysym.sym == SDLK_RIGHT)
                        ind_move = pawn_move(allPawns[is_white], damier, ind_move, false);
                    else if (event.key.keysym.sym == SDLK_UP)
                        /*D'accord, je vois : si j'appuye sur haut, ça va manger le premier pion disponible selon l'ordre
                        horaire ou anti-horaire. Je suppose que c'est plus simple à coder mais j'admets etre dubitatif quand
                        au fait que les regles forcent un joueur a manger un pion en particulier. Ne serait-il pas plus judicieux de
                        laisser au joueur dont c'est le trait de choisir le pion qu'il mange ?
                        Quoique, il est vrai que la regle des rafles impose de choisir la meilleure, je suppose ce changement provisoire.*/
                        ind_move = eatPawn(allPawns[is_white], allPawns[!is_white], damier, ind_move);
                    else if (event.key.keysym.sym == SDLK_ESCAPE)
                        is_playing = false;
                    else if (event.key.keysym.sym == SDLK_r) {
                        printBestRafle(allPawns[is_white], allPawns[!is_white], damier, ind_move);
                        ind_move = NEUTRAL_IND;
                    }
                    else
                        ind_move = NEUTRAL_IND;
                    break;
                }
            }

            // Check if the move is allowed

            if (ind_move == IND_CHANGE_ALLOWED)
            {
                ind_move = NEUTRAL_IND;
                change_ticks++;
                prepareText(draw, txtMessage, "pawn moved");
                if (becomeDame(allPawns[is_white][ind_move]))
                    allPawns[is_white][ind_move].queen = true;
            }
            else if (ind_move == IND_PB)
            {
                // printf("No pawn moved");
                prepareText(draw, txtMessage, "NO pawn moved");
                ind_move = NEUTRAL_IND;
                error_ticks++;
            }
        }
        // is_playing=false;
    }

    statut = EXIT_SUCCESS;
    // printf("change \n");
    // print_damier(damier);

Quit:
    // Free the resources
    if (txtMessage->texture != NULL)
        SDL_DestroyTexture(txtMessage->texture);
    if (txtMessage->surface != NULL)
        SDL_FreeSurface(txtMessage->surface);
    if (txtMessage->font != NULL)
        TTF_CloseFont(txtMessage->font);
    if (NULL != draw)
        SDL_DestroyRenderer(draw);
    if (NULL != window)
        SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return statut;
}