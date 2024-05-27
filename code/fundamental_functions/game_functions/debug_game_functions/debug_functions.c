#include "debug_functions.h"

/*







   Debug functions






*/

void print_pawn(pawn p, int ind)
{
    printf("Ind %d Coord %d %d Ennemy %d, Friend %d Color %d Alive %d Pba %d\n", ind, p.lig, p.col, p.ennemy, p.friendly, p.color, p.alive, p.pba);
    fflush(stdout);
}

// prints all the pawns of the same color
void print_pawns(Game *g, bool color)
{
    for (int i = 0; i < 2 * NB_PAWNS; i++)
    {
        if (g->allPawns[color][i].alive)
            print_pawn(g->allPawns[color][i], i);
    }
}

void print_damier(Case **damier, Game *g)
{
    int ind;
    bool c;
    for (int i = 0; i < NB_CASE_LG; i++)
    {
        for (int j = 0; j < NB_CASE_LG; j++)
        {
            ind = damier[i][j].ind_pawn;
            c = damier[i][j].pawn_color;
            pawn p = g->allPawns[c][ind];
            if (ind != -1)
            {
                printf("Case lig %d col %d Ind Pion %d color %d In cloud %d is queen %d ind friend %d ind foe %d\n \n",
                       p.lig, p.col, ind, c, 1 != p.pba,
                       p.queen, p.friendly, p.ennemy);
                // print_pawn(g->allPawns[c][ind], ind);
                // printf("---\n");
            }
        }
    }
}

void print_little_linked_list(int_chain *l)
{
    for (int i = 0; i < l->ind_actu; i++)
    {

        printf("indice %d\n", l->tableau[i]);
    }
}

void printv(char *s)
{
    // Affiche direct dans le terminal
    printf("%s\n", s);
    flush();
}

void picture_this(Game *g)
{
    print_damier(g->damier, g);
    error();
    error();
}

void print_state_game(Game *g, int which_pmetre)
{

    printf("\n     ");
    for (int i = 0; i < NB_CASE_LG; i++)
    {
        printf("%d     ", i);
    }
    printf("\n");
    for (int i = NB_CASE_LG-1; i >-1; i--)
    {
        printf("%d |", i);
        for (int j = 0; j < NB_CASE_LG; j++)
        {
            Case c = g->damier[i][j];
            if (freeCase(c))
            {
                printf("     |");
            }
            else {
                pawn p = g->allPawns[c.pawn_color][c.ind_pawn];
                int valeur;
                switch (which_pmetre)
                {
                case FRIENDLY:
                    valeur = p.friendly;
                    break;

                case ENNEMY:
                    valeur = p.ennemy;
                    break;

                case QUEEN:
                    valeur = p.queen;
                    break;

                case PBA:
                    valeur = p.pba;
                    break;
                
                default:
                    assertAndLog(false, "state_game: which pmetre non reconnu");
                    break;
                }

                printf("%d-%d-%d|", c.pawn_color, c.ind_pawn, valeur);
            }
        }
        printf("\n");
    }
    flush();
}

void picture_game(Game *g, bool iw)
{
    printf("nb pawns %d\n nb queen with friend %d\n nb queen without friend %d\n nb ghost pawn %d\n \
    nb friend no queen %d\n nb foe %d\n \n",
           g->nb_pawns[iw], g->nbQueenWithFriend[iw],
           g->nbQueenWithoutFriend[iw], g->lengthCloud[iw], g->nbFriendNoQueen[iw], g->nbFoe[iw]);
}