#include "orase.h"

int main(int argc, char *argv[]) {

    if (strcmp(argv[argc-1], "1") == 0) task1();
    else task2();

    return 0;
}