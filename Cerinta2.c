/* SIMION George Constantin - 313CB */

#include "orase.h"

TOras *InitOras(int M)
/* Inițializam un array de structuri TOras */
{
    TOras *oras = (TOras*)malloc(M * sizeof(TOras));
    for (int i = 0; i < M; i++) {
        oras[i].curr[0] = '\0';
        oras[i].minCost = 100000;
        oras[i].vecini = NULL;
        oras[i].index = -1;                                     /* Initializam fiecare oraș din array */
    }
    return oras;
}

void freeHarta(TOras *harta, int M)
/* Eliberam memoria alocată pentru harta și vecinii fiecărui oraș */
{
    for (int i = 0; i < M; i++) {
        TVecin current = harta[i].vecini;
        while (current != NULL) {                               /* Itereaza prin fiecare oras si elibereaza lista de vecini */
            TVecin temp = current;
            current = current->urm;
            free(temp);
        }
    }
    free(harta);
}

int findOras(TOras *harta, int M, char *oras)
/* Functie pentru gasirea indexului unui oras */
{
    for (int i = 0; i < M; i++) {
        if (strcmp(harta[i].curr, oras) == 0)
            return i;
    }
    return -1;                                                  /* Returneaza indexul orasului sau -1 in caz ca nu l-am gasit */
}

void addDrum(TOras *harta, int *currentIndex, int M, char *src, char *dest, int cost)
/* Functie care daugă un drum între două orașe, adăugând orașele în hartă dacă nu există */
{
    int srcIndex = findOras(harta, M, src);
    if (srcIndex == -1) {
        srcIndex = *currentIndex;
        strcpy(harta[srcIndex].curr, src);
        harta[srcIndex].index = srcIndex;
        (*currentIndex)++;
    }                                                           /* Daca sursa nu exista o adaugam */

    int destIndex = findOras(harta, M, dest);
    if (destIndex == -1) {
        destIndex = *currentIndex;
        strcpy(harta[destIndex].curr, dest);
        harta[destIndex].index = destIndex;
        (*currentIndex)++;                                      /* Daca destinatia nu exista o adaugam */
    }

    TVecin newVecin = (TVecin)malloc(sizeof(TCelVec));
    newVecin->cost = cost;
    newVecin->destIndex = destIndex;
    newVecin->urm = harta[srcIndex].vecini;
    harta[srcIndex].vecini = newVecin;                          /* Adaugam vecinul pentru orasul sursa */

    newVecin = (TVecin)malloc(sizeof(TCelVec));
    newVecin->cost = cost;
    newVecin->destIndex = srcIndex;
    newVecin->urm = harta[destIndex].vecini;
    harta[destIndex].vecini = newVecin;                         /* Adaugam vecinul pentru orasul destinatie */
}

int* dijkstra(TOras *harta, int M, char *start, TDrum *drumuri, int *nrMuchi)
/* Algoritmul Dijkstra pentru găsirea costurilor minime de la un oraș de start */
{
    int startIndex = findOras(harta, M, start);
    if (startIndex == -1) return NULL;                          /* Dacă orașul de start nu există, returnează NULL */

    int *minCosts = (int *)malloc(M * sizeof(int));
    int visited[M];
    memset(visited, 0, sizeof(visited));
    for (int i = 0; i < M; i++) {
        minCosts[i] = 100000;                                   /* Inițializam costurile minime cu o valoare mare */
    }

    minCosts[startIndex] = 0;                                   /* Costul min pentru orasul de start */
    harta[startIndex].minCost = 0;

    for (int count = 0; count < M - 1; count++) {
        int min = 100000, minIndex = -1;

        for (int v = 0; v < M; v++) {
            if (!visited[v] && minCosts[v] < min) {             /* Iteram prin fiecare oras si gasim orasul cu cost minim care nu a fost vizitat */
                min = minCosts[v];
                minIndex = v;
            }
        }

        if (minIndex == -1) break;                              /* Dacă nu mai sunt orașe de vizitat, oprim cautarea */

        visited[minIndex] = 1;                                  /* Marcam orasul ca vizitat */

        for (TVecin vecin = harta[minIndex].vecini; vecin != NULL; vecin = vecin->urm) {        /* Actualizeaza costurile pentru vecini */
            int v = vecin->destIndex;

            int roadActive = 0;
            for (int i = 0; i < M; i++) {                       /* Verificăm dacă drumul între orașe este activ */
                if ((strcmp(drumuri[i].src, harta[minIndex].curr) == 0 && strcmp(drumuri[i].dest, harta[v].curr) == 0) ||
                    (strcmp(drumuri[i].dest, harta[minIndex].curr) == 0 && strcmp(drumuri[i].src, harta[v].curr) == 0)) {
                    roadActive = drumuri[i].active;
                    break;
                }
            }

            if (roadActive && !visited[v] && minCosts[minIndex] + vecin->cost < minCosts[v]) { /* Actualizam costul minim dacă se găsește un drum mai ieftin */
                minCosts[v] = minCosts[minIndex] + vecin->cost;
                (*nrMuchi)++;
            }
        }
    }

    return minCosts;
}

