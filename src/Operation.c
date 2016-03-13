/**
 * Modelisation de l'evolution de la chaleur sur une plaque chauffante
 * 20/02/2016
 * Chloe Guglielmi et Lucas Sauvage
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <math.h>
#include "Operation.h"

pthread_barrier_t barrier;

/**
 * Fonction chaufferMilieu
 * Chauffe le milieu de la matrice
 * @author Chloe
 */
int chaufferMilieu(float **matrice, int n, int taille, float TEMP_CHAUD) {
    int dep_chaud=(2<<(n-2))-(2<<(n-5));
    int fin_chaud=(2<<(n-2))+(2<<(n-5));
    if(n==4){ //car 2<<0=0 alors que 2^0=1
        dep_chaud=7;
        fin_chaud=9;
    }
    int i, j;
    for(i=dep_chaud; i<fin_chaud;i++){
        for (j=dep_chaud; j<fin_chaud;j++){
            matrice[i][j]=TEMP_CHAUD;
        }
    }
    return 0;
}

/**
 * Fonction miseAzero
 * Initialise la matrise avec des 0
 * @author Lucas
 */
int miseAFroid(float **matrice, int taille, float TEMP_FROID) {
    int i, j;
    for(i=0; i< taille ; i++) {
        for (j = 0; j < taille; j++) {
            matrice[i][j] = TEMP_FROID;
        }
    }

    return 0;
}

/**
 * Appelle les thread en fonction du nombre demande
 * @author Chloe
 */
int lancerThreads(MatriceInfo *matInfo, int nbThread){
    int taille=matInfo->taille;

    //Identifiants thread
    pthread_t *thread_ids;
    thread_ids=(pthread_t *)malloc(nbThread*sizeof(pthread_t));
    int i, j;
    for (i=0;i<nbThread;i++){
        pthread_t thread_id;
        thread_ids[i]= thread_id;
    }

    //Barriere
    int ret;
    if(pthread_barrier_init(&barrier,NULL,nbThread)){
        printf("Impossible de creer la barriere\n");
        return -1;
    }

    //Calcul du nombre de cases par thread
    double val= taille * taille / nbThread;
    int nbCaseParThread = sqrt((double)val);
    if (nbThread>taille){
        printf("Nombre de thread trop important par rapport a la taille.\n");
        return -1;
    }

    //Copies de matInfo pour chaque thread 
    MatriceInfo **copies;
    copies=(MatriceInfo **)malloc(nbThread*sizeof(MatriceInfo*));

    //Appel au thread en creant les copies et ajoutant les bons arguments
    int k=0;
    for(i = 0 ; i<taille ; i += nbCaseParThread){
        for(j = 0 ; j< taille ; j += nbCaseParThread){
            MatriceInfo *copie1;
            copie1=(MatriceInfo *)malloc(sizeof(MatriceInfo));
            copie1->matrice = matInfo->matrice;
            copie1->taille = matInfo->taille;
            copie1->TEMP_FROID = matInfo->TEMP_FROID ;   
            copie1->deb_i=i;
            copie1->deb_j=j;
            copie1->fin_i=i+nbCaseParThread;
            copie1->fin_j=j+nbCaseParThread;
            copies[k]=copie1;
           // printf("--> Ici je vais de : %d-%d à %d-%d \n", copie1->deb_i, copie1->deb_j, copie1->fin_i, copie1->fin_j);
            ret=pthread_create(&thread_ids[k],NULL,&uneIterationV2,(void*)copie1);
            if(ret!=0) {
                printf("Impossible de creer le thread.\n");
                return -1;
            }
            k++; 
        }
    }

    //Attentes des autres threads
    for (i=0; i<nbThread;i++){
        if(pthread_join(thread_ids[i],NULL)){
            printf("Impossible de joindre le thread.%d\n", i);
            return -1;
        }   
    }

    //destructions de la barriere et liberation memoire 
    pthread_barrier_destroy(&barrier);
    free(thread_ids);
    for (i=0; i<nbThread;i++){
        free(copies[i]);
    }
    free(copies);
    return 0;
}

/**
 * Fonction uneIteration
 * Effectue la repartition de la chaleur pour une iteration
 * @author Lucas & Chloe
 */
