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

void flush()
{
    fflush(stdout);
}