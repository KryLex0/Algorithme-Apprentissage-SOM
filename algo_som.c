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

struct vecteurIrisData tab_vecteurs[150];
float moy_col1 = 0;
float moy_col2 = 0;
float moy_col3 = 0;
float moy_col4 = 0;

float moyennesColonnes[4];

//fonction qui prend en paramètre le fichier de données iris-data
//et qui récupère chaque vecteurs afin de les sauvegarder dans un tableau de vecteurs
void convertIrisDataToStruct(FILE *fichier)
{
    int compteur = 0;
    char line[50];
    const char *separators = ",";
    //struct vecteurIrisData vecteur[150];
    float moduleTemp = 0;

    if (fichier != NULL)
    {
        while (fgets(line, 1000, fichier))
        {
            //printf("Vecteur : %d \n",compteur);
            char *strToken = strtok(line, separators);
            tab_vecteurs[compteur].vecteurData = (double *)malloc(4 * sizeof(double));

            for (int i = 0; i < 5; i++)
            {
                if (i < 4)
                {
                    tab_vecteurs[compteur].vecteurData[i] = atof(strToken);

                    moduleTemp += pow(tab_vecteurs[compteur].vecteurData[i], 2);
                }
                else
                {
                    tab_vecteurs[compteur].module = sqrt(moduleTemp);
                    tab_vecteurs[compteur].etiquette = malloc(20 * sizeof(char));
                    strncpy(tab_vecteurs[compteur].etiquette, strToken, 20);
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
    for (int i = 0; i < 150; i++)
    {
        printf("Vecteur: %d\t\t", (i + 1));
        for (int j = 0; j < 4; j++)
        {
            printf("%f\t", tab_vecteurs[i].vecteurData[j]);
        }
        printf("%s\n", tab_vecteurs[i].etiquette);
    }
}

//fonction qui affiche les module de données des vecteurs iris-data
void afficherModuleDonneesVecteurs()
{
    for (int i = 0; i < 150; i++)
    {
        printf("Module [%d] = %f\n", i + 1, tab_vecteurs[i].module);
    }
}

//remplacement de chaque donnees du vecteur. (donnee j du vecteur/module i du vecteur)
void normaliseVecteurs()
{
    for (int i = 0; i < 150; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tab_vecteurs[i].vecteurData[j] = tab_vecteurs[i].vecteurData[j] / tab_vecteurs[i].module;
        }
    }
}

//fonction permettant le calcul de la moyenne de chaque colonne des vecteurs de données iris-data 
void calculMoyColonneVecteurs()
{
    float tempVal = 0;
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 150; i++)
        {
            tempVal += tab_vecteurs[i].vecteurData[j];
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
    struct vecteurIrisData tab_vec_temp;
    for (int i = 0; i < 150; i++)
    {
        randomNumber = rand() % 150;

        tab_vec_temp = tab_vecteurs[i];
        //printf("temp%f|%f|%f|%f\n", tab_vec_temp.vecteurData[0], tab_vec_temp.vecteurData[1], tab_vec_temp.vecteurData[2], tab_vec_temp.vecteurData[3]);

        tab_vecteurs[i] = tab_vecteurs[randomNumber];
        tab_vecteurs[randomNumber] = tab_vec_temp;
    }
}
/*
//fonction qui libère les allocations mémoires éffectués
void libereDonneesVecteurs()
{
    for (int i = 0; i < 150; i++)
    {
        free(tab_vecteurs[i].vecteurData);
        free(tab_vecteurs[i].etiquette);
    }
    free(matrice);
    for (int i = 0; i < 10; i++)
    {
        free(matrice[i]);
        for (int j = 0; j < 6; j++)
        {
            free(matrice[i][j].vecteurData);
        }
    }
    free(cell);
    free(tete);
    free(cell->next);
}
*/

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
            matrice[i][j].vecteurData = (double *)malloc(4 * sizeof(double));
            for (int k = 0; k < 4; k++)
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
            for (int k = 0; k < 4; k++)
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
            if(!strncmp(&matrice[i][j].etiquette[8],"g", 1)){
                printf("\033[0;31m");
            }
            
            if(!strncmp(&matrice[i][j].etiquette[8],"s", 1)){
                printf("\033[0;32m");
            }
            
            if(!strncmp(&matrice[i][j].etiquette[8],"o", 1)){
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

    while (bmuTemp != NULL)
    {
        if (x == randomBMU)
        {
            bmuFinal = bmuTemp;
            //printf("BMU SELECTIONNE[%d][%d], %d\n", bmuFinal->ligne, bmuFinal->colonne, x);
            break;
        }
            x++;
            bmuTemp = bmuTemp->next;
        
    }

/*
    if(nbTourIteration < 5){
        printf("BMU SELECTIONNE[%d][%d], randomBMU: %d (NB BMU: %d)\n", bmuFinal->ligne, bmuFinal->colonne, randomBMU, nodeConfig.bmu->nbBMU);
    }
    */


    if (nbTourIteration <= 500)
    {
        alpha = 0.7 * (1 - nbTourIteration / 2000);
    }
    else
    {
        alpha = 0.07 * (1 - nbTourIteration / 2000);
    }
    //printf("Iteration num:%d|| Alpha: %f\n", nbTourIteration, alpha);
    //printf("Vecteur Numero: %d\n", numVecteurIrisData);

    int ligneBMU = bmuFinal->ligne;//nodeConfig.bmu->ligne;//
    int colonneBMU = bmuFinal->colonne;//nodeConfig.bmu->colonne;//
    
    //printf("[%d]iteration:%d || [%d][%d]\n", numVecteurIrisData, nbTourIteration, ligneBMU, colonneBMU);

    nodeConfig.map[ligneBMU][colonneBMU].etiquette = tab_vecteurs[numVecteurIrisData].etiquette;
    //printf("|| %s|| degre: %d\n", nodeConfig.map[ligneBMU][colonneBMU].etiquette, degreVoisin);
    

    for (int i = (ligneBMU - degreVoisin); i <= (ligneBMU + degreVoisin); i++)  //nodeConfig.bmu->ligne
    {
        for (int j = (colonneBMU - degreVoisin); j <= (colonneBMU + degreVoisin); j++)  //nodeConfig.bmu->colonne 
        {

            //printf("CHANGEMENT %f\n",nodeConfig.map[i][j].vecteurData[0]);

            if ((i >= 0 && i < nodeConfig.nb_lignes) && (j >= 0 && j < nodeConfig.nb_colonnes))
            {
                for (int k = 0; k < 4; k++)
                {
                    nodeConfig.map[i][j].vecteurData[k] = nodeConfig.map[i][j].vecteurData[k] + (alpha * (tab_vecteurs[numVecteurIrisData].vecteurData[k] - nodeConfig.map[i][j].vecteurData[k]));
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

    int nbTourIteration = 1;
    int degreVoisin = 3;
    int x = ((int)(500/3)); //166
    

    while (nbTourIteration <= 2000)
    {
        for (int i = 0; i < 150; i++)
        { //tab_vecteurs
            for (int j = 0; j < nodeConfig.nb_lignes; j++)
            { //matrice[j]
                for (int k = 0; k < nodeConfig.nb_colonnes; k++)
                { //matrice[j][k]
                    nodeConfig.map[j][k].distance = sqrt(
                        pow((tab_vecteurs[i].vecteurData[0]) - (nodeConfig.map[j][k].vecteurData[0]), 2) +
                        pow((tab_vecteurs[i].vecteurData[1]) - (nodeConfig.map[j][k].vecteurData[1]), 2) +
                        pow((tab_vecteurs[i].vecteurData[2]) - (nodeConfig.map[j][k].vecteurData[2]), 2) +
                        pow((tab_vecteurs[i].vecteurData[3]) - (nodeConfig.map[j][k].vecteurData[3]), 2));

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
            if(degreVoisin > 1){
                if(nbTourIteration == x){
                    degreVoisin -= 1;
                    x = x + ((int)(500/3));
                }
            }
            
            //modifie le BMU et son voisinnage
            modifVoisinnage(degreVoisin, i, nbTourIteration);
        }

        //fonction de shuffle des vecteurs iris-data
        melangeStructVecteurs();
        
        //affiche la matrice couleur toutes les 500 itérations
        if ((nbTourIteration%500) == 0 || nbTourIteration == 1)
        {
            printf("\nMatrice a l iteration numero %d\n", nbTourIteration);
            afficheMatriceNodeNom(nodeConfig.map);
        }
        nbTourIteration += 1;
        //printf("[%d]nbBMU: %d\n", nbTourIteration, nodeConfig.bmu->nbBMU);
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
    //afficherModuleDonneesVecteurs();
    //afficherDonneesVecteurs();

    calculMoyColonneVecteurs();
    configReseauMatrice(10, 6);
    //afficheMatriceNodeNom(nodeConfig.map);

    calculDistanceNodeVecteur();
    //afficheDistanceNodeVecteur();
    //afficheBmu();

    //trouveDegreVoisinage();
    //modifVoisinnage(trouveDegreVoisinage(), 0);
    //afficheMatriceNode(matrice);

    //printf("%d\n", trouveDegreVoisinage());

    //libereDonneesVecteurs();
    printf("\nFin du programme\n");
    //melangeStructVecteurs();
    return 0;
}

/*

struct livre *init_bib(int n){
    struct livre * book = malloc(n * sizeof(struct livre));
    for(int i=0; i<n; i++){
        book[i].titre[0] = '\0';
        book[i].prix = 0;
        book[i].code = 0;
    }
    return book;
}

void affiche_bib(int n, struct livre *bib){
    for(int i=0; i<n; i++){
        printf("titre: %s, prix: %d, code: %d\n", bib[i].titre, bib[i].prix, bib[i].code);
    }
}



void echange_livre(int i, int j, struct livre *bib){
    struct livre temp;
    temp = bib[i];
    bib[i] = bib[j];
    bib[j] = temp;
}

int main(void){
    int n = 3;
    struct livre *bib = init_bib(n);
    strcpy(bib[0].titre,"titre1");
    bib[0].prix = 10;
    bib[0].code = 1;

    strcpy(bib[1].titre,"titre2");
    bib[1].prix = 8;
    bib[1].code = 2;

    strcpy(bib[2].titre,"titre3");
    bib[2].prix = 5;
    bib[2].code = 3;

    affiche_bib(n, bib);
    printf("\n");
    echange_livre(0, 2, bib);
    affiche_bib(n, bib);
    return 0;
    
}

*/