void *uneIterationV2(void *matInfo) {
    MatriceInfo *m=(MatriceInfo*)matInfo;
    float **matrice=m->matrice;
    int taille=m->taille;
    float TEMP_FROID=m->TEMP_FROID;

    //Matrice temporaire
    float **tmp;
    tmp = (float **)malloc(taille*sizeof(float*)) ; 
    int i, j;
    for (i = 0 ; i < taille ; i++ ) {
      tmp[i] = ( float * ) malloc( taille * sizeof(float) ) ; 
    } 

    //Copie de la matrice dans tmp
    for(i=0; i< taille ; i++){
        for(j=0; j< taille ; j++) {
            tmp[i][j] =  matrice[i][j];
        }
    }
    //Boucle pour repartir la chaleur
    for(i=m->deb_i; i< m->fin_i ; i++){
        for(j=m->deb_j; j< m->fin_j ; j++){

            //Ici il ne faut pas recuperer des valeurs en dehors de la matrice
            //Cas hors matrice negatif sur i et j
            if(i-1 < 0 && j-1 < 0) {
                //On ne fait que le milieu et les +
                //(matrice[i+1][j]) / 36 * 4;
                tmp[i][j] += (((matrice[i][j]) / 36) * 16) - ((matrice[i][j]));
                tmp[i][j] += ((matrice[i+1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j+1]) / 36) * 4;
                tmp[i][j] += ((matrice[i+1][j+1]) / 36);

            }
                //Cas positif hors matrice sur i et j
            else if(i+1 == taille && j+1 == taille) {
                //On ne fait que le milieu et les -
                tmp[i][j] += (((matrice[i][j]) / 36) * 16) - ((matrice[i][j]));
                tmp[i][j] += ((matrice[i-1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j-1]) / 36) * 4;
                tmp[i][j] += ((matrice[i-1][j-1]) / 36);

            }
                //Cas hors matrice i positif et j negatif
            else if (i+1 == taille && j-1 < 0){
                //On fait le milieu, les i negatif et les j positifs
                tmp[i][j] += (((matrice[i][j]) / 36) * 16) - ((matrice[i][j]));
                tmp[i][j] += ((matrice[i-1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j+1]) / 36) * 4;
                tmp[i][j] += ((matrice[i-1][j+1]) / 36);
            }
                //Cas hors matrice i negatif et j positif
            else if (i-1 < 0 && j+1 == taille){
                //On fait le milieu, les i positifs et les j negatifs
                tmp[i][j] += (((matrice[i][j]) / 36) * 16) - ((matrice[i][j]));
                tmp[i][j] += ((matrice[i+1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j-1]) / 36) * 4;
                tmp[i][j] += ((matrice[i+1][j-1]) / 36);

            }
                //Cas hors matrice negatif sur i
            else  if(i-1 < 0) {
                //On ne fait que les j, les i+1 et le milieu
                tmp[i][j] += (((matrice[i][j]) / 36) * 16) - ((matrice[i][j]));
                tmp[i][j] += ((matrice[i+1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j-1]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j+1]) / 36) * 4;
                tmp[i][j] += ((matrice[i+1][j+1]) / 36);
                tmp[i][j] += ((matrice[i+1][j-1]) / 36);

            }
                //Cas hors matrice negatif sur j
            else if(j-1 < 0) {
                //On ne fait que les i, les j+1 et le milieu
                tmp[i][j] += (((matrice[i][j]) / 36) * 16) - ((matrice[i][j]));
                tmp[i][j] += ((matrice[i-1][j]) / 36) * 4;
                tmp[i][j]+= ((matrice[i+1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j+1]) / 36) * 4;
                tmp[i][j] += ((matrice[i+1][j+1]) / 36);
                tmp[i][j] += ((matrice[i-1][j+1]) / 36);

            }
                //Cas positif hors matrice sur i
            else if(i+1 == taille) {
                //On ne fait que les j, les i-1 et le milieu
                tmp[i][j] += (((matrice[i][j]) / 36) * 16) - ((matrice[i][j]));
                tmp[i][j] += ((matrice[i-1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j-1]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j+1]) / 36) * 4;
                tmp[i][j] += ((matrice[i-1][j-1]) / 36);
                tmp[i][j] += ((matrice[i-1][j+1]) / 36);

            }
                //Cas positif hors matrice sur j
            else if(j+1 == taille) {
                //On ne fait que les i, les j-1 et le milieu
                tmp[i][j] += (((matrice[i][j]) / 36) * 16) - ((matrice[i][j]));
                tmp[i][j] += ((matrice[i-1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i+1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j-1]) / 36) * 4;
                tmp[i][j] += ((matrice[i-1][j-1]) / 36);
                tmp[i][j] += ((matrice[i+1][j-1]) / 36);

            }
            else {
                //On fait tout
                tmp[i][j] += (((matrice[i][j]) / 36) * 16) - ((matrice[i][j]));
                tmp[i][j] += ((matrice[i-1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i+1][j]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j-1]) / 36) * 4;
                tmp[i][j] += ((matrice[i][j+1]) / 36) * 4;
                tmp[i][j] += ((matrice[i-1][j-1]) / 36);
                tmp[i][j] += ((matrice[i+1][j+1]) / 36);
                tmp[i][j] += ((matrice[i-1][j+1]) / 36);
                tmp[i][j] += ((matrice[i+1][j-1]) / 36);
                //printf("tmp[i][j]=%d\n", tmp[i][j]);
            }
        }
    }

    //attente des autres thread
    int rc=pthread_barrier_wait(&barrier);
    if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
    {
        printf("Could not wait on barrier\n");
        exit(-1);
    }

    //Copie de la matrice tmp dans celle finale
    for(i=m->deb_i; i< m->fin_i ; i++){
        for(j=m->deb_j; j< m->fin_j ; j++){
            matrice[i][j] = tmp[i][j];
        }
    }

    //On free la matrice temporaire
    for ( i = 0 ; i < taille ; i++ ) {free( tmp[i] );} 
    free( tmp );

}
