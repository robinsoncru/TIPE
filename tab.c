#include <stdbool.h>
#include <stdio.h>

typedef struct {
    int x;
} one;

typedef struct {
    one o[2];
} two;

int main(int argc, char *argv[]) {
    int a[] = {5, 6};
    bool ind=true;
    
    two two1;
    (two1.o)[0].x = 1;
}