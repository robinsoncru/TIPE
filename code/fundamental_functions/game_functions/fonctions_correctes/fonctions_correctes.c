#include "fonctions_correctes.h"

// handle structure

void put_pawn_value(Game *g, bool color, int ind, int wich_pmetre_modify, int value)
{
    /* 1:alive | 2:ennemy | 3:friendly | 4:queen | 5:lig | 6:col | 7:pba | 8:color
    Permet de modifier les pmetres d'un pion de la liste de g reperer avec sa couleur, wich_pmetre_modify modifie le i-ième pmetre */
    assert(isValidIndex(ind));
    switch (wich_pmetre_modify)
    {
    case ALIVE:
        g->allPawns[color][ind].alive = int_to_bool(value);
        break;
    case ENNEMY:
        g->allPawns[color][ind].ennemy = value;
        break;
    case FRIENDLY:
        g->allPawns[color][ind].friendly = int_to_bool(value);
        break;
    case QUEEN:
        g->allPawns[color][ind].queen = int_to_bool(value);
        break;
    case LIG:
        g->allPawns[color][ind].lig = value;
        break;
    case COL:
        g->allPawns[color][ind].col = value;
        break;
    case PBA:
        g->allPawns[color][ind].pba = value;
        break;
    case COLOR:
        g->allPawns[color][ind].color = int_to_bool(value);
        break;
    default:
        printv("put pawn value error");
        assert(false);
        break;
    }
}

int get_pawn_value(Game *g, bool color, int ind, int wich_pmetre_get)
{
    assert(isValidIndexInGame(g, ind, color));
    pawn *p = &(g->allPawns[color][ind]);

    /* 1 : alive |2 : ennemy |3 : friendly |4 : queen |5 : lig |6 : col |7 : pba */
    switch (wich_pmetre_get)
    {
    case ALIVE:
        return bool_to_int(p->alive);
    case ENNEMY:
        return p->ennemy;
    case FRIENDLY:
        return p->friendly;
    case QUEEN:
        return bool_to_int(p->queen);
    case LIG:
        return p->lig;
    case COL:
        return p->col;
    case PBA:
        return p->pba;
    case COLOR:
        return p->color;
    default:
        printv("get pawn value error");
        assert(false);
        return VOID_INDEX;
    }
}

pawn get_pawn(Game *g, bool color, int ind)
{
    assert(isValidIndexInGame(g, ind, color));
    return g->allPawns[color][ind];
}

Case get_case_damier(Game *g, int i, int j)
{
    assert(inGame(i, j));
    return g->damier[i][j];
}

void put_case_damier(Game *g, int i, int j, int whichPmetre, int valeur)
{
    assert(inGame(i, j));
    // A appeler pour modifier les champs color, pawn_color, ind_pawn de la case
    switch (whichPmetre)
    {
    case CASE_COLOR:
        g->damier[i][j].color = int_to_bool(valeur);
        break;
    case PAWN_COLOR:
        g->damier[i][j].pawn_color = int_to_bool(valeur);
        break;
    case IND_PAWN_ON_CASE:
        g->damier[i][j].ind_pawn = valeur;
        break;
    default:
        printv("put_case_damier error");
        assert(false);
        break;
    }
}

bool getFriendByInd(Game *g, int indActu, int indFriend, bool colorActu)
{
    assertAndLog(isValidIndexInGame(g, indActu, colorActu), "getFriendByInd indActu pas valide");
    assertAndLog(isValidIndexInGame(g, indFriend, !colorActu), "getFriendByInd indFriend pas valide");

    if (colorActu)
    {
        return g->liensAmitie[indFriend][indActu];
    }
    {
        return g->liensAmitie[indActu][indFriend];
    }
}

bool has_friend(Game *g, int ind, int color)
{
    for (int i = 0; i < MAX_NB_PAWNS; i++)
    {
        if (isValidIndexInGame(g, i, !color) && getFriendByInd(g, ind, i, color))
        {
            return true;
        }
    }
    return false;
}

void putFriendByInd(Game *g, int indActu, int indFriend, bool colorActu, bool valeur)
{
    assertAndLog(isValidIndexInGame(g, indActu, colorActu), "putFriendByInd indActu pas valide");
    assertAndLog(isValidIndexInGame(g, indFriend, !colorActu), "putFriendByInd indFriend pas valide");

    if (colorActu)
    {
        g->liensAmitie[indFriend][indActu] = valeur;
    }
    {
        g->liensAmitie[indActu][indFriend] = valeur;
    }

    if (valeur)
    {
        if (!int_to_bool(get_pawn_value(g, colorActu, indActu, FRIENDLY)))
        {
            put_pawn_value(g, colorActu, indActu, FRIENDLY, 1);
        }
        if (!int_to_bool(get_pawn_value(g, !colorActu, indFriend, FRIENDLY)))
        {
            put_pawn_value(g, !colorActu, indFriend, FRIENDLY, 1);
        }
    }
    else
    {
        if (!has_friend(g, indFriend, !colorActu))
        {
            put_pawn_value(g, !colorActu, indFriend, FRIENDLY, 0);
        }
    }
}

int *friendTabToListChaine(Game *g, int ind, bool color)
{
    int *l = create_list(MAX_NB_PAWNS);
    prout
        push(l, i);
}
return l;
}