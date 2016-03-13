/*
 * @authors Chloe et Lucas
 */

#ifndef PROJET_PROG_CONCURRENTE_OPERATION_H
#define PROJET_PROG_CONCURRENTE_OPERATION_H

typedef struct MatriceInfo MatriceInfo;
struct MatriceInfo{
    float** matrice;
    int taille;
    float TEMP_FROID;
    int deb_i;
    int deb_j;
    int fin_i;
    int fin_j;
};

int chaufferMilieu( float **matrice, int n, int taille, float TEMP_CHAUD);
void *uneIterationV2(void *matInfo);//float **matrice,float **tmp, int taille, float TEMP_FROID);
int miseAFroid(float **matrice, int taille, float TEMP_FROID);
int lancerThreads(MatriceInfo *matInfo, int nbThread);
//lancerThread choisit deb i, deb j...-> pb car on lui passe matInfo sans ces 4 champs?

#endif //PROJET_PROG_CONCURRENTE_OPERATION_H
