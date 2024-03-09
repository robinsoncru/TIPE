#include "path_tree_display.h"
#include <stdio.h>

void getMiddlePoint(Case c, int* x, int* y){
    *x = c.rect.x + LG_CASE / 2;
    *y = c.rect.y + LG_CASE / 2;
}

void drawLineBetweenCases(SDL_Renderer* render, Case c0, Case c1){
    int x0, y0, x1, y1;
    getMiddlePoint(c0, &x0, &y0);
    getMiddlePoint(c1, &x1, &y1);
    drawLine(render, orange, x0, y0, x1, y1);
}

void drawLinesBetweenParentAndChilds(SDL_Renderer* render, Case **damier, PathTree* t){
    if (t != emptyTree) {
        PathTree* child;
        int di, dj;
        Case c0, c1;
        Coord depart = pathTreeLabel(t);
        Coord arrivee;
        c0 = damier[depart.i][depart.j];
        for (int k = 0; k < 4; k++) {
            getDirsFromCode(k, &di, &dj);
            child = pathTreeChild(t, dj, di);
            if (child != emptyTree) {
                arrivee = pathTreeLabel(child);
                c1 = damier[arrivee.i][arrivee.j];
                drawLineBetweenCases(render, c0, c1);
            }
        }
    }
}

void pathTreeDisplay(SDL_Renderer* render, Case **damier, PathTree* t){
    if (t != emptyTree) {
        drawLinesBetweenParentAndChilds(render, damier, t);
        int di, dj;
        PathTree* child;
        for (int k = 0; k < 4; k++) {
            getDirsFromCode(k, &di, &dj);
            child = pathTreeChild(t, dj, di);
            pathTreeDisplay(render, damier, child);
        }
    }
}