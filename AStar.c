#include "AStar.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "SDL.h"
//#include "SDL_image.h"



// fonction pour cr�er (allouer) un noeud de liste et l'initialiser avec le taquin pass� en param�tre
ptrListAStar createNodeList(Taquin * pTaquin, int gValue, int fValue, deplacement d, ptrListAStar pPrevPlay)
{
	if (!pTaquin)
	{
		return NULL;
	}
	ptrListAStar Anode = (ptrListAStar)malloc(sizeof(ListAStar));
	Anode->DfromStart = fValue;
	Anode->Heur = gValue;
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

// Insertion dans la liste de fa�on tri�e ou non (F+G)
// si on passe le param�tre tri � 0, on ins�re en t�te de liste
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
	if (tri == 0) //insert en t�te
	{
		pNode->NextNode = (*ppHead);
		(*ppHead)->LastNode = pNode;
	}
	else //insert selon F+G
	{
		if (fpg(pNode) > fpg((*ppHead)))
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
			else if (fpg(pNode) > fpg((*ppHead)->NextNode))
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

// Fonction pour pr�lever le noeud en t�te de liste
// Retourne le noeud pr�lev�
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

// fonction qui retourne le noeud dans lequel on trouve le taquin pass� en param�tre (pointeur sur le pointeur dans la liste)

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
		return (*ppHead);
	}
	else
	{
		return isInList(&((*ppHead)->NextNode), pTaquin);
	}
	return NULL;
}

// fonction pour afficher une liste
// si on met displayFGH � 0 les valeur de F, G et H ne sont pas affich�es
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

// Fonction pour r�soudre le taquin en utilisant l'algorithme A*
// La fonction prend comme taquin initial le taquin sp�cifi� par pTaquin
// elle remplit 
//   - pTabDeplacement (un tableau contenant les d�placements � effectuer pour r�soudre le taquin)
//   - pNbDeplacements
//   - pNbTaquinsGeneres
//   - pTimeElapsed
// Si stepByStep est diff�rent de 0 on affiche dans la console toutes les �tapes de la r�solution
// pWindow
int solveTaquin(Taquin *pTaquin, deplacement ** pTabDeplacement, unsigned long *pNbDeplacements, unsigned long * pNbTaquinsGeneres, unsigned long * pTimeElapsed, int stepByStep, SDL_Surface * pWindow)
{
	return 1;
}

// fonction d'�valuation pour la r�solution avec AStar
int h(Taquin * pTaquin)
{
	return 0;
}