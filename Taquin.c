#define _CRT_SECURE_NO_WARNINGS
// Inclusion des bibliothèques utiles
#include "taquin.h"
#include "AStar.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>



// Fonction pour copier un plateau de taquin pSrc vers pDest
int copyTaquin(Taquin * pSrc, Taquin * pDest)
{
	//printf("copie\n"); //test

	int i, j;

	if (pSrc == NULL)//error
	{
		printf("error pTaquin n'existe pas\n");
		return -1;
	}

	if (pDest == NULL)//error
	{
		printf("error pTaquin n'existe pas\n");
		return -1;
	}

	if (pSrc != NULL)//si le taquin de départ existe
	{

		

		pDest->hauteur = pSrc->hauteur;//attribution de la hauteur, largeur du plateau Src au plateau Des 
		pDest->largeur = pSrc->largeur;

		for (i = 0; i < pDest->hauteur; i++) //on attribut a chaque cases du plateau Dest la valeur des cases du plateau Src
		{
			for (j = 0; j < pDest->largeur; j++)
			{
				pDest->plateau[i][j] = pSrc->plateau[i][j];
				//test début
				//printf("%2d " , pDest->plateau[i][j]);

			}
		}

		pDest->x = pSrc->x;//on attribut les coordonnées de la case vide
		pDest->y = pSrc->y;

	}
	
	return 1;
}

// fonction qui renvoie 1 si les 2 taquins sont identiques
// 0 sinon
// -1 si il y a une erreur au passage des paramètres
int equalTaquin(Taquin * pTaquin1, Taquin * pTaquin2)
{
	//printf("egalite\n");//test


	if (!pTaquin1)//error
	{
		printf("error pTaquin1 n'existe pas\n");
		return 0;
	}
	if (!pTaquin2)//error
	{
		printf("error pTaquin2 n'existe pas\n");
		return -1;
	}
	if ((pTaquin1->hauteur != pTaquin2->hauteur) || (pTaquin1->largeur != pTaquin2->largeur))
		return -1;

	int CaseValide = 0;

	if (pTaquin1 != NULL && pTaquin2 != NULL)
	{
		for (int i = 0; i < pTaquin1->hauteur; i++)
		{
			for (int j = 0; j < pTaquin2->largeur; j++)
			{
				if (pTaquin1->plateau[i][j] == pTaquin2->plateau[i][j])
					CaseValide++;
			}
		}

	}

	if (CaseValide == pTaquin1->hauteur * pTaquin1->largeur)// si le nombre de case valide est égaux a toute les case du taquin alors elle sont egaux
	{
		printf("les 2 taquins sont egaux\n");//test
			return 1;
	}
			
	
	printf("les 2 taquins ne sont pas egaux\n");//test
	
	return 0;
}

