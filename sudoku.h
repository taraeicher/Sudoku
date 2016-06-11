/*Program description: Program takes in a comma-delimited file and places it in a sudoku grid, then checks to see if the
						solution is valid. It also accepts a file with no delimiters. Errors are printed to a file.
Programmer Name: Tara Eicher
WSU ID: z847x563
Professor: Joseph Lesniak
Date Completed: 10/10/2014
*/

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>

//Read values from a file into the grid. Check for formatting errors.

unsigned short populateGrid(FILE* gridVals, FILE* errorFile);

#endif