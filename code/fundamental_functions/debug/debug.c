#include "debug.h"

void error()
{
    printf("check");
    fflush(stdout);
}

void assertAndLog(bool condition, char *message)
{
    if (!condition)
    {
        printf("\n");
        printf("Erreur : %s", message);
        printf("\n");
        assert(false);
    }
}

bool teste_frame(Game *g, bool color, int rec)
{
    if (rec > 0)
    {
        return teste_frame(g, color, rec - 1);
    }
    else
        return true;
}

void flush()
{
    fflush(stdout);
}

void print_liensAmitie(Game *g) {
    printf("\n  ");
    for (int j=0; j<MAX_NB_PAWNS; j++) {

        if (j < 10) {
            printf(" ");
        }
        printf("%d ", j);
    }
    printf("\n");
    for (int i=0; i<MAX_NB_PAWNS; i++) {
        printf("%d ", i);
        if (i < 10) {
            printf(" ");
        }
        for (int j=0; j<MAX_NB_PAWNS; j++) {
            printf("%d  ", g->liensAmitie[i][j]);
        }
        printf("\n");
    }
    flush();
}

void print_int_chain(int_chain *l) {
    assert(l->ind_actu < l->size_max);
    for (int i =0; i<l->ind_actu+1; i++) {
        printf("-> %d", l->tableau[i]);
    }
    flush();
}

void print_isssue(issue_t *t, int lgth) {
    for (int i=0; i<lgth; i++) {
        printf("Ind %d : (pba %d) (i %d) (j %d)", i, t[i].pba, t[i].pos_survivor.i, t[i].pos_survivor.j);
    }
    flush();
}