// Inclusion des biblioth�ques utiles
#include "taquin.h"
#include "AStar.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>



// Fonction pour copier un plateau de taquin pSrc vers pDest
int copyTaquin(Taquin * pSrc, Taquin * pDest)
{
	int i, j;

	if (pSrc != NULL)//si le taquin de d�part existe
	{

		printf("\n copie "); //

		pDest->hauteur = pSrc->hauteur;//attribution de la hauteur, largeur du plateau Src au plateau Des 
		pDest->largeur = pSrc->largeur;

		for (i = 0; i < pDest->hauteur; i++) //on attribut a chaque cases du plateau Dest la valeur des cases du plateau Src
		{
			for (j = 0; j < pDest->largeur; j++)
			{
				pDest->plateau[i][j] = pSrc->plateau[i][j];
				//test d�but
				printf("%d " , pDest->plateau[i][j]);
				//test fin

			}
		}

		pDest->x = pSrc->x;//on attribut les coordonn�es de la case vide
		pDest->y = pSrc->y;

	}
	
	return 1;
}

// fonction qui renvoie 1 si les 2 taquins sont identiques
// 0 sinon
// -1 si il y a une erreur au passage des param�tres
int equalTaquin(Taquin * pTaquin1, Taquin * pTaquin2)
{
	printf("\n egalite ");

	int CaseValide = 0;

	if (pTaquin1 == NULL)
		return -1;
	if (pTaquin2 == NULL)
		return -1;

	if (pTaquin1 != NULL && pTaquin2 != NULL)
	{
		for (int i = 0; i < pTaquin1->hauteur; i++)
		{
			for (int j = 0; j < pTaquin2->largeur; j++)
			{
				if (pTaquin1->plateau[i][j] == pTaquin1->plateau[i][j])
					CaseValide++;
			}
		}

	}

	if (CaseValide == pTaquin1->hauteur * pTaquin1->largeur)
	{
		printf("les 2 taquins sont egaux");//test
			return 1;
	}
			
	

	
	return 0;
}

// Fonction qui cr�e un plateau de taquin 
// ATTENTION le plateau doit �tre NULL avant l'appel � cette fonction 
// Si le plateau n'est pas NULL la fonction essayera de lib�rer la zone m�moire occup�e par le plateau et cela peut donc aboutir � un crash si le plateau n'a pas �t� initialis�
int createTaquin(Taquin * pTaquin, unsigned char hauteur, unsigned char largeur)
{
	// Test pour v�rifier que les donn�es pass�es ne sont pas corrompues
	if(!pTaquin) return 0;
	
	// Tests pour v�rifier les param�tres de taille de taquin
	if(hauteur<SIZE_MINI) hauteur = SIZE_MINI;
	if(hauteur>SIZE_MAXI) hauteur = SIZE_MAXI;
	if(largeur<SIZE_MINI) largeur = SIZE_MINI;
	if(largeur>SIZE_MAXI) largeur = SIZE_MAXI;
	
	{
		int i;

		// On v�rifie que le plateau n'existe pas
		// S'il existe on lib�re la m�moire avant de recr�er le plateau
		if(pTaquin->plateau)
		{
			// On lib�re le plateau ligne par ligne
			for(i=0; i < pTaquin->hauteur; i++) if(pTaquin->plateau[i]) free(pTaquin->plateau[i]);
			// On lib�re le tableau qui stockait les lignes
			free(pTaquin->plateau);
			pTaquin->plateau = NULL;
		}

		pTaquin->hauteur = hauteur;
		pTaquin->largeur = largeur;

		// on alloue la zone m�moire pour stocker les adresses des lignes du tableau
		pTaquin->plateau = (unsigned char**) malloc(sizeof(unsigned char*)*hauteur);
		
		// si on a pas r�ussi � allouer la zone m�moire on retourne 0
		if(!pTaquin->plateau) return 0;

		for(i=0; i < hauteur; i++) pTaquin->plateau[i] = NULL;

		for(i=0; i < hauteur; i++)
		{
			// On alloue la zone m�moire pour contenir la ligne i
			pTaquin->plateau[i] = (unsigned char*) malloc(sizeof(unsigned char)*largeur);
			// S'il y a eu un souci � l'allocation on lib�re tout ce qui a d�j�� �t� allou� et on retourne 0
			if(!pTaquin->plateau[i])
			{
				freeTaquin(pTaquin);
				return 0;
			}
		}
		// On initialise le taquin
		if(!initTaquin(pTaquin)) return 0;
	}


	return 1;
}


// Fonction qui initialise le taquin
int initTaquin(Taquin * pTaquin)
{
	printf("\n init ");

	for (int i = 0; i < pTaquin->hauteur; i++)
	{
		for (int j = 0; j < pTaquin->largeur; j++)
		{
			pTaquin->plateau[i][j] = pTaquin->largeur * i + j; //pour chaque case on attribut un identifiant de 0 a 8.
			printf("%d ", pTaquin->plateau[i][j]);
		}
	}
	
	return 1;
}

// Fonction qui m�lange le taquin en effectuant entre minRandom et maxRandom coups al�atoires
int mixTaquin(Taquin * pTaquin, int minRandom, int maxRandom)
{
	printf("mix taquin");

	int rand_a_b(int minRandom, int maxRandom);



	return 1;
}

// Fonction qui permet de bouger une pi�ce du taquin (en bougeant la case vide)
int moveTaquin(Taquin * pTaquin, deplacement d)
{
	return 1;
}

// Fonction qui renvoie le d�placement � effectuer pour annuler le d�placement donn� en param�tre
deplacement cancelMove(deplacement d)
{
	return AUCUN;
}

// Fonction qui renvoie 1 si le taquin es r�solu, 0 sinon
int endTaquin(Taquin * pTaquin)
{
	return 0;
}

// fonction d'affichage du taquin
int displayTaquin(Taquin * pTaquin, int offset)
{
	return 1;
}

// Fonction pour lib�rer les zones m�moires occup�es par un taquin
int freeTaquin(Taquin * pTaquin)
{
	return 1;
}
// Boucle de jeu 
int gameLoop(int hauteur,int largeur,int minRandom, int maxRandom)
{

	// BOUCLE DE JEU ! A DEVELOPPER
	while(!_kbhit())
	{
		printf(".");
	}
	return 1;
}

