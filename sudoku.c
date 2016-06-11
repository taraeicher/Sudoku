/*Program description: Program takes in a comma-delimited file and places it in a sudoku grid, then checks to see if the
						solution is valid. It also accepts a file with no delimiters. Errors are printed to a file.
Programmer Name: Tara Eicher
WSU ID: z847x563
Professor: Joseph Lesniak
Date Completed: 10/10/2014
*/

#include <stdio.h>
#include "sudokuOps.h"
#include "sudoku.h"
#include "string.h"

Box* sudokuGrid[9][9];	//Grid holds user input values.

int main()
{
	FILE* errorFile=fopen("ERRORS.txt","w+");		//Open error file.
	FILE* gridVals;
	char* filename=(char*)malloc(sizeof(char)*21);
	unsigned short fileIsInvalid;					//Is file valid (bool)?
	unsigned short isWrong;							//Is solution wrong (bool)?

	//Get the user's file.
	printf("Please enter the name of your file (can be no longer than 20 characters): ");
	scanf("%s",filename);

	//Populate the grid and verify the entries are correct.

	gridVals=fopen(filename,"r");

	//Print an error message if the file could not be opened.

	if(gridVals==NULL)
	{
		printf("File \"%s\" could not be opened.\n",filename);

		//Display screen until user presses ENTER.

		printf("Press ENTER to exit.");
		getc(stdin);
		getc(stdin);
	}
	else
	{
		initGrid(sudokuGrid,9);
		fileIsInvalid=populateGrid(gridVals,errorFile);
		fclose(gridVals);
		
		//If file is not invalid, validate the user entry.

		if(!fileIsInvalid)
		{
			isWrong=verifyAll(sudokuGrid,9,errorFile);

			//Close the error file.

			fclose(errorFile);

			//Print message to user telling whether solution is correct.

			if(isWrong)
				printf("There were errors. Please see ERRORS.txt for details.\n");
			else
			{
				printf("The solution is correct.\n");
				remove("ERRORS.txt");
			}

			//Display screen until user presses ENTER.

			printf("Press ENTER to exit.");
			getc(stdin);
			getc(stdin);
		}
	
		//Free all allocated resources.

		freeGrid(sudokuGrid,9);
	}
	free(filename);
}

//Read values from a file into the grid. Check for formatting errors.

unsigned short populateGrid(FILE* gridVals, FILE* errorFile)
{
	char c='\0';					//Track char from file.
	char previous='\0';				//Track previous char from file (used for reading blanks).
	unsigned short i,j;
	unsigned short badCharFound=0;

	for(i=0;i<9&&(!badCharFound);i++)
	{
		for(j=0;j<9&&(!badCharFound);j++)
		{
			//Set values for this iteration.
		
			previous=c;
			c=fgetc(gridVals);
			
			//If the character is a digit, place it in the grid.
			
			if(c>'0'&&c<='9')
				sudokuGrid[i][j]->entry=c;
				
			//If the character is a comma between two non-comma characters, do not count it.
				
			else if(c==','&&(!(previous==','||previous=='\n')))
				j--;
				
			/*If the character is a comma following a comma or a blank at the beginning or end of the line, 
			place a null character in the grid.*/
				
			else if((c==','&&(previous==','||previous=='\n'))||(c=='\n'&&previous==','))
				sudokuGrid[i][j]->entry='\0';
				
			//Otherwise, generate an error, because the character is unexpected.
			
			else
			{
				printf("Character '%c' unexpected at line %d in input file.\n",c,i+1);
				badCharFound=1;

				//Display screen until user presses ENTER.

				printf("Press ENTER to exit.");
				getc(stdin);
				getc(stdin);
			}
		}
		//Get the newline if there is not a missing character at the end of the line.

		if(c!='\n')
			c=fgetc(gridVals);	//Get the newline.
	}
	
	if(!badCharFound)
	{
		//Last character read should be end-of-file marker.
	
		c=fgetc(gridVals);
		if(c!=EOF)
		{
			printf("File is too long.\n");
			badCharFound=1;
						
			//Display screen until user presses ENTER.

			printf("Press ENTER to exit.");
			getc(stdin);
			getc(stdin);
		}
	}

	//Notify user whether population was successful.

	return badCharFound;
}