// Fonction qui crée un plateau de taquin 
// ATTENTION le plateau doit être NULL avant l'appel à cette fonction 
// Si le plateau n'est pas NULL la fonction essayera de libérer la zone mémoire occupée par le plateau et cela peut donc aboutir à un crash si le plateau n'a pas été initialisé
int createTaquin(Taquin * pTaquin, unsigned char hauteur, unsigned char largeur)
{
	// Test pour vérifier que les données passées ne sont pas corrompues
	if(!pTaquin) return 0;
	
	// Tests pour vérifier les paramètres de taille de taquin
	if(hauteur<SIZE_MINI) hauteur = SIZE_MINI;
	if(hauteur>SIZE_MAXI) hauteur = SIZE_MAXI;
	if(largeur<SIZE_MINI) largeur = SIZE_MINI;
	if(largeur>SIZE_MAXI) largeur = SIZE_MAXI;
	
	{
		int i;

		// On vérifie que le plateau n'existe pas
		// S'il existe on libère la mémoire avant de recréer le plateau
		if(pTaquin->plateau)
		{
			// On libère le plateau ligne par ligne
			for(i=0; i < pTaquin->hauteur; i++) if(pTaquin->plateau[i]) free(pTaquin->plateau[i]);
			// On libère le tableau qui stockait les lignes
			free(pTaquin->plateau);
			pTaquin->plateau = NULL;
		}

		pTaquin->hauteur = hauteur;
		pTaquin->largeur = largeur;

		// on alloue la zone mémoire pour stocker les adresses des lignes du tableau
		pTaquin->plateau = (unsigned char**) malloc(sizeof(unsigned char*)*hauteur);
		
		// si on a pas réussi à allouer la zone mémoire on retourne 0
		if(!pTaquin->plateau) return 0;

		for(i=0; i < hauteur; i++) pTaquin->plateau[i] = NULL;

		for(i=0; i < hauteur; i++)
		{
			// On alloue la zone mémoire pour contenir la ligne i
			pTaquin->plateau[i] = (unsigned char*) malloc(sizeof(unsigned char)*largeur);
			// S'il y a eu un souci à l'allocation on libère tout ce qui a déjàà été alloué et on retourne 0
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
	//printf("init\n");//test

	if (!pTaquin)//error
	{
		printf("error pTaquin n'existe pas\n");
		return -1;
	}

	for (int i = 0; i < pTaquin->hauteur; i++)
	{
		for (int j = 0; j < pTaquin->largeur; j++)
		{
			pTaquin->plateau[i][j] = pTaquin->largeur * i + j; //pour chaque case on attribut un identifiant de 0 a 8.
			//printf("%2d ", pTaquin->plateau[i][j]);
		}
	}
	
	return 1;
}

// Fonction qui mélange le taquin en effectuant entre minRandom et maxRandom coups aléatoires
int mixTaquin(Taquin * pTaquin, int minRandom, int maxRandom)
{
	//printf("MixTaquin");//test

	if (!pTaquin)//error
	{
		printf("error pTaquin n'existe pas\n");
		return -1;
	}

	int mix, D, i, valide;
	deplacement d;
	srand(time(NULL));
	mix = rand() % ((1 + maxRandom) - minRandom) + minRandom; // ajout d'aléatoire
	for (i = 0; i < mix; i = i + valide)
	{
		D = rand() % 4; //D est obtient une valeur entre 0 et 3 afin de générer un déplacement aléatoire
		if (D == 0)
			d = HAUT;
		else if (D == 1)
			d = DROITE;
		else if (D == 2)
			d = BAS;
		else
			d = GAUCHE;
		valide = moveTaquin(pTaquin, d);
		//printf("valide = %d \n", valide); // affiche si la case est valide
		//printf("i = %d \n", i); //affiche le compteur

	}

	//printf("mix = %d \n", mix);//affiche le nb de mélange
	
	return 1;
}

// Fonction qui permet de bouger une pièce du taquin (en bougeant la case vide)
int moveTaquin(Taquin * pTaquin, deplacement d)
{

	if (!pTaquin)//error
	{
		printf("error pTaquin n'existe pas\n");
		return -1;
	}

	//printf("moveTaquin\n");

	unsigned char Tmp = 0;//case temporaire
	
	if (d == HAUT)
	{
		if (pTaquin->x - 1 < 0) // test d'erreur
		{
			//printf("impossibe de desplacer vers le haut\n");
			return 0;
		}

		Tmp = pTaquin->plateau[pTaquin->x - 1][pTaquin->y];//la case du haut
		pTaquin->plateau[pTaquin->x-1][pTaquin->y] = 0;//la case de haut devient la case vide
		pTaquin->plateau[pTaquin->x][pTaquin->y] = Tmp;//la case vide devient la case du haut
		
		pTaquin->x--;

	}

	if (d == BAS)
	{
		if (pTaquin->x + 1 >= pTaquin->hauteur) // test d'erreur
		{
			//printf("impossibe de deplacer vers le bas\n");
			return 0;
		}

		Tmp = pTaquin->plateau[pTaquin->x + 1][pTaquin->y];//la case du bas
		pTaquin->plateau[pTaquin->x + 1][pTaquin->y] = 0;//la case de bas devient la case vide
		pTaquin->plateau[pTaquin->x][pTaquin->y] = Tmp;//la case vide devient la case du bas

		pTaquin->x++;
	}

	if (d == GAUCHE)
	{
		if (pTaquin->y - 1 < 0) // test d'erreur
		{
			//printf("impossibe de deplacer vers la gauche\n");
			return 0;
		}

		Tmp = pTaquin->plateau[pTaquin->x][pTaquin->y-1];//la case de gauche
		pTaquin->plateau[pTaquin->x][pTaquin->y-1] = 0;//la case de gauche devient la case vide
		pTaquin->plateau[pTaquin->x][pTaquin->y] = Tmp;//la case vide devient la case de gauche

		pTaquin->y--;
	}
	
	if (d == DROITE)
	{
		if (pTaquin->y + 1 >= pTaquin->largeur) // test d'erreur
		{
			//printf("impossibe de deplacer vers la droite\n");
			return 0;
		}


		Tmp = pTaquin->plateau[pTaquin->x][pTaquin->y+1];//la case de droite
		pTaquin->plateau[pTaquin->x][pTaquin->y+1] = 0;//la case de droite devient la case vide
		pTaquin->plateau[pTaquin->x][pTaquin->y] = Tmp;//la case vide devient la case de droite

		pTaquin->y++;
	}

	
	return 1;
}

// Fonction qui renvoie le déplacement à effectuer pour annuler le déplacement donné en paramètre
deplacement cancelMove(deplacement d)
{
	switch (d)
	{
	case GAUCHE:
		return DROITE;
	case DROITE:
		return GAUCHE;
	case BAS:
		return HAUT;
	case HAUT:
		return BAS;
	}

	return AUCUN;
}

// Fonction qui renvoie 1 si le taquin es résolu, 0 sinon
int endTaquin(Taquin * pTaquin)
{
	if (pTaquin == NULL)//error
	{
		printf("error pTaquin n'existe pas\n");
		return 0;
	}


	char solution = 0;
	int nbbonneCase = 0;
	for (int i = 0; i < pTaquin->hauteur; i++)
	{
		for (int j = 0; j < pTaquin->largeur; j++)
		{
			if (pTaquin->plateau[i][j] == solution) //si la case parcouru est la bonne case alors la variable nbbonneCase s'incrémente de 1.
				nbbonneCase++;

			solution++;// la case solution s'incrémente (solution 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15)
		}
	}

	if (nbbonneCase == pTaquin->hauteur*pTaquin->largeur)//si le nombre de bonne case est égale a toute les cases du taquin alors le taquin est résolu
	{
		printf("\nBRAVO VOUS AVEZ RESOLU LE TAQUIN\n");
		return 1;
	}

	return 0;
}

// fonction d'affichage du taquin
int displayTaquin(Taquin * pTaquin, int offset)
{
	if (pTaquin == NULL)//error
	{
		printf("error pTaquin n'existe pas\n");
		return 0;
	}
		

	for (int i = 0; i < pTaquin->largeur; i++) 
	{
		for (int j = 0; j < pTaquin->hauteur; j++)
		{
			printf("%2d ", pTaquin->plateau[i][j]);//on affiche le tablea
		}
		printf("\n");
	}

	return 1;
}

// Fonction pour libérer les zones mémoires occupées par un taquin
int freeTaquin(Taquin * pTaquin)
{

	return 1;
}
//Boucle de jeu


void viderBuffer()
{
	int c = 0;
	while (c != '\n' && c != EOF)
	{
		c = getchar();
	}
}

int gameLoop(int hauteur, int largeur, int minRandom, int maxRandom)
{

	char keyword = 0;
	deplacement d = AUCUN;
	int fin = 0;//fin = 0, si fin est vraiment la fin alors fin est a 1
	int offset = 100;

	printf("\n");
	//création du premier taquin
	Taquin * pTaquin = (Taquin*)malloc(sizeof(Taquin));//creation taquin 1
	pTaquin->plateau = NULL;
	pTaquin->hauteur = hauteur;
	pTaquin->largeur = largeur;
	pTaquin->x = 0;
	pTaquin->y = 0;

	createTaquin(pTaquin, hauteur, largeur);

	Taquin * pTaquin2 = (Taquin*)malloc(sizeof(Taquin));//creation taquin 2
	pTaquin2->plateau = NULL;
	pTaquin2->hauteur = hauteur;
	pTaquin2->largeur = largeur;
	pTaquin2->x = 0;
	pTaquin2->y = 0;

	createTaquin(pTaquin2, hauteur, largeur);

	copyTaquin(pTaquin, pTaquin2); //test de copie
	equalTaquin(pTaquin, pTaquin2);//test de equal

	printf("TAKIN 1 : \n");
	displayTaquin(pTaquin, offset);//test display


	mixTaquin(pTaquin, minRandom, maxRandom);//mélange du takin
	

	// BOUCLE DE JEU ! A DEVELOPPER
	while (fin != 1)
	{
		
		keyword = 0;
				
		scanf("%c", &keyword);//action
		viderBuffer();

		if (keyword == 'z')
			d = HAUT;

		if (keyword == 's')
			d = BAS;

		if (keyword == 'q')
			d = GAUCHE;

		if (keyword == 'd')
			d = DROITE;

		if (keyword == 'w')
			d = cancelMove(d);
			
		//system("cls");

		moveTaquin(pTaquin, d);

		displayTaquin(pTaquin,100);//test display
		fin = endTaquin(pTaquin);

		

	}
	return 1;

	

}

