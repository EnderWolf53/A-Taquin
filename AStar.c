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

// Insertion dans la liste de façon triée ou non (F+G)
// si on passe le paramètre tri à 0, on insère en tête de liste
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
	if (tri == 0) //insert en tête
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

// Fonction pour prélever le noeud en tête de liste
// Retourne le noeud prélevé
ptrListAStar popList(ptrListAStar * ppHead)
{
	return NULL;
}

// fonction qui retourne le noeud dans lequel on trouve le taquin passé en paramètre (pointeur sur le pointeur dans la liste)
ptrListAStar * isInList(ptrListAStar * ppHead, Taquin * pTaquin)
{
	return NULL;
}

// fonction pour afficher une liste
// si on met displayFGH à 0 les valeur de F, G et H ne sont pas affichées
int displayList(ptrListAStar pHead, int displayFGH)
{
	return 1;
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
	return 1;
}

// fonction d'évaluation pour la résolution avec AStar
int h(Taquin * pTaquin)
{
	return 0;
}