/*Program description: These functions perform initialization and deallocation of a sudoku grid and check the grid for
						validity. The validity creates three threads to evaluate the columns, rows, and 3x3 boxes. The 
						threads	then branch further to evaluate each individual column, row, and 3x3 box. A search
						algorithm is called to find the first instance of each digit and any subsequent instances.
						Errors are written to an error file by each thread, and access is controlled by a mutex lock.
Programmer Name: Tara Eicher
WSU ID: z847x563
Professor: Joseph Lesniak
Date Completed: 
*/

#ifndef SUDOKU_OPS_H
#define SUDOKU_OPS_H

#include <stdio.h>
#include <windows.h>

//Data for a box, including entry (if one exists) and options list

typedef struct box
{
	char entry;
	//optionList list;
}Box;

//Parameter data for threads.

typedef struct params
{
	Box* sudokuGrid[9][9];
	unsigned int rows;
	unsigned short index;
	HANDLE mutexLock;
	FILE* errorFile;
}Parameters;

//Initialize each entry in the grid to zero and make sure the options list is null.

void initGrid(Box* sudokuGrid[][9],unsigned short rows);

//Free space allocated for each entry in the grid.

void freeGrid(Box* sudokuGrid[][9],unsigned short rows);

//Check for null characters in the grid.

unsigned short gridIsFull(Box* sudokuGrid[][9],unsigned short rows);

//Verify that all entries in the grid are valid using threads.

unsigned short verifyAll(Box* sudokuGrid[][9],unsigned short rows, FILE* errorFile);

//Verify each column individually using threads.

DWORD WINAPI verifyColumns(LPVOID parameters);

//Verify each row individually using threads.

DWORD WINAPI verifyRows(LPVOID parameters);

//Verify each box individually using threads.

DWORD WINAPI verify3x3Boxes(LPVOID parameters);

//Verify that each column contains the numbers 1-9.

DWORD WINAPI verifyColumn(LPVOID params);

//Verify that each row contains the numbers 1-9.

DWORD WINAPI verifyRow(LPVOID params);

//Verify that each 3x3 box contains the numbers 1-9.

DWORD WINAPI verify3x3Box(LPVOID params);

//Search for a specific key in a given column, starting at a specified row number.
//Return the row position of the first instance of the key.

void searchColumn(Box* sudokuGrid[][9],unsigned short rows, unsigned short column, short* row, char key);

//Search for a specific key in a given row, starting at a specified row number.
//Return the column position of the first instance of the key.

unsigned short searchRow(Box* sudokuGrid[][9],unsigned short rows, unsigned short row, short* column, char key);

//Search for a specific key in a box given the coordinates.
//Return the coordinates of the first instance of the key.

void search3x3Box(Box* sudokuGrid[][9],unsigned short rows, short* xCoord, short* yCoord, char key);

//Check to see if the error file is empty. Print message to command line accordingly.

unsigned short hasErrors(FILE* errorFile);

#endif
