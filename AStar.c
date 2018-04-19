#include "AStar.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "SDL.h"
//#include "SDL_image.h"



// fonction pour créer (allouer) un noeud de liste et l'initialiser avec le taquin passé en paramètre
ptrListAStar createNodeList(Taquin * pTaquin, int gValue, int fValue, deplacement d, ptrListAStar pPrevPlay)
{
	if (!pTaquin)
	{
		return NULL;
	}
	ptrListAStar Anode = (ptrListAStar)malloc(sizeof(ListAStar));
	Anode->DfromStart = gValue;
	Anode->Heur = fValue;
	Anode->Current = pTaquin;
	Anode->LastD = d;
	Anode->LastPlay = pPrevPlay;
	Anode->LastNode = NULL;
	Anode->NextNode = NULL;
	return Anode;
}

int fpg(ptrListAStar called)
{
	if (!called)
	{
		return INT_MAX;
	}
	return (called->DfromStart + called->Heur);
}

// Insertion dans la liste de façon triée ou non (F+G)
// si on passe le paramètre tri à 0, on insère en tête de liste
int insertList(ptrListAStar * ppHead, ptrListAStar pNode, int tri)
{
	if (!ppHead)
	{
		return -1;
	}
	if (!pNode)
	{
		return -1;
	}
	if (!(*ppHead))
	{
		(*ppHead) = pNode;
		return 0;
	}
	int st = 1;
	ptrListAStar tmp = (*ppHead);
	if (tri == 0) //insert en tête
	{
		pNode->NextNode = (*ppHead);
		(*ppHead)->LastNode = pNode;
		(*ppHead) = pNode;
	}
	else //insert selon F+G
	{
		if (fpg(pNode) < fpg((*ppHead)))
		{
			if (!(*ppHead)->LastNode)
			{
			}
			else
			{
				pNode->LastNode = (*ppHead)->LastNode;
				(*ppHead)->LastNode->NextNode = pNode;
			}
			pNode->NextNode = (*ppHead);
			(*ppHead)->LastNode = pNode;
			(*ppHead) = pNode;
			st = 0;
		}
		else
		{
			if (!(*ppHead)->NextNode)
			{
				(*ppHead)->NextNode = pNode;
				pNode->LastNode = (*ppHead);
			}
			else
			{
				while (fpg(pNode) >= fpg(tmp->NextNode))
				{
					tmp = tmp->NextNode;
				}
				pNode->LastNode = tmp;
				tmp->NextNode = pNode;
			}
		}
		
	}
	(*ppHead)->LastNode = NULL;
	return 0;
}

// Fonction pour prélever le noeud en tête de liste
// Retourne le noeud prélevé
ptrListAStar popList(ptrListAStar * ppHead)
{
	if (!ppHead)
	{
		return NULL;
	}
	if (!(*ppHead))
	{
		return NULL;
	}
	ptrListAStar poped = (ptrListAStar)malloc(sizeof(ListAStar));
	poped->Current = (*ppHead)->Current;
	poped->DfromStart = (*ppHead)->DfromStart;
	poped->Heur = (*ppHead)->Heur;
	poped->LastD = (*ppHead)->LastD;
	poped->LastNode = (*ppHead)->LastNode;
	poped->NextNode = (*ppHead)->NextNode;
	if (!(*ppHead)->NextNode)
	{
		free((*ppHead));
		(*ppHead) = NULL;
	}
	else
	{
		(*ppHead) = (*ppHead)->NextNode;
		free((*ppHead)->LastNode);
		(*ppHead)->LastNode = NULL;
	}
	return poped;
}

// fonction qui retourne le noeud dans lequel on trouve le taquin passé en paramètre (pointeur sur le pointeur dans la liste)

ptrListAStar * isInList(ptrListAStar * ppHead, Taquin * pTaquin)
{
	if (!ppHead)
	{
		return NULL;
	}
	if (!(*ppHead))
	{
		return NULL;
	}
	if (!pTaquin)
	{
		return NULL;
	}
	ptrListAStar tmp = (ptrListAStar)malloc(sizeof(ListAStar));
	tmp->Current = (*ppHead)->Current;
	tmp->DfromStart = (*ppHead)->DfromStart;
	tmp->Heur = (*ppHead)->Heur;
	tmp->LastD = (*ppHead)->LastD;
	tmp->LastNode = (*ppHead)->LastNode;
	tmp->NextNode = (*ppHead)->NextNode;
	while (tmp->NextNode != NULL)
	{
		if (equalTaquin(tmp->Current, pTaquin) == 1)
		{
			return &tmp;
		}
		else
		{
			tmp = tmp->NextNode;
		}
	}
	return NULL;
}

