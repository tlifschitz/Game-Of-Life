#include "GameLogic.h"
#include "Configs.h"
#include <stdio.h>  // ShowUniverse uses printf()
#include <string.h> // Evolve uses memcpy()
#include <time.h>	// RandomFill uses time()
#include <stdlib.h> // ShowUniverse uses system()

struct universe_t {
	CellState_t * cells;
	uint16_t width;
	uint16_t height;
	uint16_t size;
	uint16_t generations;
	uint8_t  X;
	uint8_t  Y;
	uint8_t  Z;
	uint8_t  W;
};

/* Returns TRUE or FALSE depending on whether the cell (x,y) is inside the universe or not */
static
Bool_t 		InsideTheUniverse 	(universe_t * univ, uint16_t x, uint16_t y);

/* Receives coordinates (x,y) and returns the number of that cell */
static
uint16_t 	GetIndex 			(uint16_t x, uint16_t y, uint16_t width);

/* Receives the number of a cell and returns its coordinates (x,y) */
static
void 		GetCoordenates 		(uint16_t index, uint16_t * x, uint16_t * y, uint16_t width);

/* Receives coordinates of a cell and returns whether it should live or die according to the rules*/
static
Bool_t 		RulesSayYouWillLive (universe_t * univ, uint16_t x, uint16_t y);

/* Returns a random state, LIVE or DEAD */
static
CellState_t RandomState 		(void);



Bool_t 		NewUniverse  		(universe_t ** univ)
{
	*univ = calloc (1,sizeof(universe_t));
	if (!univ)
	{
		if(DEBUG) fprintf(stderr, "ERROR: Could not allocate memory for universe\n");
		return FALSE;
	}
	else return TRUE;
}

Bool_t 		DimensionUniverse 	(universe_t * univ, int width, int height)
{
	univ->width = width;
	univ->height = height;
	univ->size = width*height;
	univ->cells = calloc(univ->size,sizeof(CellState_t));
	if (!univ->cells)
	{
		fprintf(stderr, "ERROR: Could not allocate memory for universe cells\n");
		return FALSE;
	}
	else return TRUE;
}

void 		ShowUniverse 		(universe_t * univ)
{
	uint16_t i,j;

	system("clear");

	printf(" ");
	for (j=0;j<univ->width;j++) printf("__");
	printf("\n");

	for (i=0;i<univ->height;i++)
	{
		printf("|");
		for (j=0;j<univ->width;j++)
		{
			if (GetCellState(univ,j,i)) printf("██");
			else printf("  ");
		}
		printf("|\n");
	}

	printf(" ");
	for (j=0;j<univ->width;j++) printf("¯¯");
	printf("\n");
}

CellState_t GetCellState 		(universe_t * univ, uint16_t x, uint16_t y)
{
	if (InsideTheUniverse(univ,x,y))
	{
		uint16_t index = GetIndex(x,y,GetUniverseWidth(univ));
		CellState_t state = univ->cells[index];
		return state;
	}
	else
	{
		fprintf(stderr,"ERROR: Tried to access the cell (%d,%d) which is out of the universe of (%d x %d)\n",x,y,GetUniverseHeight(univ),GetUniverseWidth(univ));
		return FALSE;
	}
}

Bool_t 		KillCell 			(universe_t * univ, uint16_t x, uint16_t y)
{
	if (InsideTheUniverse(univ,x,y))
	{
		univ->cells[GetIndex(x,y,GetUniverseWidth(univ))] = DEAD;
		return TRUE;
	}
	else
	{
		if(DEBUG) fprintf(stderr,"ERROR: Tried to access the cell (%d,%d) which is out of the universe of (%d x %d)\n",x,y,GetUniverseHeight(univ),GetUniverseWidth(univ));
		return FALSE;
	}
}

Bool_t 		ReviveCell 			(universe_t * univ, uint16_t x, uint16_t y)
{
	if (InsideTheUniverse(univ,x,y))
	{
		univ->cells[GetIndex(x,y,GetUniverseWidth(univ))] = ALIVE;
		return TRUE;
	}
	else
	{
		if(DEBUG) fprintf(stderr,"ERROR: Tried to access the cell (%d,%d) which is out of the universe of (%d x %d)\n",x,y,GetUniverseHeight(univ),GetUniverseWidth(univ));
		return FALSE;
	}
}

