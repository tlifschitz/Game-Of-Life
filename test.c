#include "GameLogic.h"
#include <stdio.h>
#include <time.h>
#include <string.h>  ////
Bool_t StallDetection(uint16_t livingcells);

int main (void)
{
	universe_t * universe = NULL;
	NewUniverse(&universe);
	DimensionUniverse(universe,40,25);

	SetUniverseRules(universe,1,4,0,3); // Conoway's Game Of Life: 1,4,0,3
	RandomFill(universe);
	uint16_t livingcells;

	while(!StallDetection(livingcells))
	{
		livingcells = GetLivingCells(universe);
		ShowUniverse(universe);
		printf("Generacion: %u\nCelulas vivas: %u",GetGenerationCount(universe),livingcells);
		fflush(stdout);
		usleep(300000);
		Evolve(universe);
	}

	printf("\nThe game has stalled\n");

	DestroyUniverse(universe);
	return 0;
}




Bool_t StallDetection(uint16_t livingcells)
{
	#define SCOPE 10

	static uint16_t History[SCOPE] = {0};
	static uint8_t LoopCounter = 0;

	memcpy(History+1, History, SCOPE-1);
	History[0]=livingcells;
	/*

	if ( !memcmp(History,History+5,5) )
	{

	}
	if ( !memcmp(History,History+4,4) )
	{

	}
	if ( !memcmp(History,History+3,3) )
	{

	}
	if ( !memcmp(History,History+2,2) )
	{

	}*/
	if(History[0]==History[1])
		LoopCounter++;


	if (LoopCounter==30) return TRUE;
	else return FALSE;
}


/*
void play (universe_t * universe)
{

}*/