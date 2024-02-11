#include "play_back_game_functions.h"

void pawnMoveNGE(Game *g, bool is_white, int ind, bool left)
{
    moveBackGameManagement(g);
    pawn p = g->allPawns[is_white][ind];
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place_new(g, g->damier, ind, is_white, i + di, j + dj);
    if (p.friendly != NEUTRAL_IND)
    {
        g->ind_move_back = p.friendly;
        // le pion indique a partir de son indice
    }
}

void pawnMoveCancel(Game *g, bool is_white, int ind, bool left)
{
    pawn p = g->allPawns[is_white][ind];
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place_new(g, g->damier, ind, is_white, i - di, j - dj);
}

void recreateCloud(Game *g, cloud_chain *l, ind_pba_t *survivor, bool iw)
{

    while (!cis_empty(l))
    {
        tcloud data = cpop(l);
        int i = data.coord.i;
        int j = data.coord.j;
        createPawn(g, iw, i, j);
        int ind = g->damier[i][j].ind_pawn;
        put_pawn_value(g, iw, ind, PBA, data.pba);
        push(g->cloud[iw], ind);
        g->lengthCloud[iw]++;
    }

    put_pawn_value(g, iw, survivor->ind, PBA, survivor->pba);
    push(g->cloud[iw], survivor->ind);
    g->lengthCloud[iw]++;
}

int promotionNGE(Game *g, int ind)
{
    moveBackGameManagement(g);
    /* Promote the pawn at the ind in pmetre : do nothing, become a queen or become an ennemy pawn */
    // Return the index of the ennemy pawn created, -1 else
    bool iw = g->is_white;
    int choice = rand() % 3;
    if (choice == 1)
    {
        put_pawn_value(g, iw, ind, QUEEN, 1);
        printv("QUEEN");
        return VOID_INDEX;
    }
    else if (choice == 2)
    {
        printv("BLACK");
        int i = get_pawn_value(g, iw, ind, LIG);
        int j = get_pawn_value(g, iw, ind, COL);

        // Kill the former pawn
        killPawn(g, g->damier, i, j);

        // Give birth to the ennemy pawn
        createPawn(g, !iw, i, j);
        int indNew = g->damier[i][j].ind_pawn;
        if (canBePromoted(g, !iw, indNew))
            promote(g, !iw, indNew);

        return indNew;
    }
    printv("NOTHING");

    return VOID_INDEX;
}

void cancelPromotion(Game *g, int ind_old_friend, int ind_new_foe)
{
    bool iw = g->is_white;
    if (ind_new_foe != -1)
    {
        int i = get_pawn_value(g, iw, ind_new_foe, LIG);
        int j = get_pawn_value(g, iw, ind_new_foe, COL);
        killPawn(g, g->damier, i, j);
        createPawn(g, iw, i, j);
    }
    else
        put_pawn_value(g, iw, ind_old_friend, QUEEN, 0);
}

void pawnMoveBackNGE(Game *g, bool is_white, int ind, bool left)
{
    pawn p = g->allPawns[is_white][ind];
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place_new(g, g->damier, ind, is_white, i - di, j - dj);
}

ind_bool_t biDeplNGE(Game *g, bool color, int ind)
{
    moveBackGameManagement(g);
    // On suppose le coup legal
    bool depl = int_to_bool(rand() % 2);
    // Depl le pion a droite ou a gauche et creera l'autre ghost pawn de l'autre cote
    int dj = depl ? 1 : -1;
    int di = color ? 1 : -1;
    // Creer un pion a droite ou a gauche aleatoirement
    int newLig = get_pawn_value(g, color, ind, LIG) + di;
    int newCol = get_pawn_value(g, color, ind, COL) + dj;
    createPawn(g, color, newLig, newCol);
    int newInd = g->damier[newLig][newCol].ind_pawn;
    put_pawn_value(g, color, newInd, PBA, get_pawn_value(g, color, ind, PBA) * 2);

    // Rajoute dans le cloud
    if (!isInCloud(g, color, ind))
        push(g->cloud[color], ind);
    push(g->cloud[color], newInd);

    g->lengthCloud[color]++;

    // Deplace le pion de l'autre cote
    put_pawn_value(g, color, ind, PBA, get_pawn_value(g, color, ind, PBA) * 2);
    simplyPawnMove(g, color, ind, depl);
    ind_bool_t data = {.ind = newInd, .b = depl};
    return data;
}

void cancelBidepl(Game *g, int oldInd, ind_bool_t data)
{
    bool iw = g->is_white;
    bool left = data.b;
    int newInd = data.ind;
    pop(g->cloud[iw]); // Supress the last pawn pushed in the cloud, which is indNew
    g->lengthCloud[iw]--;
    killPawnByInd(g, iw, newInd);

    // Move back
    pawnMoveCancel(g, iw, oldInd, left);

    int pba = get_pawn_value(g, iw, oldInd, PBA);
    put_pawn_value(g, iw, oldInd, PBA, pba / 2);
    if (!isInCloud(g, iw, oldInd))
    {
        // If the pawn exit the cloud
        pop(g->cloud[iw]);
        g->lengthCloud[iw]--;
    }
}

