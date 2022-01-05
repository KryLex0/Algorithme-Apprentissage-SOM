#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

struct vecteurIrisData
{
    double *vecteurData; //tableau avec les datas de iris data (5.1,3.5,1.4,0.2)
    double module;       //calculer la norme (normaliser) ||module||=sqrt(x1²+x2²+x3²+x4²)
    char *etiquette;     //espece qui sont dans le fichier
};

struct nodeMatrice
{
    double *vecteurData; //=>tableau de 4 valeurs
    double distance;     //=> distance (vide au debut)
    char *etiquette;     //=>espece
};

struct N_Config
{
    int nb_nodes;             //nombre de noeuds
    int nb_lignes;            //nombre de lignes
    int nb_colonnes;          //nombre de colonnes
    struct bmu *bmu;          //BMU
    struct nodeMatrice **map; //Matrice de noeuds
};

struct bmu
{
    int ligne;
    int colonne;
    struct bmu *next;
    int nbBMU;
};

struct N_Config nodeConfig;
struct nodeMatrice **matrice;

struct bmu *cell, *tete;

struct vecteurIrisData tab_vecteurs_init[150];
struct vecteurIrisData tab_vecteurs_shuffle[150];
float moy_col1 = 0;
float moy_col2 = 0;
float moy_col3 = 0;
float moy_col4 = 0;

float moyennesColonnes[4];

int nbLignesVecteurIrisData = sizeof(tab_vecteurs_init) / sizeof(tab_vecteurs_init[0]);
int nbColonnesVecteurIrisData = sizeof(tab_vecteurs_init[0]) / sizeof(tab_vecteurs_init[0].vecteurData[0]) + 1;

//fonction qui prend en paramètre le fichier de données iris-data
//et qui récupère chaque vecteurs afin de les sauvegarder dans un tableau de vecteurs
void convertIrisDataToStruct(FILE *fichier)
{
    int compteur = 0;
    char line[50];
    const char *separators = ",";
    float moduleTemp = 0;

    if (fichier != NULL)
    {
        while (fgets(line, 1000, fichier))
        {
            //printf("Vecteur : %d \n",compteur);
            char *strToken = strtok(line, separators);
            tab_vecteurs_init[compteur].vecteurData = (double *)malloc(4 * sizeof(double));

            for (int i = 0; i < 5; i++)
            {
                if (i < 4)
                {
                    tab_vecteurs_init[compteur].vecteurData[i] = atof(strToken);
                    moduleTemp += pow(tab_vecteurs_init[compteur].vecteurData[i], 2);
                }
                else
                {
                    tab_vecteurs_init[compteur].module = sqrt(moduleTemp);
                    tab_vecteurs_init[compteur].etiquette = malloc(20 * sizeof(char));
                    strncpy(tab_vecteurs_init[compteur].etiquette, strToken, 20);
                }
                strToken = strtok(NULL, separators);
            }
            moduleTemp = 0;
            compteur += 1;
        }
        fclose(fichier);
    }
    else
    {
        printf("Erreur! Ce fichier n'existe pas");
    }
}

//fonction qui affiche les vecteurs iris data
void afficherDonneesVecteurs()
{
    for (int i = 0; i < nbLignesVecteurIrisData; i++)
    {
        printf("Vecteur: %d\t\t", (i + 1));
        for (int j = 0; j < nbColonnesVecteurIrisData; j++)
        {
            printf("%f\t", tab_vecteurs_init[i].vecteurData[j]);
        }
        printf("%s\n", tab_vecteurs_init[i].etiquette);
    }
}

//fonction qui affiche les module de données des vecteurs iris-data
void afficherModuleDonneesVecteurs()
{
    for (int i = 0; i < nbLignesVecteurIrisData; i++)
    {
        printf("Module [%d] = %f\n", i + 1, tab_vecteurs_init[i].module);
    }
}

//remplacement de chaque donnees du vecteur. (donnee j du vecteur/module i du vecteur)
void normaliseVecteurs()
{
    for (int i = 0; i < nbLignesVecteurIrisData; i++)
    {
        for (int j = 0; j < nbColonnesVecteurIrisData; j++)
        {
            tab_vecteurs_init[i].vecteurData[j] = tab_vecteurs_init[i].vecteurData[j] / tab_vecteurs_init[i].module;
        }
    }
}

