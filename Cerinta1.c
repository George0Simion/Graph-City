/* SIMION George Constantin - 313CB */

#include "orase.h"

TGL *InitGraph(int R)
/* Inițializăm un graf cu R noduri */
{
    TGL *graf = (TGL*)malloc(R * sizeof(TGL));
    for (int i = 0; i < R; i++) {
        graf[i].oras_curent[0] = '\0';
        graf[i].max_uzura = 0.0;
        graf[i].x = 0;                                                      /* Iteram prin fiecare nod, setam uzura la 0, si pointerul lalista de arce la NULL */
    }
    return graf;
}

void freeGraf(TGL *graf, int rute)
/* Eliberam memoria alocata pentru graf si arce */
{
    for (int i = 0; i < rute; i++) {                                        /* Iteram prin fiecare nod */
        AArc arc = graf[i].x;
        while (arc) {                                                       /* Iteram prin fiecare arc si eliberam memoria */
            AArc temp = arc;
            arc = arc->urm;
            free(temp->uzura);
            free(temp);
        }
    } 
    free(graf);
}

void freeOrase(char ***orase, int rute)
/* Eliberam memoria alocata pentru matricea de orase */
{
    for (int i = 0; i < rute; i++) {
        for (int j = 0; j < 2; j++) {
            free(orase[i][j]);
        }
        free(orase[i]);
    }
    free(orase);
}

void addMuchie(TGL *graf, int R, char *src, char *dest, int nr_tronsoane, float *uzura)
/* Adaugam o muchie intre doua noduri in graf */
{
    AArc new_arc = (AArc)malloc(sizeof(TCelArc));
    strcpy(new_arc->vecin, dest);                                           /* Adaugam destinatia arcului */

    new_arc->nr_trons = nr_tronsoane;
    new_arc->uzura = (float*)malloc(nr_tronsoane * sizeof(float));
    for (int i = 0; i < nr_tronsoane; i++) {
        new_arc->uzura[i] = uzura[i];                                       /* Copiem uzura pentru fiecare tronson */
    }

    new_arc->urm = NULL;
    new_arc->directie = 1;                                                  /* Setam directia arcului */

    int found_src = 0;
    for (int i = 0; i < R; i++) {                                           /* Cautam orasul sursa in graf */
        if (strcmp(graf[i].oras_curent, src) == 0) {
            found_src = 1;
            new_arc->urm = graf[i].x;
            graf[i].x = new_arc;
            break;
        }
    }

    if (!found_src) {                                                       /* Daca orasul nu a fost gasit il adaugam */
        for (int i = 0; i < R; i++) {
            if (graf[i].oras_curent[0] == '\0') {
                strcpy(graf[i].oras_curent, src);
                graf[i].x = new_arc;
                break;
            }
        }
    }

    AArc new_arc_reverse = (AArc)malloc(sizeof(TCelArc));
    strcpy(new_arc_reverse->vecin, src);                                    /* Setam sursa arcului invers */

    new_arc_reverse->nr_trons = nr_tronsoane;
    new_arc_reverse->uzura = (float*)malloc(nr_tronsoane * sizeof(float));
    for (int i = 0; i < nr_tronsoane; i++) {
        new_arc_reverse->uzura[i] = uzura[i];                               /* Adaugam uzura */
    }

    new_arc_reverse->urm = NULL;
    new_arc_reverse->directie = 0;                                          /* Setam directia */

    int found_dest = 0;
    for (int i = 0; i < R; i++) {                                           /* Cautam orasul destinatie in graf */
        if (strcmp(graf[i].oras_curent, dest) == 0) {
            found_dest = 1;
            new_arc_reverse->urm = graf[i].x;
            graf[i].x = new_arc_reverse;
            break;
        }
    }

    if (!found_dest) {                                                      /* Daca orasul destinatie nu a fost gasit il adaugam in graf */
        for (int i = 0; i < R; i++) {
            if (graf[i].oras_curent[0] == '\0') {
                strcpy(graf[i].oras_curent, dest);
                graf[i].x = new_arc_reverse;
                break;
            }
        }
    }
}

void calculate_max_uzura(TGL *graf, int R)
/* Calculam uzura maxima pentru fiecare nod din graf */
{
    for (int i = 0; i < R; i++) {
        AArc arc = graf[i].x;
        graf[i].max_uzura = 0.0;

        while (arc) {                                                       /* Iteram prin fiecare vecin si calculam maximul uzurii de pe langa nod */
            float uzura_near = (arc->directie == 1) ? arc->uzura[0] : arc->uzura[arc->nr_trons - 1];
            if (uzura_near > graf[i].max_uzura) {
                graf[i].max_uzura = uzura_near;
            }

            arc = arc->urm;
        }
    }
}

