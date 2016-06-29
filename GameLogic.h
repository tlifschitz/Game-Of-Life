#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <stdint.h>
#include "Bool.h"

typedef struct universe_t universe_t;
typedef enum {DEAD, ALIVE} CellState_t;

/* Create new universe */
Bool_t 		NewUniverse  	  	(universe_t ** univ);

/* Sets the dimensions of the universe  */
Bool_t 		DimensionUniverse 	(universe_t * univ, int width, int height);

/* Shows universe in linux console */
void 		ShowUniverse  		(universe_t * univ);

/* Returns whether a cell is dead or alive */
CellState_t GetCellState 		(universe_t * univ, uint16_t x, uint16_t y);

/* Kills a cell */
Bool_t 		KillCell     		(universe_t * univ, uint16_t x, uint16_t y);

/* Revives a cell */
Bool_t		ReviveCell   		(universe_t * univ, uint16_t x, uint16_t y);

/* Sets the cell's state to dead or alive */
Bool_t 		SetCellState 		(universe_t * univ, uint16_t x, uint16_t y, CellState_t);

/* Fills the universe randomly with living cells */
void		RandomFill	 		(universe_t * univ);

/* Makes the universe evolve according to the rules */
void		Evolve		 		(universe_t * univ);

/* Returns universe height*/
uint16_t	GetUniverseHeight 	(universe_t * univ);

/* Returns universe with */
uint16_t	GetUniverseWidth 	(universe_t * univ);

/* Sets universe rules:
Each living cell with X living neighbours or less dies, as if by solitude.
Each living cell with Y living neighbours or more dies, as if by overpopulation.
Each dead cell with exactly W living neighbours revives.
	(X < Y)
*/
void		SetUniverseRules 	(universe_t * univ, int X, int Y, int Z, int W);

/* Saves universe rules in the variables received */
void		GetUniverseRules 	(universe_t * univ, int * X, int * Y, int * Z, int * W);

/* Returns the number of generations since the evolution started */
uint16_t	GetGenerationCount 	(universe_t * univ);

/* Returns the number of living cells in the current generation */
uint16_t	GetLivingCells 		(universe_t * univ);

/* Kills all cells and put generation counter to 0 */
void 		ResetUniverse		(universe_t * univ);

/* Destroy the universe. Must call at the end of the program*/
void 		DestroyUniverse		(universe_t * univ);

#endif	// GAMELOGIC_H


/*                              EXAMPLE

(test.c)

#include "GameLogic.h"
#include <stdio.h>
#include <time.h>

int main (void)
{
	universe_t * universe = NULL;
	NewUniverse(&universe);
	DimensionUniverse(universe,30,25);

	SetUniverseRules(universe,1,4,0,3); // Conoway's Game Of Life rules: 1,4,0,3
	RandomFill(universe);

	uint16_t livingcells;

	while(TRUE)
	{
		livingcells = GetLivingCells(universe);
		ShowUniverse(universe);
		printf("Generacion: %u\n Celulas vivas: %u",GetGenerationCount(universe),livingcells);
		fflush(stdout);
		usleep(400000);
		Evolve(universe);
	}


	DestroyUniverse(universe);
	return 0;
}

*/