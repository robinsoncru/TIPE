
#include "../Source/source_header.h"

bool freeCase(Case c);

bool isPawnValid(Game *g);
bool isValidIndexInGame(Game *g, int ind, bool color);
bool isValidIndex(int ind);
bool inGame(int lig, int col);
bool outOfBounds(int i, int j);
bool eatingIsOutOfBounds(int i, int j, int add0, int add1);
bool isInCloud(Game *g, bool color, int ind);