void updateZero(TGL *graf, int R)
/* Actualizam uzurile care sunt cu 0 */
{
    for (int i = 0; i < R; i++) {
        AArc arc = graf[i].x;
        while (arc) {                                                       /* Iteram prin fiecare nod / vecin */
            float *cop = malloc(arc->nr_trons * sizeof(float));
            memcpy(cop, arc->uzura, arc->nr_trons * sizeof(float));         /* Copiem vectorul initial de uzuri */

            for (int j = 1; j < arc->nr_trons - 1; j++) {                   /* Iteram prin uzurile care nu sunt la marginele vectorului */
                if (arc->uzura[j] == 0) {
                    float max = cop[j - 1];
                    if (max < cop[j + 1]) {
                        max = cop[j + 1];
                    }
                    arc->uzura[j] = max / 4;                                /* Calculam maximul din stanga si din dreapta si il atribuim 0-ului curent */
                }
            }

            if (cop[0] == 0 && arc->nr_trons >= 2) {                        /* Daca e 0 la inceputul vectorului de uzuri */
                if (arc->directie == 1) {
                    arc->uzura[0] = cop[1] / 4;

                    if (arc->uzura[0] < graf[i].max_uzura / 4) {
                        arc->uzura[0] = graf[i].max_uzura / 4;              /* Pentru directia 1 salvam veccinul lui 0 de pe acel tronsom si dil verificam cu maximul din nod */
                    }

                } else if (arc->directie == 0) {                            /* Pentru directia 0 salvam uzura de la final, de pe acelasi tronson, de langa 0-ul curent */
                    arc->uzura[arc->nr_trons - 1] = cop[arc->nr_trons - 2] / 4;

                    for (int k = 0; k < R; k++) {
                        if (strcmp(arc->vecin, graf[k].oras_curent) == 0) { /* Verificam valoarea salvata cu maximul de pe nodul urmator */
                            if (arc->uzura[arc->nr_trons - 1] < graf[k].max_uzura / 4) {
                                arc->uzura[arc->nr_trons - 1] = graf[k].max_uzura / 4;
                            }
                            break;
                        }
                    }
                }
            } else if (cop[0] == 0 && arc->nr_trons == 1) {                 /* Daca e doar un element in vector verificam maximul de pe nodul curent cu maximul de pe nodul urmator */
                arc->uzura[0] = graf[i].max_uzura / 4;
                for (int k = 0; k < R; k++) {
                    if (strcmp(arc->vecin, graf[k].oras_curent) == 0) {
                        if (arc->uzura[0] < graf[k].max_uzura / 4) {
                            arc->uzura[0] = graf[k].max_uzura / 4;
                        }
                        break;
                    }
                }
            }

            if (cop[arc->nr_trons - 1] == 0) {                              /* Daca e 0 pe ultima pozitie */
                for (int j = 0; j < R; j++) {
                    if (strcmp(arc->vecin, graf[j].oras_curent) == 0) {
                        if (graf[i].x->directie == 1) {                     /* Salvam valoarea de dinainte si o comparam cu maximul de pe urmatorul nod */
                            if (arc->nr_trons > 1) {
                                arc->uzura[arc->nr_trons - 1] = cop[arc->nr_trons - 2] / 4;

                                if (arc->uzura[arc->nr_trons - 1] < graf[j].max_uzura / 4) {
                                    arc->uzura[arc->nr_trons - 1] = graf[j].max_uzura / 4;
                                }
                            } else {
                                arc->uzura[arc->nr_trons - 1] = graf[j].max_uzura / 4;
                            }
                        } else if (graf[i].x->directie == 0) {              /* Pentru directia 0 salvam valoarea urmatorea si o comparam cu maximul de pe nodul curent */
                            if (arc->nr_trons > 1) {
                                arc->uzura[0] = arc->uzura[arc->nr_trons - 2] / 4;

                                if (arc->uzura[0] < graf[j].max_uzura / 4) {
                                    arc->uzura[0] = graf[j].max_uzura / 4;
                                }
                            } else {
                                arc->uzura[0] = graf[j].max_uzura / 4;
                            }
                        }
                        break;
                    }
                }
            }

            free(cop);
            arc = arc->urm;
        }
    }
}

void updateUzura(TGL *graf, int rute)
/* Functie care fac ori *2 la uzurile  */
{
    for (int i = 0; i < rute; i++) {
        AArc arc = graf[i].x;
        while (arc) {
                for (int j = 0; j < arc->nr_trons; j++) {
                    arc->uzura[j] *= 2;
                }
            arc = arc->urm;
        }
    }
}

