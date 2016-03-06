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
int afficher(float *matrice, int taille){
    for(int i=0; i< taille ; i++){
        for(int j=0; j< taille ; j++){
            printf("%.2f-", matrice[(taille*i)+j]);
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
int afficherQuart(float *matrice, int taille){
    for(int i=0; i< taille/2 ; i++){
        for(int j=0; j< taille/2 ; j++){
            printf("%.2f-", matrice[(taille*i)+j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

/**
 * Fonction afficherTabInt
 * Affiche un tableau de int (utilite nottament pour verifier si les parametres en entree ont bien ete recupere)
 * @author Lucas
 */
int afficherTabInt(int tab[], int taille) {

    for(int i=0; i < taille; i++) {
        printf("%d", tab[i]);
    }
    printf("\n");

    return 0;
}