Bool_t 		SetCellState 		(universe_t * univ, uint16_t x, uint16_t y, CellState_t state)
{
	switch(state)
	{
		case DEAD:
			return KillCell(univ,x,y);

		case ALIVE:
			return ReviveCell(univ,x,y);

		default:
			return FALSE;
	}
}

void 		RandomFill	 		(universe_t * univ)
{
	srand(time(NULL));
	uint16_t i,j;
	for (i=0;i<univ->height;i++)
	{
		for (j=0;j<univ->width;j++)
		{
			SetCellState(univ,j,i,RandomState());
		}
	}
}

void 		Evolve 				(universe_t * univ)
{
	uint16_t i, j;

	CellState_t * NextGen =  calloc(univ->size,sizeof(CellState_t));

	for (i=0;i<univ->height;i++)
	{
		for (j=0;j<univ->width;j++)
		{
			if (RulesSayYouWillLive(univ,j,i) )
				NextGen[GetIndex(j,i,univ->width)] = ALIVE;
			else
				NextGen[GetIndex(j,i,univ->width)] = DEAD;
		}
	}
	memcpy (univ->cells,NextGen,univ->size*sizeof(CellState_t));
	univ->generations++;
}

uint16_t 	GetUniverseHeight 	(universe_t * univ)
{
	return univ->height;
}

uint16_t 	GetUniverseWidth 	(universe_t * univ)
{
	return univ->width;
}

void 		SetUniverseRules 	(universe_t * univ, int X, int Y, int Z, int W)
{
	univ->X  = X;
	univ->Y  = Y;
	univ->Z  = Z;
	univ->W  = W;
}

void 		GetUniverseRules 	(universe_t * univ, int * X, int * Y, int * Z, int * W)
{
	*X = univ->X ;
	*Y = univ->Y ;
	*Z = univ->Z ;
	*W = univ->W ;
}

uint16_t 	GetGenerationCount 	(universe_t * univ)
{
	return univ->generations;
}

uint16_t	GetLivingCells 		(universe_t * univ)
{
	uint16_t i,j,livingcells=0;

	for (i=0;i<univ->height;i++)
	{
		for (j=0;j<univ->width;j++)
		{
			if(GetCellState(univ,j,i)) livingcells++;
		}
	}
	return livingcells;
}

void 		ResetUniverse 		(universe_t * univ)
{
	uint16_t i;
	for (i=0; i<univ->size; i++)
		univ->cells[i] = DEAD;
	univ->generations = 0;
}

void 		DestroyUniverse 	(universe_t * univ)
{
	free (univ->cells);
	free (univ);
}


/*   Private functions   */

static
Bool_t 		InsideTheUniverse 	(universe_t * univ, uint16_t x, uint16_t y)
{
	return x < GetUniverseWidth(univ)  &&  y < GetUniverseHeight(univ) ? TRUE : FALSE ;
}

static
uint16_t 	GetIndex 			(uint16_t x, uint16_t y,uint16_t width)
{
	return  y * width + x;
}

static
void 		GetCoordenates 		(uint16_t index, uint16_t * x, uint16_t * y, uint16_t width)
{
	*x = index%width;
	*y = index/width;
}

static
Bool_t 		RulesSayYouWillLive (universe_t * univ, uint16_t x, uint16_t y)
{
	uint8_t neighbours = 0, i, j;
	/* By default the function checks all the 8 neighbours of a cell. */
    uint8_t Ymin=1;
    uint8_t Ymax=1;
    uint8_t Xmin=1;
    uint8_t Xmax=1;
    /* But if the cell is in one of the borders of the universe this range has to be smaller */
    if (x==0) Xmin=0;
    if (x== univ->width-1) Xmax=0;
    if (y==0) Ymin=0;
    if (y== univ->height-1) Ymax=0;


	for (i=y-Ymin; i<=y+Ymax; i++)
		for (j=x-Xmin; j<=x+Xmax; j++)
			if (GetCellState(univ,j,i)) neighbours++;

	if (GetCellState(univ,x,y)) neighbours--;

	return ( neighbours == univ->W  || (neighbours > univ->X && neighbours < univ->Y && GetCellState(univ,x,y)) ) ? TRUE : FALSE;
}

static
CellState_t RandomState 		(void)
{
	return ( rand()<(RAND_MAX/100*PROBABILITY) ) ? ALIVE : DEAD;
}