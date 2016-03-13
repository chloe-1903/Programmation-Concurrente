/**
 * Modelisation de l'evolution de la chaleur sur une plaque chauffante
 * 20/02/2016
 * Chloe Guglielmi et Lucas Sauvage
 */

//VERIFIER LES BARIERES!!!!!!!!!!!!!!!!!!

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
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
    nbThread=4;//à supprimer!!!!
    int taille=matInfo->taille;

    pthread_t thread_id_1,thread_id_2,thread_id_3,thread_id_4;
    pthread_attr_t attr;
    int ret;
    void *res;
    if(pthread_barrier_init(&barrier,NULL,4)){
        printf("Could not create a barrier\n");
        return -1;
    }
    float **matrice= matInfo->matrice;

    MatriceInfo *copie1;
    copie1=(MatriceInfo *)malloc(sizeof(MatriceInfo));
    copie1->matrice = matInfo->matrice;
    copie1->taille = matInfo->taille;
    copie1->TEMP_FROID = matInfo->TEMP_FROID ;
    copie1->deb_i=0;
    copie1->deb_j=0;
    copie1->fin_i=taille/(nbThread/2);
    copie1->fin_j=taille/(nbThread/2);
    ret=pthread_create(&thread_id_1,NULL,&uneIterationV2,(void*)copie1);
    if(ret!=0) {
        printf("Unable to create thread1");
        return -1;
    }

    MatriceInfo *copie2; 
    copie2=(MatriceInfo *)malloc(sizeof(MatriceInfo));
    copie2->matrice = matInfo->matrice;
    copie2->taille = matInfo->taille;
    copie2->TEMP_FROID = matInfo->TEMP_FROID ;
    copie2->deb_i=0;
    copie2->deb_j=taille/(nbThread/2);
    copie2->fin_i=taille/(nbThread/2);
    copie2->fin_j=2*(taille/(nbThread/2));
    ret=pthread_create(&thread_id_2,NULL,&uneIterationV2,(void*)copie2);
    if(ret!=0) {
        printf("Unable to create thread2");
        return -1;
    }

    MatriceInfo *copie3;
    copie3=(MatriceInfo *)malloc(sizeof(MatriceInfo));
    copie3->matrice = matInfo->matrice;
    copie3->taille = matInfo->taille;
    copie3->TEMP_FROID = matInfo->TEMP_FROID ;
    copie3->deb_i=taille/(nbThread/2);
    copie3->deb_j=taille/(nbThread/2);
    copie3->fin_i=2*(taille/(nbThread/2));
    copie3->fin_j=2*(taille/(nbThread/2));
    ret=pthread_create(&thread_id_3,NULL,&uneIterationV2,(void*)copie3);
    if(ret!=0) {
        printf("Unable to create thread3");
        return -1;
    }

    MatriceInfo *copie4;
    copie4=(MatriceInfo *)malloc(sizeof(MatriceInfo));
    copie4->matrice = matInfo->matrice;
    copie4->taille = matInfo->taille;
    copie4->TEMP_FROID = matInfo->TEMP_FROID ;
    copie4->deb_i=taille/(nbThread/2);
    copie4->deb_j=0;
    copie4->fin_i=2*(taille/(nbThread/2));
    copie4->fin_j=taille/(nbThread/2);
    ret=pthread_create(&thread_id_4,NULL,&uneIterationV2,(void*)copie4);
    if(ret!=0) {
        printf("Unable to create thread4. Code: %d \n", ret);
        return -1;
    }
    
    if(pthread_join(thread_id_1,NULL)){
            printf("Could not join thread 1\n");
            return -1;
    }
    if(pthread_join(thread_id_2,NULL)){
            printf("Could not join thread 2\n");
            return -1;
    }
    if(pthread_join(thread_id_3,NULL)){
            printf("Could not join thread 3\n");
            return -1;
    }
    if(pthread_join(thread_id_4,NULL)){
            printf("Could not join thread 4\n");
            return -1;
    }

    pthread_barrier_destroy(&barrier);
        free(copie1);    free(copie2);    free(copie3);    free(copie4);
    //m->fin_i=taille/(nbThread/2)+m->fin_i;
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
    tmp = (float * *) malloc( taille * sizeof( float * ) ) ; 
    //if ( tmp==NULL ) return -1 ; 
    int i, j;
    for (i = 0 ; i < taille ; i++ ) {
      tmp[i] = ( float * ) malloc( taille * sizeof(float) ) ; 
      //if ( tmp[i]==NULL ) return -1; ; 
    } 

    //Copie de la matrice dans tmp
    for(i=0; i< taille ; i++){
        for(j=0; j< taille ; j++) {
            tmp[i][j] =  matrice[i][j];
        }
    }
    //printf("Je vais de : %d-%d à %d-%d \n", m->deb_i, m->deb_j, m->fin_i, m->fin_j);
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
            }
        }
    }

    //Copie de la matrice tmp dans celle finale
    for(i=m->deb_i; i< m->fin_i ; i++){
        for(j=m->deb_j; j< m->fin_j ; j++){
            matrice[i][j] = tmp[i][j];
        }
    }

    //On free la matrice temporaire
    for ( i = 0 ; i < taille ; ++i ) {free( tmp[i] );} 
    free( tmp );
    int rc=pthread_barrier_wait(&barrier);
    if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
    {
        printf("Could not wait on barrier\n");
        exit(-1);
    }
}