//fonction permettant le calcul de la moyenne de chaque colonne des vecteurs de données iris-data
void calculMoyColonneVecteurs()
{
    float tempVal = 0;
    for (int j = 0; j < nbColonnesVecteurIrisData; j++)
    {
        for (int i = 0; i < nbLignesVecteurIrisData; i++)
        {
            tempVal += tab_vecteurs_init[i].vecteurData[j];
        }
        if (j == 0)
        {
            moyennesColonnes[j] = tempVal / 150;
        }
        else if (j == 1)
        {
            moyennesColonnes[j] = tempVal / 150;
        }
        else if (j == 2)
        {
            moyennesColonnes[j] = tempVal / 150;
        }
        else if (j == 3)
        {
            moyennesColonnes[j] = tempVal / 150;
        }
        tempVal = 0;
    }

    //printf("Moy1: %f\nMoy2: %f\nMoy3: %f\nMoy4: %f\n\n", moyennesColonnes[0], moyennesColonnes[1], moyennesColonnes[2], moyennesColonnes[3]);
}

//fonction permettant un shuffle des vecteurs de données iris-data
void melangeStructVecteurs()
{
    int randomNumber = 0;
    //struct vecteurIrisData tab_vec_temp;
    for (int i = 0; i < nbLignesVecteurIrisData; i++)
    {
        randomNumber = rand() % nbLignesVecteurIrisData;

        tab_vecteurs_shuffle[i] = tab_vecteurs_init[randomNumber];
    }
}

//fonction qui libère les allocations mémoires éffectués
void libereMemoire()
{
    for (int i = 0; i < nbLignesVecteurIrisData; i++)
    {
        free(tab_vecteurs_init[i].vecteurData);
        free(tab_vecteurs_init[i].etiquette);
    }
    printf("Liberation de la memoire du tableau de vecteurs Iris-Data.\n");

    for (int i = 0; i < nodeConfig.nb_lignes; i++)
    {
        for (int j = 0; j < nodeConfig.nb_colonnes; j++)
        {
            free(matrice[i][j].vecteurData);
        }
        free(matrice[i]);
    }
    free(matrice);

    printf("Liberation de la memoire de la matrice de nodes.\n");
}

//fonction permettant de retourner une valeur de type double random
//entre un min et max passé en paramètre
double RandDouble(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

//fonction qui initialise le reseau de la matrice
//afin de lui allouer la mémoire pour les lignes et colonnes
//avec comme données une valeur aléatoire comprise entre -0.05 et +0.05 autour de la moyenne de chaque colonne des vecteurs iris-data
void configReseauMatrice(int lignes, int colonnes)
{
    matrice = malloc(lignes * sizeof(struct nodeMatrice *));

    nodeConfig.nb_nodes = lignes * colonnes;
    nodeConfig.nb_colonnes = colonnes;
    nodeConfig.nb_lignes = lignes;

    for (int i = 0; i < lignes; i++)
    {
        matrice[i] = malloc(colonnes * sizeof(struct nodeMatrice));
    }

    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            matrice[i][j].vecteurData = (double *)malloc(nbLignesVecteurIrisData * sizeof(double));
            for (int k = 0; k < nbColonnesVecteurIrisData; k++)
            {
                matrice[i][j].vecteurData[k] = RandDouble((moyennesColonnes[k] - 0.05), (moyennesColonnes[k] + 0.05)); //randomNum;
                matrice[i][j].etiquette = "?????????";
            }
        }
    }
    nodeConfig.map = matrice;
}

//fonction qui affiche la matrice avec les vecteurs de données
void afficheMatriceNode(struct nodeMatrice **matrice)
{
    for (int i = 0; i < nodeConfig.nb_lignes; i++)
    {
        printf("Node[%d]\t\t", (i + 1));
        for (int j = 0; j < nodeConfig.nb_colonnes; j++)
        {
            for (int k = 0; k < nbColonnesVecteurIrisData; k++)
            {
                printf("%f|", matrice[i][j].vecteurData[k]);
            }
            printf(" |");
        }
        printf("\n\n");
    }
}

