/* SIMION George Constantin - 313CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

/* Cerinta 1 */
typedef struct celarc { 
    int nr_trons;
    float *uzura;
    char vecin[50];
    int directie;
    struct celarc *urm;
} TCelArc, *AArc;

typedef struct {
    float max_uzura;
    char oras_curent[50];
    AArc x;
} TGL;

void task1();
TGL *InitGraph(int R);
void freeGraf(TGL *graf, int rute);
void freeOrase(char ***orase, int rute);
void addMuchie(TGL *graf, int R, char *src, char *dest, int nr_tronsoane, float *uzura);
void calculate_max_uzura(TGL *graf, int R);
void updateZero(TGL *graf, int R);
void updateUzura(TGL *graf, int rute);
void printGraph(FILE *fout, TGL *graf, int R, float L, char ***matrice);
void updateOneHundred(TGL *graf, int R);

/* Cerinta 2 */
typedef struct vecin {
    int cost;
    int destIndex;
    struct vecin *urm;
} TCelVec, *TVecin;

typedef struct oras {
    char curr[50];
    int minCost;
    int index;
    TVecin vecini;
} TOras;

typedef struct drum {
    char src[50];
    char dest[50];
    int cost;
    int active;
} TDrum;

void task2();
TOras *InitOras(int M);
void freeHarta(TOras *harta, int M);
int findOras(TOras *harta, int M, char *oras);
void addDrum(TOras *harta, int *currentIndex, int M, char *src, char *dest, int cost);
int* dijkstra(TOras *harta, int M, char *start, TDrum *drumuri, int *nrMuchi);
int countActiveDrumuri(TDrum *drumuri, int M_exist);
void printRemainingDrumuri(FILE *fout, TDrum *drumuri, int M_exist, TOras *harta, int M);
void removeInactiveEdges(TOras *harta, int M, TDrum *drumuri);