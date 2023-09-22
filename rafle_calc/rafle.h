// Operators for Rafle structure

typedef struct Rafle Rafle;
Rafle *createRafle();
bool isEmpty(Rafle *rafle);
void addRafle(Rafle *rafle, int ind_eat);
void destroyRafle(Rafle *rafle);