int countActiveDrumuri(TDrum *drumuri, int M_exist)
/* Functie care numara drumurile active */
{
    int count = 0;
    for (int i = 0; i < M_exist; i++) {
        if (drumuri[i].active) {
            count++;
        }
    }
    return count;
}

void printRemainingDrumuri(FILE *fout, TDrum *drumuri, int M_exist, TOras *harta, int M)
/* Functie care printeaza drumurile active prin verificarea daca exista in harta */
{
    for (int i = 0; i < M_exist; i++) {
        int srcIndex = findOras(harta, M, drumuri[i].src);
        int destIndex = findOras(harta, M, drumuri[i].dest);

        if (srcIndex != -1 && destIndex != -1) {               /* Daca orasele sursa si destinatie exxista in harta */
            TVecin current = harta[srcIndex].vecini;
            while (current != NULL) {
                if (current->destIndex == destIndex) {          /* Verificam daca exista drum intre ele */
                    fprintf(fout, "%s %s\n", drumuri[i].src, drumuri[i].dest);
                    break;
                }
                current = current->urm;
            }
        }
    }
}

void removeInactiveEdges(TOras *harta, int M, TDrum *drumuri)
/* Functie care elimina drumurile inactive din harta */
{
    for (int i = 0; i < M; i++) {                               /* Iteram prin fiecare oras */
        TVecin prev = NULL, current = harta[i].vecini;
        while (current != NULL) {                               /* Iteram prin fiecare vecin */
            int active = 0;
            for (int j = 0; j < M; j++) {
                if ((strcmp(drumuri[j].src, harta[i].curr) == 0 && strcmp(drumuri[j].dest, harta[current->destIndex].curr) == 0) ||
                    (strcmp(drumuri[j].dest, harta[i].curr) == 0 && strcmp(drumuri[j].src, harta[current->destIndex].curr) == 0)) {
                    active = drumuri[j].active;                 /* Verificam daca drumul este */
                    break;
                }
            }

            if (!active) {                                      /* Daca drumul nu este activ il eliminam */
                if (prev == NULL) {
                    harta[i].vecini = current->urm;
                } else {
                    prev->urm = current->urm;
                }
                TVecin temp = current;
                current = current->urm;
                free(temp);
            } else {                                            /* Daca drumul este activ trecem la urmatorul vecin */
                prev = current;
                current = current->urm;
            }
        }
    }
}