//fonction qui affiche dans le terminal la matrice en couleur suivant le type de donnée (g, s, o)
//permettant de visualiser le résultat de façon claire
void afficheMatriceNodeNom(struct nodeMatrice **matrice)
{
    printf("\033[0;31m");
    printf("g: Iris-virginica || ");
    printf("\033[0;32m");
    printf("s: Iris-versicolor || ");
    printf("\033[0;33m");
    printf("o: Iris-setosa\n");
    printf("\033[0m");
    for (int i = 0; i < nodeConfig.nb_lignes; i++)
    {
        for (int j = 0; j < nodeConfig.nb_colonnes; j++)
        {
            if (!strncmp(&matrice[i][j].etiquette[8], "g", 1))
            {
                printf("\033[0;31m");
            }

            if (!strncmp(&matrice[i][j].etiquette[8], "s", 1))
            {
                printf("\033[0;32m");
            }

            if (!strncmp(&matrice[i][j].etiquette[8], "o", 1))
            {
                printf("\033[0;33m");
            }

            printf("%c", matrice[i][j].etiquette[8]);
            printf("\033[0m");
            printf("\t| ");
        }
        printf("\n");
    }
}

//fonction permettant d'afficher la liste de tout les BMU pendant l'itération en cours
void afficheBmu()
{
    struct bmu *actuel = nodeConfig.bmu;
    while (actuel != NULL)
    {
        printf("BMU | Ligne : %d | Colonne : %d | Distance : %f \n", actuel->ligne, actuel->colonne, nodeConfig.map[actuel->ligne][actuel->colonne].distance);
        actuel = actuel->next;
    }
}

/*
//fonction permettant de trouver le degre de voisinage afin d'avoir plus entre 30% et 50% de voisin à modifier 
int trouveDegreVoisinage(){
    //int compteurVecteursIrisData){
    int nbVoisins = 0;
    int degreVoisin = 1;

    while (nbVoisins <= (nodeConfig.nb_nodes / 2))
    {
        for (int i = (nodeConfig.bmu->ligne - degreVoisin); i <= (nodeConfig.bmu->ligne + degreVoisin); i++)
        {
            for (int j = (nodeConfig.bmu->colonne - degreVoisin); j <= (nodeConfig.bmu->colonne + degreVoisin); j++)
            {

                if ((i >= 0 && i < nodeConfig.nb_lignes) && (j >= 0 && j < nodeConfig.nb_colonnes))
                {
                    nbVoisins += 1;
                }
            }

            //printf("degre: %d\n", degreVoisin);
            //printf("nbVoisins: %d\n", nbVoisins);
        }
        if (nbVoisins < (nodeConfig.nb_nodes / 2))
        {
            nbVoisins = 0;
        }
        degreVoisin += 1;
    }
    return degreVoisin;
}
*/

//sélectionne un BMU random entre 1 et nombre total de BMU pendant l'itération en cours
//et met à jour les données des vecteurs du bmu ainsi que de son voisinage suivant le nombre d'itération réalisé
//avec une valeur alpha de 0.7 pour les 500 premières itérations à 0.07 pour les 1500 dernières itérations (10%)
void modifVoisinnage(int degreVoisin, int numVecteurIrisData, int nbTourIteration)
{
    float alpha;

    struct bmu *bmuTemp = nodeConfig.bmu;
    struct bmu *bmuFinal;
    int nbBMU = bmuTemp->nbBMU;

    int randomBMU = (rand() % (nbBMU - 1 + 1)) + 1; //valeur random entre 1 et le nombre de BMU dans l'itération en cours

    int x = 1;

    //permet de récupérer un bmu aléatoire de la liste chainée
    while (bmuTemp != NULL)
    {
        if (x == randomBMU)
        {
            bmuFinal = bmuTemp;
            break;
        }
        x++;
        bmuTemp = bmuTemp->next;
    }

    if (nbTourIteration <= 500)
    {
        alpha = 0.7 * (1 - nbTourIteration / 2000);
    }
    else
    {
        alpha = 0.07 * (1 - nbTourIteration / 2000);
    }

    int ligneBMU = bmuFinal->ligne;
    int colonneBMU = bmuFinal->colonne;

    nodeConfig.map[ligneBMU][colonneBMU].etiquette = tab_vecteurs_shuffle[numVecteurIrisData].etiquette;

    for (int i = (ligneBMU - degreVoisin); i <= (ligneBMU + degreVoisin); i++)
    {
        for (int j = (colonneBMU - degreVoisin); j <= (colonneBMU + degreVoisin); j++)
        {
            if ((i >= 0 && i < nodeConfig.nb_lignes) && (j >= 0 && j < nodeConfig.nb_colonnes))
            {
                for (int k = 0; k < nbColonnesVecteurIrisData; k++)
                {
                    nodeConfig.map[i][j].vecteurData[k] = nodeConfig.map[i][j].vecteurData[k] + (alpha * (tab_vecteurs_shuffle[numVecteurIrisData].vecteurData[k] - nodeConfig.map[i][j].vecteurData[k]));
                }
            }
        }
    }
}

