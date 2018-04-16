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
	Anode->DfromStart = gValue;
	Anode->Heur = fValue;
	Anode->Current = pTaquin;
	Anode->LastD = d;
	Anode->LastNode = pPrevPlay;
	Anode->NextNode = NULL;
	return Anode;
}

// Insertion dans la liste de fa�on tri�e ou non (F+G)
// si on passe le param�tre tri � 0, on ins�re en t�te de liste
int insertList(ptrListAStar * ppHead, ptrListAStar pNode, int tri)
{
	if (!ppHead)
	{
		return 1;
	}
	if (!(*ppHead))
	{
		return 1;
	}
	if (!pNode)
	{
		return 1;
	}
	if (tri == 0) //insert en t�te
	{
		pNode->NextNode = (*ppHead);
		(*ppHead)->LastNode = pNode;
	}
	else //insert selon F+G
	{
		if (pNode->DfromStart + pNode->Heur > (*ppHead)->DfromStart + (*ppHead)->Heur)
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
			else if (pNode->DfromStart + pNode->Heur > (*ppHead)->NextNode->DfromStart + (*ppHead)->NextNode->Heur)
			{
				(*ppHead)->NextNode = pNode;
			}
		}
	}
	return 0;
}

// Fonction pour pr�lever le noeud en t�te de liste
// Retourne le noeud pr�lev�
ptrListAStar popList(ptrListAStar * ppHead)
{
	return NULL;
}

// fonction qui retourne le noeud dans lequel on trouve le taquin pass� en param�tre (pointeur sur le pointeur dans la liste)
ptrListAStar * isInList(ptrListAStar * ppHead, Taquin * pTaquin)
{
	return NULL;
}

// fonction pour afficher une liste
// si on met displayFGH � 0 les valeur de F, G et H ne sont pas affich�es
int displayList(ptrListAStar pHead, int displayFGH)
{
	return 1;
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