void task2() {
    FILE *fin = fopen("tema3.in", "r");
    if (!fin) return;

    FILE *fout = fopen("tema3.out", "w");
    if (!fout) return;

    char oras_start[50];
    int K_pastrate = 0, M_exist = 0;

    fscanf(fin, "%s", oras_start);
    fscanf(fin, "%d %d", &K_pastrate, &M_exist);                /* Citim orasul de start, nr de drumuri de pastrat si nr de drumuri existente */

    TOras *harta = InitOras(M_exist);                           /* Initializam orasul */
    TDrum *drumuri = (TDrum*)malloc(M_exist * sizeof(TDrum));
    char src[50], dest[50];
    int cost = 0;
    int index = 0;

    for (int i = 0; i < M_exist; i++) {
        fscanf(fin, "%s%s%d", src, dest, &cost);

        strcpy(drumuri[i].src, src);
        strcpy(drumuri[i].dest, dest);
        drumuri[i].cost = cost;
        drumuri[i].active = 1;

        addDrum(harta, &index, M_exist ,src, dest, cost);      /* Adaugam fiecare drum in harta */
    }

    int nrMuchi = 0;
    int *minCosts = dijkstra(harta, M_exist, oras_start, drumuri, &nrMuchi);                        /* Calculam drumurile minime */

    int sumaInitiala = 0;
    for (int i = 0; i < M_exist; i++) {
        sumaInitiala += minCosts[i];                            /* Calculam suma costurilor minime */
    }

    int max = -1;
    while (countActiveDrumuri(drumuri, M_exist) > K_pastrate) {
        int routeToRemove = -1;
        max = -1;
        for (int i = 0; i < M_exist; i++) {                     /* Verificam fiecare drum */
            if (drumuri[i].active) {
                drumuri[i].active = 0;

                int nrMuchiTemp = 0;
                int *tempMinCosts = dijkstra(harta, M_exist, oras_start, drumuri, &nrMuchiTemp);            /* Calculam drumurile minime temporare */

                int sumaTemp = 0;
                sumaInitiala = 0;
                for (int j = 0; j < M_exist; j++) {
                    sumaTemp += tempMinCosts[j];
                    sumaInitiala += minCosts[j];                /* Calculam suma costurilor temporare */
                }

                if (max < nrMuchiTemp) {                        /* Dacă numărul de muchii temporare este mai mare, actualizează max și routeToRemove */
                    max = nrMuchiTemp;
                    routeToRemove = i;
                    memcpy(minCosts, tempMinCosts, M_exist * sizeof(int));

                } else if (max > nrMuchiTemp) {                 /* Dacă numărul de muchii temporare este mai mic, verifică suma costurilor */
                    if (sumaInitiala > sumaTemp) {
                        routeToRemove = i;
                        memcpy(minCosts, tempMinCosts, M_exist * sizeof(int));
                    }

                } else {                                        /* Dacă numărul de muchii temporare este egal, verificam suma costurilor */
                    if (sumaInitiala > sumaTemp) {
                        routeToRemove = i;
                        memcpy(minCosts, tempMinCosts, M_exist * sizeof(int));

                    } else if (sumaInitiala == sumaTemp) {      /* Daca sumele sunt egale salvam ruta si oprim cautarea */
                        routeToRemove = i;
                        memcpy(minCosts, tempMinCosts, M_exist * sizeof(int));
                        free(tempMinCosts);
                        break;
                    }
                }

                free(tempMinCosts);
                drumuri[i].active = 1;                          /* Reactivam drumul */
            }
        }

        if (routeToRemove != -1) {
            drumuri[routeToRemove].active = 0;
        } 
        else break;                                             /* Daca nu s-a gasit niciun drum oprim cautarea */
    }

    while (1) {                                                 /* Continuam cautarea pentru muchiile care mai pot fi eliminat dupa ce am ajuns K */
        int routeToRemove = -1;
        float sumaTemp = 0;
        max = -1;
        for (int i = 0; i < M_exist; i++) {                     /* Verificam fiecare drum */
            if (drumuri[i].active) {
                drumuri[i].active = 0;

                int nrMuchiTemp = 0;
                int *tempMinCosts = dijkstra(harta, M_exist, oras_start, drumuri, &nrMuchiTemp);        /* Costuri minime temporare */

                sumaTemp = 0;
                sumaInitiala = 0;

                for (int j = 0; j < M_exist; j++) {             /* Suma costurilor temporare */
                    sumaTemp += tempMinCosts[j];
                    sumaInitiala += minCosts[j];
                }

                if (sumaInitiala == sumaTemp) {                 /* Daca suma initiala este egala cu suma temporara inseamna ca am gasit o muchie care poate fi eliminata */
                    routeToRemove = i;
                    memcpy(minCosts, tempMinCosts, M_exist * sizeof(int));
                }

                free(tempMinCosts);
                drumuri[i].active = 1;                          /* Reactivam drumul */
            }
        }

        if (routeToRemove != -1) {
            printf("%s -> %s\n", drumuri[routeToRemove].src, drumuri[routeToRemove].dest);
            drumuri[routeToRemove].active = 0;
        } else {
            break;                                              /* Daca am gasit un drum il eliminam, daca nu iesim din while */
        }
    }

    removeInactiveEdges(harta, M_exist, drumuri);                               /* Elibram drumurile inactive din graf */
    fprintf(fout, "%d\n", countActiveDrumuri(drumuri, M_exist));                /* Printeam nr de drumuri active  ramase */
    printRemainingDrumuri(fout, drumuri, M_exist, harta, M_exist);             /* Printeaza drumurile active ramase */

    freeHarta(harta, M_exist);
    free(drumuri);
    free(minCosts);

    fclose(fin);
    fclose(fout);
}