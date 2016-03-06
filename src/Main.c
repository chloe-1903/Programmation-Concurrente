/**
 * Modelisation de l'evolution de la chaleur sur une plaque chauffante
 * 20/02/2016
 * Chole Guglielmi et Lucas Sauvage
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <sys/resource.h>
#include "Main.h"
#include "Affichage.h"
#include "Operation.h"

//Temperatures predefinies
float TEMP_FROID = 0;
float TEMP_CHAUD = 256;

//Taille de la matrice
int n;
int taille;

//Structure pour calculer la memoire utilise
struct rusage r_usage;


/**
 * Fonction main
 * Recupere les valeurs en entree en fonction de differentes options
 * Appel la fonction lancement
 * @author Lucas
 */
int main(int argc, char *argv[]) {

    //Variables des differentes options recupere en entree
    int *s = malloc (sizeof(int));
    int tailleS;
    int it = 10000;
    int *e = malloc (sizeof(int));
    int tailleE;
    int *t = malloc (sizeof(int));
    int tailleT;
    int m = 0 , a = 0, M = 0; //Booleen a faux
    //Tableau de char pour decouper les options
    char tmp[1];

    //Variable pour recuperer les options
    int optch;
    extern char * optarg;
    extern int optind, opterr;

    //Recuperation des options
    while ((optch = getopt(argc, argv, "s:mMai:e:t:")) != -1)
        switch (optch) {
            case 's':
                //On decoupe les differentes tailles de problemes
                tailleS = strlen(optarg);
                for(int i=0; i < tailleS; i++) {
                    *tmp = optarg[i];
                    s[i] = atoi(tmp);
                }
                break;
            case 'm':
                m = 1;
                break;
            case 'a':
                a = 1;
                break;
            case 'M':
                M = 1;
                break;
            case 'i':
                it = atoi(optarg);
                break;
            case 'e':
                //On decoupe les differentes etapes a executer
                tailleE = strlen(optarg);
                for(int i=0; i < tailleE; i++) {
                    *tmp = optarg[i];
                    e[i] = atoi(tmp);
                }
                break;
            case 't':
                //On decoupe les differents nombre de thread a executer
                tailleT = strlen(optarg);
                for(int i=0; i < tailleT; i++) {
                    *tmp = optarg[i];
                    t[i] = atoi(tmp);
                }
                break;
        }

    //Regarde dans l'option e quels sont les programmes a executer
    for (int i= 0; i < tailleE; i++) {

        //Iteratif
        if(e[i] == 0) {

            //Appel de la fonction lancement
            lancement(tailleS, s, it, a, m, M);

        }

    }


    return 0;
}


/**
 * Fonction lancement
 * Lance la fonction lancerUnScenario en prenant en compte le parametre -m
 * @author Chloe
 */
int lancement(int tailleS, int s[], int it, int a, int m, int M) {

    //On gere l'option -s
    for (int i=0; i< tailleS; i++) {

        //Variable pour le calcul de temps
        float temps[10];
        float temps2[10];

        //Mise a jour de la taille
        n = 4 + s[i];
        taille = 2<<(n-1);

        //On gere l'option m et M
        if(m || M){
            //on fait 10x la scenario pour avoir une moyenne du temps (on enleve les deux extremes)
            lancerUnScenario(taille, it, a, n, TEMP_FROID, TEMP_CHAUD, &temps[0], &temps2[0]);
            float min=temps[0];
            float max=temps[0];

            float min2=temps2[0];
            float max2=temps2[0];

            for (int j=1; j<10; j++){
                lancerUnScenario(taille, it, 0,  n , TEMP_FROID, TEMP_CHAUD, &temps[j], &temps2[j]);//a =0 car on ne veut pas l'afficher a chaque fois
                if (temps[j]>max)
                    max=temps[j];
                if (temps[j]<min)
                    min=temps[j];
                if (temps2[j]>max2)
                    max2=temps2[j];
                if (temps2[j]<min2)
                    min2=temps2[j];

            }
            //Calcul de la moyenne:
            float somme=0;
            int cmpt=0;

            float somme2=0;
            int cmpt2=0;

            for (int j = 0; j < 10; ++j){
                //printf("temps[i]: %f\n", temps[j]);
                if(temps[j]!=max && temps[j]!=min){
                    somme+=temps[j];
                    cmpt++;
                }

                if(temps2[j] == min2){
                    min2 = -1;
                }
                else if(temps2[j] == max2) {
                    max2 = -1;
                }
                else {
                    somme2 += temps2[j];
                    cmpt2++;
                }
            }

            float f=somme/cmpt;
            float f2=somme2/cmpt2;

            if(m) {
                printf("Temps moyen (CPU) pour matrice de taille %d : %f s\n", taille, f);
            }
            if(M) {
                printf("Temps moyen (utilisateur) pour matrice de taille %d : %f s\n", taille, f2);
            }

        } else {
            lancerUnScenario(taille, it, a,  n ,TEMP_FROID, TEMP_CHAUD, &temps[0], &temps2[0]);

        }

        //Recupere et affiche la memoire utilisee
        getrusage(RUSAGE_SELF, &r_usage);
        printf("Memoire utilisee (taille %d) : %ld ko\n",taille, r_usage.ru_maxrss);

    }
    return 0;
}

/**
 * Fonction lancement d'un scenario
 * Lance les operations sur la matrice : chauffe le milieu et effectue la repartition de la chaleur
 * @author Lucas
 */
int lancerUnScenario(int taille, int it, int a, int n, float TEMP_FROID, float TEMP_CHAUD, float *temps, float *temps2){
    //Matrice
    float matrice[taille][taille];

    //Pour mesurer le temps
    clock_t t1, t2;
    time_t tt1, tt2;
    t1 = clock();
    tt1 = time(NULL);

    //On initialise la matrice avec des 0
    miseAzero((float*)matrice, taille);

    chaufferMilieu((float*)matrice, n, taille, TEMP_CHAUD);

    //Affiche le quart de la matrice (avant execution) si l'option a est utilisee
    if(a) {
        afficherQuart((float*)matrice, taille);
    }

    for (int i=1; i <= it; i++) {
        uneIteration((float*)matrice, taille, TEMP_FROID);
        //On remet le centre chaud
        chaufferMilieu((float*)matrice, n, taille, TEMP_CHAUD);
    }

    //Affiche le quart de la matrice (apres execution) si l'option a est utilisee
    if(a) {
       afficherQuart((float*)matrice, taille);
    }

    //Pour calculer le temps
    t2 = clock();
    tt2 = time(NULL);
    *temps = (float) (t2 - t1) / CLOCKS_PER_SEC;
    *temps2 = (float) (tt2 - tt1);
    return 0;
}