primary_data_t queenDeplNGE(Game *g, int ind, bool color, queen_move_t tuple_coord)
{
    moveBackGameManagement(g);
    // Suppose que l'entree est valide
    // On a deja la position d'arrivee et le potentiel pion a manger en entree
    int lig = tuple_coord.pos_dame.i;
    int col = tuple_coord.pos_dame.j;
    int enn_lig = tuple_coord.pos_eaten_pawn.i;
    int enn_col = tuple_coord.pos_eaten_pawn.j;
    change_pawn_place_new(g, g->damier, ind, color, lig, col);
    int ennInd = VOID_INDEX;
    primary_data_t data = {.friend = -1, .foe = -1, .queen = false};
    if (enn_lig != -1 && enn_col != -1)
    {
        ennInd = g->damier[enn_lig][enn_col].ind_pawn;
        if (ennInd != VOID_INDEX)
        {
            data.friend = get_pawn_value(g, !color, ennInd, FRIENDLY);
            data.foe = get_pawn_value(g, !color, ennInd, ENNEMY);
            data.queen = int_to_bool(get_pawn_value(g, !color, ennInd, QUEEN));
            killPawn(g, g->damier, enn_lig, enn_col);
        }
    }

    if (get_pawn_value(g, color, ind, FRIENDLY) != NEUTRAL_IND)
    {
        g->ind_move_back = get_pawn_value(g, color, ind, FRIENDLY);
        // le pion indique a partir de son indice
    }
    return data;
}

void cancelDeplQueen(Game *g, int ind_queen, queen_move_t tuple_coord, primary_data_t data, Coord init_coord)
{

    bool iw = g->is_white;
    int ennInd = VOID_INDEX;
    int enn_lig = tuple_coord.pos_eaten_pawn.i;
    int enn_col = tuple_coord.pos_eaten_pawn.j;

    if (!outOfBounds(enn_lig, enn_col))
    {
        ennInd = g->damier[enn_lig][enn_col].ind_pawn;
        if (ennInd != VOID_INDEX)
        {
            createPawn(g, !iw, enn_lig, enn_col);
            put_pawn_value(g, !iw, ennInd, FRIENDLY, data.friend);
            put_pawn_value(g, !iw, ennInd, ENNEMY, data.foe);
            put_pawn_value(g, !iw, ennInd, QUEEN, bool_to_int(data.queen));
        }
    }

    change_pawn_place_new(g, g->damier, ind_queen, iw, init_coord.i, init_coord.j);
}

// void moveBack(Game *g, Kmaillon *l); // ?

// void promotionBack(Game *g, Kmaillon *l);

// void lienAmitieBack(int lig, int col, Game *g, Kmaillon *l);

// void lienEnnemitieBack(int lig, int col, Game *g, Kmaillon *l);

// void biDeplBack(Game *g, int ind, bool color, Kmaillon *l);

// void queenDeplBack(Game *g, int ind, bool color, queen_move_t tuple_coord, Kmaillon *l);

/*







   Play functions






*/

// Elles ont toutes des effets de bord
// et on suppose que les coups joues sont legaux

// moves the indicated pawn frontward depending on the
// indicated direction.
// On suppose que ce coup est legal.
// Met fin au tour pour la structure de jeu

// void pawnMove(Game *g, bool is_white, int ind, bool left)
// {
//     startTurnGameManagement(g);
//     pawn *p = &(g->allPawns[is_white][ind]);
//     int i = p->lig;
//     int j = p->col;
//     int di = is_white ? 1 : -1;
//     int dj = left ? -1 : 1;

//     change_pawn_place_new(g, g->damier, ind, is_white, i + di, j + dj);
//     if (p->friendly != NEUTRAL_IND)
//     {
//         g->ind_move_back = p->friendly;
//         // le pion indique a partir de son indice
//     }
//     endTurnGameManagement(g, is_white, ind, IND_CHANGE_ALLOWED, true);
// }

// /*

//    QUEEN FUNCTION

// */

// /*

//    Promotion functions

// */

// void promotionPmetre(pawn *pawns, bool is_white, Case **damier, int ind, Game *g)
// {
//     startTurnGameManagement(g);
//     /* Promote the pawn at the ind in pmetre : do nothing, become a queen or become an ennemy pawn */
//     int choice = rand() % 3;
//     if (choice == 1)
//     {
//         pawns[ind].queen = true;
//         endTurnGameManagement(g, is_white, ind, IND_GLORY_QUEEN, false);
//         return;
//     }
//     else if (choice == 2)
//     {
//         int i = pawns[ind].lig;
//         int j = pawns[ind].col;
//         // printf("Quantic %d %d", i, j);

//         // Kill the former pawn
//         killPawn(g, damier, i, j);

//         // Give birth to the ennemy pawn
//         createPawn(g, !is_white, i, j);
//         int indNew = damier[i][j].ind_pawn;
//         if (canBePromoted(g, !is_white, indNew))
//             promote(g, !is_white, indNew);

