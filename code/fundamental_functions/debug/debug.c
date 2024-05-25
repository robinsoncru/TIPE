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