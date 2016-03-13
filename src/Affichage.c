/**
 * Modelisation de l'evolution de la chaleur sur une plaque chauffante
 * 20/02/2016
 * Chole Guglielmi et Lucas Sauvage
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Affichage.h"

/**
 * Fonction afficher
 * Affiche l'integralite de la matrice
 * @author Chloe
 */
int afficher(float **matrice, int taille){
    int i, j;
    for(i=0; i< taille ; i++){
        for(j=0; j< taille ; j++){
            printf("%.2f-", matrice[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}


/**
 * Fonction afficherQuart
 * Affiche un quart de la matrice
 * @author Chloe
 */
int afficherQuart(float **matrice, int taille){
    int i, j;
    for(i=0; i< taille/2 ; i++){
        for(j=0; j< taille/2 ; j++){
            printf("%.2f-", matrice[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}


