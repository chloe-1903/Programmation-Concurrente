/*
 * @authors Chloe et Lucas
 */

#ifndef PROJET_PROG_CONCURRENTE_OPERATION_H
#define PROJET_PROG_CONCURRENTE_OPERATION_H

typedef struct MatriceInfo MatriceInfo;
struct MatriceInfo{
    float** matrice;
    float** tmp;
    int taille;
    float TEMP_FROID;
};

int chaufferMilieu( float **matrice, int n, int taille, float TEMP_CHAUD);
int uneIterationV2(void *matInfo);//float **matrice,float **tmp, int taille, float TEMP_FROID);
int miseAFroid(float **matrice, int taille, float TEMP_FROID);
//int lancerThreads(MatriceInfo matInfo, int nbThread);

#endif //PROJET_PROG_CONCURRENTE_OPERATION_H