void printGraph(FILE *fout, TGL *graf, int R, float L, char ***matrice)
/* Functie care printeaza graful cu uzurile noi si muchiile care inca pot fi folosite */
{
    int index = 0;
    while (index < R / 2) {
        for (int i = 0; i < R; i++) {                                   /* Iteram prin fiecare nod */
            if (strcmp(graf[i].oras_curent, matrice[index][0]) == 0) {
                AArc arc = graf[i].x;

                while (arc) {
                    if (strcmp(arc->vecin, matrice[index][1]) == 0) {
                        fprintf(fout, "%s %s %d ", matrice[index][0], matrice[index][1], arc->nr_trons);

                        for (int j = 0; j < arc->nr_trons; j++) {       /* Printam arcele si uzurile in ordinea in care au fost date */
                            fprintf(fout, "%.2f ", arc->uzura[j]);
                        }
                        fprintf(fout, "\n");

                        index++;
                        break;
                    }
                    arc = arc->urm;
                }
                break;
            }
        }
    }

    float med = 0;
    int nr = 1;
    index = 0;
    while(index < R / 2) {
        for (int i = 0; i < R; i++) {
            if (strcmp(graf[i].oras_curent, matrice[index][0]) == 0) {
                AArc arc = graf[i].x;

                while (arc) {
                    med = 0;
                    if (strcmp(arc->vecin, matrice[index][1]) == 0) {
                        for (int j = 0; j < graf[i].x->nr_trons; j++) {
                            med += graf[i].x->uzura[j];
                        }
                        med /= graf[i].x->nr_trons;                      /* Calculam media uzurii */

                        if (med < L) {
                            fprintf(fout, "%d ", nr);                   /* Daca e mai mica ca L o afisam */
                        }
                        nr++;

                        break;
                    }
                    arc = arc->urm;
                }
                index++;
                break;
            }
        }
    }
}

void updateOneHundred(TGL *graf, int R)
/* Functie care face 100 din uzurile mai mari ca 100 */
{
    for (int i = 0; i < R; i++) {
        AArc arc = graf[i].x;
        while (arc) {
            for (int j = 0; j < arc->nr_trons; j++) {
                if (arc->uzura[j] > 100) {
                    arc->uzura[j] = 100;
                }
            }
            arc = arc->urm;
        }
    }
}

void task1() {
    FILE *fin = fopen("tema3.in", "r");
    if (!fin) return;

    FILE *fout = fopen("tema3.out", "w");
    if (!fout) return;

    int R_rute = 0, K_ani = 0, L_grad = 0;
    fscanf(fin, "%d %d %d", &R_rute, &K_ani, &L_grad);

    TGL *graf = InitGraph(2 * R_rute);
    char src[50], dest[50];
    int nr_tronsoane = 0;

    char ***orase = (char ***)malloc(2 * R_rute * sizeof(char**));
    for (int i = 0; i < 2 * R_rute; i++) {
        orase[i] = (char**)malloc(2 * sizeof(char*));
        for (int j = 0; j < 2; j++) {
            orase[i][j] = malloc(50);                                   /* Alocam spatiu pentru matricea de orasepe care o folosim pentru afisare */
        }
    }

    for (int i = 0; i < R_rute; i++) {
        fscanf(fin, "%s %s %d", src, dest, &nr_tronsoane);
        strcpy(orase[i][0], src);
        strcpy(orase[i][1], dest);

        float *uzura = (float*)malloc(nr_tronsoane * sizeof(float));
        for (int j = 0; j < nr_tronsoane; j++) {
            fscanf(fin, "%f", &uzura[j]);                               /* Citim orasele, uzurile si le adaugam in graf */
        }

        addMuchie(graf, 2 * R_rute, src, dest, nr_tronsoane, uzura);
        free(uzura);
    }

    for (int i = 0; i < K_ani; i++) {
        updateUzura(graf, 2 * R_rute);
        calculate_max_uzura(graf, R_rute);
        updateZero(graf, 2 * R_rute);
        calculate_max_uzura(graf, R_rute);                              /* Pe parcursul a K ani actualizam uzurile si calculam de fiecare data maximul in noduri */
    }

    updateOneHundred(graf, R_rute);
    printGraph(fout, graf, 2 * R_rute, L_grad, orase);

    freeGraf(graf, 2 * R_rute);
    freeOrase(orase, 2 * R_rute);                                       /* Printam graful si eliberam memoria */

    fclose(fin);
    fclose(fout);
}