// fonction pour afficher une liste
// si on met displayFGH à 0 les valeur de F, G et H ne sont pas affichées
int displayList(ptrListAStar pHead, int displayFGH)
{
	int error;
	if (!pHead)
	{
		return -1;
	}
	if (displayFGH)
	{
		printf("F = %d, G = %d, H = %d\n", pHead->DfromStart, pHead->Heur, pHead->DfromStart + pHead->Heur);
	}
	error = displayTaquin(pHead->Current, 0);
	if (!(pHead->NextNode))
	{
		return 0;
	}
	return displayList(pHead->NextNode, displayFGH);
}

// Fonction pour résoudre le taquin en utilisant l'algorithme A*
// La fonction prend comme taquin initial le taquin spécifié par pTaquin
// elle remplit 
//   - pTabDeplacement (un tableau contenant les déplacements à effectuer pour résoudre le taquin)
//   - pNbDeplacements
//   - pNbTaquinsGeneres
//   - pTimeElapsed
// Si stepByStep est différent de 0 on affiche dans la console toutes les étapes de la résolution
// pWindow
int solveTaquin(Taquin *pTaquin, deplacement ** pTabDeplacement, unsigned long *pNbDeplacements, unsigned long * pNbTaquinsGeneres, unsigned long * pTimeElapsed, int stepByStep, SDL_Surface * pWindow)
{
	//TODO : Faire le choix du prochain noeud.
	if ((!pTaquin) || (!pTabDeplacement) || (!(*pTabDeplacement)) || (!pNbDeplacements) || (!pNbTaquinsGeneres) || (!pTimeElapsed))
	{
		return -1;
	}
	int verifier[4] = { 0,0,0,0 };
	int g = 0;
	int mana = INT_MAX, off = 0;
	ptrListAStar HeadOpen = createNodeList(pTaquin, g, h(pTaquin), AUCUN, NULL); //taquin actuel;
	ptrListAStar HeadClose = NULL;
	while (mana != 0)
	{
		ptrListAStar Playing = createNodeList(HeadOpen->Current, HeadOpen->DfromStart, HeadOpen->Heur, HeadOpen->LastD, HeadOpen->LastNode);
		ptrListAStar Close = popList(&HeadOpen);
		insertList(&HeadClose, Close, 0);
		mana = h(pTaquin);
		//off = displayTaquin(pTaquin, 0);
		/*printf("openlist precopy:\n");
		off = displayList(HeadOpen, 1);
		system("pause");
		copyTaquin(pTaquin, pTaquinD);
		copyTaquin(pTaquin, pTaquinG);
		copyTaquin(pTaquin, pTaquinS);
		copyTaquin(pTaquin, pTaquinN);
		printf("openlist postcopy:\n");
		off = displayList(HeadOpen, 1);
		system("pause");*/
		///////////////////////////////////////////////////////////////////////////////
		Taquin * pTaquinN = (Taquin*)malloc(sizeof(Taquin));//creation taquin Haut
		pTaquinN->plateau = NULL;
		pTaquinN->hauteur = pTaquin->hauteur;
		pTaquinN->largeur = pTaquin->largeur;
		pTaquinN->x = 0;
		pTaquinN->y = 0;
		createTaquin(pTaquinN, pTaquinN->hauteur, pTaquinN->largeur);
		copyTaquin(pTaquin, pTaquinN);

		Taquin * pTaquinS = (Taquin*)malloc(sizeof(Taquin));//creation taquin Bas
		pTaquinS->plateau = NULL;
		pTaquinS->hauteur = pTaquin->hauteur;
		pTaquinS->largeur = pTaquin->largeur;
		pTaquinS->x = 0;
		pTaquinS->y = 0;
		createTaquin(pTaquinS, pTaquinS->hauteur, pTaquinS->largeur);
		copyTaquin(pTaquin, pTaquinS);

		Taquin * pTaquinG = (Taquin*)malloc(sizeof(Taquin));//creation taquin Gauche
		pTaquinG->plateau = NULL;
		pTaquinG->hauteur = pTaquin->hauteur;
		pTaquinG->largeur = pTaquin->largeur;
		pTaquinG->x = 0;
		pTaquinG->y = 0;
		createTaquin(pTaquinG, pTaquinG->hauteur, pTaquinG->largeur);
		copyTaquin(pTaquin, pTaquinG);

		Taquin * pTaquinD = (Taquin*)malloc(sizeof(Taquin));//creation taquin Droite
		pTaquinD->plateau = NULL;
		pTaquinD->hauteur = pTaquin->hauteur;
		pTaquinD->largeur = pTaquin->largeur;
		pTaquinD->x = 0;
		pTaquinD->y = 0;
		createTaquin(pTaquinD, pTaquinD->hauteur, pTaquinD->largeur);
		copyTaquin(pTaquin, pTaquinD);
		////////////////////////////////////////////////////////////////////////////////

		g++;
		//Haut
		verifier[0] = moveTaquin(pTaquinN, HAUT);
		if (verifier[0] == 1)
		{
			ptrListAStar * HandlerO = isInList(&HeadOpen, pTaquinN);
			ptrListAStar * HandlerC = isInList(&HeadClose, pTaquinN);
			if (!HandlerO)
			{
				if (!HandlerC)
				{
					ptrListAStar Haut = createNodeList(pTaquinN, g, h(pTaquinN), HAUT, Playing);
					insertList(&HeadOpen, Haut, 1);
				}
			}
			HandlerC = NULL;
			HandlerO = NULL;
		}
		//Droite
		verifier[1] = moveTaquin(pTaquinD, DROITE);
		if (verifier[1] == 1)
		{
			ptrListAStar * HandlerO = isInList(&HeadOpen, pTaquinD);
			ptrListAStar * HandlerC = isInList(&HeadClose, pTaquinD);
			if (!HandlerO)
			{
				if (!HandlerC)
				{
					ptrListAStar Droite = createNodeList(pTaquinD, g, h(pTaquinD), DROITE, Playing);
					insertList(&HeadOpen, Droite, 1);
				}
			}
			HandlerC = NULL;
			HandlerO = NULL;
		}
		//Bas
		verifier[2] = moveTaquin(pTaquinS, BAS);
		if (verifier[2] == 1)
		{
			ptrListAStar * HandlerO = isInList(&HeadOpen, pTaquinS);
			ptrListAStar * HandlerC = isInList(&HeadClose, pTaquinS);
			if (!HandlerO)
			{
				if (!HandlerC)
				{
					ptrListAStar Bas = createNodeList(pTaquinS, g, h(pTaquinS), BAS, Playing);
					insertList(&HeadOpen, Bas, 1);
				}
			}
			HandlerC = NULL;
			HandlerO = NULL;
		}
		//Gauche
		verifier[3] = moveTaquin(pTaquinG, GAUCHE);
		if (verifier[3] == 1)
		{
			ptrListAStar * HandlerO = isInList(&HeadOpen, pTaquinG);
			ptrListAStar * HandlerC = isInList(&HeadClose, pTaquinG);
			if (!HandlerO)
			{
				if (!HandlerC)
				{
					ptrListAStar Gauche = createNodeList(pTaquinG, g, h(pTaquinG), GAUCHE, Playing);
					insertList(&HeadOpen, Gauche, 1);
				}
			}
			HandlerC = NULL;
			HandlerO = NULL;
		}
		//free(Playing);
		pTaquin = HeadOpen->Current;
		//(*pTabDeplacement)[g - 1] = HeadOpen->LastD;
		(*pNbDeplacements)++;
		(*pNbTaquinsGeneres) += 4;
		if (stepByStep != 0)
		{
			//off = displayTaquin(pTaquin, off);
			//printf("\n");
			//printf("openlist :\n");
			//off = displayList(HeadOpen, 1);
			//printf("\ncloselist :\n");
			//off = displayList(HeadClose, 1);
			//system("pause");
			//system("cls");
		}
		if (mana == 0)
		{
			system("pause");
			break;
		}
	}
	return g;
}

// fonction d'évaluation pour la résolution avec AStar
int h(Taquin * pTaquin)
{
	if (!pTaquin)
	{
		return INT_MAX;
	}
	int i, j, mana, th, tl, dx, dy;
	mana = 0;
	for (i = 0; i < pTaquin->hauteur; i++)
	{
		for (j = 0; j < pTaquin->largeur; j++)
		{
			th = pTaquin->plateau[i][j] / pTaquin->largeur;
			tl = pTaquin->plateau[i][j] % pTaquin->largeur;
			dx = abs(i - th);
			dy = abs(j - tl);
			mana += dx + dy;
		}
	}
	return mana;
}