/*Program description: These functions print sudoku validation errors to an error file. A message is printed when a
						number is not found in a row, in a column, or in a 3x3 box. A message is also printed when
						a duplicate number is found in a row, in a column, or in a 3x3 box.
Programmer Name: Tara Eicher
WSU ID: z847x563
Professor: Joseph Lesniak
Date Completed: 
*/

#include <stdio.h>

//Print an error stating that a number was not found in the specified column.

void printNotFoundInColumnError(FILE* outFile, unsigned short column, char number)
{
    fprintf(outFile,"ERROR: Number '%c' not found in column %d\n",number,column+1);
}

//Print an error stating that a number was not found in the specified row.

void printNotFoundInRowError(FILE* outFile, unsigned short row, char number)
{
    fprintf(outFile,"ERROR: Number '%c' not found in row %d\n",number,row+1);
}

//Print an error stating that a number was not found in the specified 3x3 box.

void printNotFoundIn3x3BoxError(FILE* outFile, unsigned short box, char number)
{
    fprintf(outFile,"ERROR: Number '%c' not found in 3x3 box %d\n",number,box+1);
}

//Print an error stating that a duplicate number was found in the specified column.

void printDuplicateInColumnError(FILE* outFile, unsigned short column, unsigned short row, char number)
{
	fprintf(outFile,"ERROR: Duplicate number '%c' found in column %d at row %d\n",number,column+1,row+1);
}

//Print an error stating that a duplicate number was found in the specified row.

void printDuplicateInRowError(FILE* outFile, unsigned short row, unsigned short column, char number)
{
	fprintf(outFile,"ERROR: Duplicate number '%c' found in row %d at column %d\n",number,row+1,column+1);
}

//Print an error stating that a duplicate number was found in the specified 3x3 box.

void printDuplicateIn3x3BoxError(FILE* outFile, unsigned short box, unsigned short column, unsigned short row, char number)
{
	fprintf(outFile,"ERROR: Duplicate number '%c' found in 3x3 box %d at column %d, row %d\n",number,box+1,column+1,row+1);
}