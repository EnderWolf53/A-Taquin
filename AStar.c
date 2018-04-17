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
	Anode->LastNode = pPrevPlay;
	Anode->NextNode = NULL;
	return Anode;
}

int fpg(ptrListAStar called)
{
	if (!called)
	{
		return -1;
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
	if (!(*ppHead))
	{
		return -1;
	}
	if (!pNode)
	{
		return -1;
	}
	if (tri == 0) //insert en tête
	{
		pNode->NextNode = (*ppHead);
		(*ppHead)->LastNode = pNode;
	}
	else //insert selon F+G
	{
		if (fpg(pNode) < fpg((*ppHead)))
		{
			pNode->NextNode = (*ppHead);
			(*ppHead)->LastNode = pNode;
		}
		else
		{
			pNode->LastNode = (*ppHead);
			if (!(*ppHead)->NextNode)
			{
				(*ppHead)->NextNode = pNode;
			}
			else if (fpg(pNode) < fpg((*ppHead)->NextNode))
			{
				(*ppHead)->NextNode->LastNode = pNode;
				pNode->NextNode = (*ppHead)->NextNode;
				(*ppHead)->NextNode = pNode;
			}
			else
			{
				return insertList(&((*ppHead)->NextNode), pNode, tri);
			}
		}
	}
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
	}
	else
	{
		ptrListAStar tmp = (ptrListAStar)malloc(sizeof(ListAStar));
		tmp->Current = (*ppHead)->NextNode->Current;
		tmp->DfromStart = (*ppHead)->NextNode->DfromStart;
		tmp->Heur = (*ppHead)->NextNode->Heur;
		tmp->LastD = (*ppHead)->NextNode->LastD;
		tmp->LastNode = (*ppHead)->NextNode->LastNode;
		tmp->NextNode = (*ppHead)->NextNode->NextNode;
		free((*ppHead)->NextNode);
		free((*ppHead));
		(*ppHead) = tmp;
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
	if (equalTaquin((*ppHead)->Current, pTaquin))
	{
		return ppHead;
	}
	else
	{
		return isInList(&((*ppHead)->NextNode), pTaquin);
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
	if ((!pTaquin) || (!pTabDeplacement) || (!(*pTabDeplacement)) || (!pNbDeplacements) || (!pNbTaquinsGeneres) || (!pTimeElapsed))
	{
		return -1;
	}
	int verifier[4] = { 0,0,0,0 };
	int g = 0;
	int mana = INT_MAX;
	ptrListAStar HeadOpen = createNodeList(pTaquin, g, h(pTaquin), AUCUN, NULL); //taquin actuel
	ptrListAStar HeadClose = NULL;

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
	while (mana != 0)
	{
		mana = h(pTaquin);
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
					ptrListAStar Haut = createNodeList(pTaquinN, g, h(pTaquin), HAUT, HeadOpen);
					insertList(&HeadOpen, Haut, 1);
				}
			}
		}
		//Droite
		verifier[1] = moveTaquin(pTaquinD, DROITE);
		if (verifier[1] == 1)
		{
			ptrListAStar * HandlerO = isInList(&HeadOpen, pTaquinN);
			ptrListAStar * HandlerC = isInList(&HeadClose, pTaquinN);
			if (!HandlerO)
			{
				if (!HandlerC)
				{
					ptrListAStar Droite = createNodeList(pTaquinD, g, h(pTaquin), DROITE, HeadOpen);
					insertList(&HeadOpen, Droite, 1);
				}
			}
		}
		//Bas
		verifier[2] = moveTaquin(pTaquinS, BAS);
		if (verifier[2] == 1)
		{
			ptrListAStar * HandlerO = isInList(&HeadOpen, pTaquinN);
			ptrListAStar * HandlerC = isInList(&HeadClose, pTaquinN);
			if (!HandlerO)
			{
				if (!HandlerC)
				{
					ptrListAStar Bas = createNodeList(pTaquinS, g, h(pTaquin), BAS, HeadOpen);
					insertList(&HeadOpen, Bas, 1);
				}
			}
		}
		//Gauche
		verifier[3] = moveTaquin(pTaquinG, GAUCHE);
		if (verifier[3] == 1)
		{
			ptrListAStar * HandlerO = isInList(&HeadOpen, pTaquinN);
			ptrListAStar * HandlerC = isInList(&HeadClose, pTaquinN);
			if (!HandlerO)
			{
				if (!HandlerC)
				{
					ptrListAStar Gauche = createNodeList(pTaquinG, g, h(pTaquin), GAUCHE, HeadOpen);
					insertList(&HeadOpen, Gauche, 1);
				}
			}
		}
		if (g > 1)
		{
			ptrListAStar Close = popList(&HeadOpen);
			insertList(&HeadClose, Close, 0);
		}
		else
		{
			HeadClose = popList(&HeadOpen);
		}
	}
	return 0;
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