//fonction qui calcule la distance de chaque vecteurs de nodes de la matrice, vérifie la présence d'un ou plusieurs BMU
//et qui appelle une autre fonction modifiant le BMU ainsi que son voisinage
void calculDistanceNodeVecteur()
{
    cell = (struct bmu *)malloc(sizeof(struct bmu));
    tete = cell;
    cell->colonne = 0;
    cell->ligne = 0;
    cell->nbBMU = 0;
    nodeConfig.bmu = tete;

    double tempDist = 0;
    int nbTourIteration = 1;
    int degreVoisin = 3;
    int x = ((int)(500 / 3)); //166

    while (nbTourIteration <= 2000)
    {
        for (int i = 0; i < nbLignesVecteurIrisData; i++)
        { //tab_vecteurs
            for (int j = 0; j < nodeConfig.nb_lignes; j++)
            { //matrice[j]
                for (int k = 0; k < nodeConfig.nb_colonnes; k++)
                { //matrice[j][k]
                    for (int u = 0; u < nbColonnesVecteurIrisData; u++)
                    {
                        tempDist += pow((tab_vecteurs_shuffle[i].vecteurData[u]) - (nodeConfig.map[j][k].vecteurData[u]), 2);
                    }
                    nodeConfig.map[j][k].distance = sqrt(tempDist);
                    tempDist = 0;

                    if (nodeConfig.map[j][k].distance == nodeConfig.map[cell->ligne][cell->colonne].distance)
                    {
                        cell->next = (struct bmu *)malloc(sizeof(struct bmu));
                        cell->next->ligne = j;
                        cell->next->colonne = k;
                        cell->nbBMU += 1;
                        cell->next->next = NULL;

                        cell = cell->next;
                    }
                    else if (nodeConfig.map[j][k].distance < nodeConfig.map[cell->ligne][cell->colonne].distance)
                    {
                        cell = (struct bmu *)malloc(sizeof(struct bmu));
                        tete = cell;
                        cell->ligne = j;
                        cell->colonne = k;
                        cell->nbBMU = 1;
                        nodeConfig.bmu = tete;
                    }
                }
            }

            //diminue le degre de voisinnage toute les 500/3 itérations
            //passant d'un degré de 3 à 1 minimum
            if (degreVoisin > 1)
            {
                if (nbTourIteration == x)
                {
                    degreVoisin -= 1;
                    x = x + ((int)(500 / 3));
                }
            }

            //modifie le BMU et son voisinnage
            modifVoisinnage(degreVoisin, i, nbTourIteration);
        }

        //fonction de shuffle des vecteurs iris-data
        melangeStructVecteurs();

        //affiche la matrice couleur toutes les 500 itérations
        if ((nbTourIteration % 500) == 0 || nbTourIteration == 1)
        {
            printf("\nMatrice a l iteration numero %d\n", nbTourIteration);
            afficheMatriceNodeNom(nodeConfig.map);
        }
        nbTourIteration += 1;
    }
}

//fonction qui affiche la distance calculé pour chaque vecteurs de nodes
void afficheDistanceNodeVecteur()
{
    for (int i = 0; i < nodeConfig.nb_lignes; i++)
    { //tab_vecteurs
        printf("Distance[%d]", i);
        for (int j = 0; j < nodeConfig.nb_colonnes; j++)
        { //matrice[j]
            printf("[%d]:%f\t", j, nodeConfig.map[i][j].distance);
        }
        printf("\n");
    }
}

int main()
{
    FILE *fichier = fopen("iris.data", "r");
    srand(time(NULL));
    convertIrisDataToStruct(fichier);

    normaliseVecteurs();
    melangeStructVecteurs();

    calculMoyColonneVecteurs();
    configReseauMatrice(10, 6);

    calculDistanceNodeVecteur();

    libereMemoire();
    printf("\nFin du programme\n");

    return 0;
}