//         endTurnGameManagement(g, is_white, ind, IND_BAD_CHOICE, false);
//         return;
//     }
//     endTurnGameManagement(g, is_white, ind, IND_NOTHING_HAPPENED, false);
// }

// void promotion(Game *g)
// {
//     promotionPmetre(g->allPawns[g->is_white], g->is_white, g->damier, g->ind_move, g);
// }

// /*

//    LienAmitie functions

// */

// void lienAmitiePmetre(int lig, int col, Case **damier, int ind, bool is_white, Game *g)
// {
//     /* Lie d'amitie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
//     couleur opposé et qu'il existe. Gère le pmetre Pawn.friendly. Si on lie d'amitié un pion qui était déjà ami, l'action n'a pas lieu et
//     le joueur doit rejouer.
//     Suppose le pion ainsi que le pion ami selectionne valides */
//     startTurnGameManagement(g);
//     Case c = damier[lig][col];
//     put_pawn_value(g, is_white, ind, 3, c.ind_pawn);
//     put_pawn_value(g, c.pawn_color, c.ind_pawn, 3, ind);
//     endTurnGameManagement(g, is_white, ind, IND_CHANGE_ALLOWED, false);
// }

// void lienAmitie(int lig, int col, Game *g)
// {
//     // On suppose le coup legal
//     bool iw = g->is_white;
//     lienAmitiePmetre(lig, col, g->damier, g->ind_move, iw, g);
// }

// /*

//    MOVE BACK FUNCTION

// */

// // Functions to move back a pawn because the friend has just moved

// void moveBack(Game *g)
// {
//     /* Suppose move on the just pawn. Move back the pawn referred by ind_move_back to the case localised by the coord coordForMoveBack */
//     int ind = g->ind_move_back;
//     bool iw = g->is_white;
//     change_pawn_place(g->allPawns[iw], g->damier, ind, g->coordForMoveBack.i, g->coordForMoveBack.j);
//     moveBackGameManagement(g);
// }

// /*

//    LienEnnemitie functions

// */

// void lienEnnemitiePmetre(bool is_white, int lig, int col, Case **damier, int ind, Game *g)
// {
//     /* Declare ennemis pour la vie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
//     couleur opposé et qu'il existe. Gère le pmetre Game.friendly. Si on declare ennemi un pion qui était déjà ennemi, le coup n'est pas joué
//     Suppose legal move */
//     startTurnGameManagement(g);
//     Case c = damier[lig][col];
//     put_pawn_value(g, is_white, ind, 2, c.ind_pawn);
//     put_pawn_value(g, c.pawn_color, c.ind_pawn, 2, ind);
//     // printf("Enn %d %d\n", lig, col);
//     // print_pawn(g->allPawns[c.pawn_color][c.ind_pawn], c.ind_pawn);
//     // printf("Actu pawn");
//     // print_pawn(g->allPawns[is_white][ind], ind);
//     // flush();
//     endTurnGameManagement(g, is_white, ind, IND_CHANGE_ALLOWED, false);
// }

// void lienEnnemitie(int lig, int col, Game *g)
// {
//     // Suppose legal move
//     bool iw = g->is_white;
//     lienEnnemitiePmetre(iw, lig, col, g->damier, g->ind_move, g);
// }

// /*

//    Deplacement quantique : bi-deplacement

// */

// void biDepl(Game *g, int ind, bool color)
// {
//     // On suppose le coup legal
//     bool depl = int_to_bool(rand() % 2);
//     // Depl le pion a droite ou a gauche et creera l'autre ghost pawn de l'autre cote
//     int dj = depl ? 1 : -1;

//     startTurnGameManagement(g);
//     int di = color ? 1 : -1;
//     // Creer un pion a droite ou a gauche aleatoirement
//     int newLig = get_pawn_value(g, color, ind, LIG) + di;
//     int newCol = get_pawn_value(g, color, ind, COL) + dj;
//     createPawn(g, color, newLig, newCol);
//     int newInd = g->damier[newLig][newCol].ind_pawn;
//     put_pawn_value(g, color, newInd, PBA, get_pawn_value(g, color, ind, PBA) * 2);

//     // Rajoute dans le cloud
//     if (!isInCloud(g, color, ind))
//         push(g->cloud[color], ind);
//     push(g->cloud[color], newInd);

//     g->lengthCloud[color]++;

//     // Deplace le pion de l'autre cote
//     put_pawn_value(g, color, ind, PBA, get_pawn_value(g, color, ind, PBA) * 2);
//     simplyPawnMove(g, color, ind, depl);

//     // Maybe the clone pawn is near a pawn of the opposite color
//     if (canStormBreaks(g, newInd, color))
//         AleatStormBreaks(g, color);
//     else if (canStormBreaksForTheOthers(g, newInd, color))
//         AleatStormBreaks(g, !color);

//     endTurnGameManagement(g, color, ind, IND_CHANGE_ALLOWED, false);
// }
