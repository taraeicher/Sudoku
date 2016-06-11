/*Program description: These functions perform initialization and deallocation of a sudoku grid and check the grid for
						validity. The validity creates three threads to evaluate the columns, rows, and 3x3 boxes. The 
						threads	then branch further to evaluate each individual column, row, and 3x3 box. A search
						algorithm is called to find the first instance of each digit and any subsequent instances.
						Errors are written to an error file by each thread, and access is controlled by a mutex lock.
Programmer Name: Tara Eicher
WSU ID: z847x563
Professor: Joseph Lesniak
Date Completed: 10/10/2014
*/

#include <windows.h>
#include <stdio.h>
#include "sudokuOps.h"
#include "errors.h"

//Initialize each entry in the grid to zero and make sure the options list is null.

void initGrid(Box* sudokuGrid[][9], unsigned short rows)
{
	unsigned short i,j;

	//Allocate space for each box in the grid and enter a null character.

	for(i=0;i<rows;i++)
	{
		for(j=0;j<9;j++)
		{
			sudokuGrid[i][j]=(Box*)malloc(sizeof(Box));
			sudokuGrid[i][j]->entry='\0';
		}
	}
}

//Free space allocated for each entry in the grid.

void freeGrid(Box* sudokuGrid[][9],unsigned short rows)
{
	unsigned short i,j;

	//Allocate space for each box in the grid and enter a null character.

	for(i=0;i<rows;i++)
	{
		for(j=0;j<9;j++)
		{
			free(sudokuGrid[i][j]);
		}
	}
}

//Check for null characters in the grid.

unsigned short gridIsFull(Box* sudokuGrid[][9], unsigned short rows)
{
	unsigned short breakLoop=0;	//sentinel flag
	short row=0;
	unsigned short i;
	
	//Check for a null character in each column.
	//Exit when either a null character is found or when all columns have been checked.
	
	for(i=0;i<rows&&breakLoop==0;i++)
	{
	  searchColumn(sudokuGrid,rows,i,&row,'\0');
		
		//Break the loop when a null character is found.
		
		if(row!=-1)
			breakLoop=1;

		//Set row back to 0 to start next iteration.

		row=0;
	}
	
	//This condition will only be met if the loop was not broken.
	
	if(!breakLoop)
		return 1;
		
	else return 0;
}


//Verify that all entries in the grid are valid using threads.

unsigned short verifyAll(Box* sudokuGrid[][9], unsigned short rows, FILE* errorFile)
{
	DWORD ColumnThreadID,RowThreadID,BoxThreadID;	//Store thread ID's.
	unsigned short i,j,k;
	
	//Use a thread each to evaluate columns, rows, and boxes.
	//Use a mutex lock to disallow multiple threads printing errors to the same file at the same time.
	
	HANDLE topLevelThreads[3];
	HANDLE mutex=CreateMutex(NULL,FALSE,NULL);

	//Set parameters.

	Parameters params;
//	initGrid(params.sudokuGrid,9);
	params.index=0;//trivial
	params.mutexLock=mutex;
	params.errorFile=errorFile;
	for(i=0;i<9;i++)
		for(j=0;j<9;j++)
			params.sudokuGrid[i][j]=sudokuGrid[i][j];
	params.rows=rows;
	

	//Keep this conditionalization for part 1 of project only.

	if(gridIsFull(params.sudokuGrid,params.rows)==0)
		fprintf(errorFile,"ERROR: There are blank spaces in the grid.\n");
	else
	{
		topLevelThreads[0]=CreateThread(NULL,0,verifyColumns,(void*)&params,0,&ColumnThreadID);
		topLevelThreads[1]=CreateThread(NULL,0,verifyRows,(void*)&params,0,&RowThreadID);
		topLevelThreads[2]=CreateThread(NULL,0,verify3x3Boxes,(void*)&params,0,&BoxThreadID);
	
		//Wait for all of the threads to complete.

		WaitForMultipleObjects(3,topLevelThreads,TRUE,INFINITE);
		
		//Close the thread handles and deallocate the grid.
	
		for(k=0;k<3;k++)
			CloseHandle(topLevelThreads[k]);
	}
//	freeGrid(params.sudokuGrid,9);

	//Check for errors.

	return hasErrors(errorFile);
	
}

//Verify each column individually using threads.

DWORD WINAPI verifyColumns(LPVOID parameters)
{
	DWORD threadIDs[9];	//Store thread ID's.
	Parameters params[9];	//Holds column numbers and mutex locks to pass to threads.
	HANDLE columnThreads[9];
	unsigned short i,j,k,l,m;

	//Unpack arguments.

	Parameters* args=(Parameters*)parameters;
	
	//Initialize parameters.
//printf("Top-level column thread running\n");	
	for(i=0;i<9;i++)
	{
		params[i].index=i;
	//	initGrid(params[i].sudokuGrid,9);
		params[i].mutexLock=args->mutexLock;
		for(j=0;j<9;j++)
			for(k=0;k<9;k++)
				params[i].sudokuGrid[j][k]=args->sudokuGrid[j][k];
		params[i].rows=args->rows;
		params[i].errorFile=args->errorFile;
	}
	
	//Use a thread to evaluate each column.
	
	columnThreads[0]=CreateThread(NULL,0,verifyColumn,(void*)&params[0],0,&threadIDs[0]);
	columnThreads[1]=CreateThread(NULL,0,verifyColumn,(void*)&params[1],0,&threadIDs[1]);
	columnThreads[2]=CreateThread(NULL,0,verifyColumn,(void*)&params[2],0,&threadIDs[2]);
	columnThreads[3]=CreateThread(NULL,0,verifyColumn,(void*)&params[3],0,&threadIDs[3]);
	columnThreads[4]=CreateThread(NULL,0,verifyColumn,(void*)&params[4],0,&threadIDs[4]);
	columnThreads[5]=CreateThread(NULL,0,verifyColumn,(void*)&params[5],0,&threadIDs[5]);
	columnThreads[6]=CreateThread(NULL,0,verifyColumn,(void*)&params[6],0,&threadIDs[6]);
	columnThreads[7]=CreateThread(NULL,0,verifyColumn,(void*)&params[7],0,&threadIDs[7]);
	columnThreads[8]=CreateThread(NULL,0,verifyColumn,(void*)&params[8],0,&threadIDs[8]);
	
	//Wait for all of the threads to complete.
	
	WaitForMultipleObjects(9,columnThreads,TRUE,INFINITE);
		
	//Close the thread handles and clear the grids.
	
	for(l=0;l<9;l++)
		CloseHandle(columnThreads[l]);
/*	for(m=0;m<9;m++)
*		freeGrid(params[m].sudokuGrid,9);*/
}

//Verify each row individually using threads.

DWORD WINAPI verifyRows(LPVOID parameters)
{
	DWORD threadIDs[9];	//Store thread ID's.
	Parameters params[9];	//Holds column numbers and mutex locks to pass to threads.
	HANDLE rowThreads[9];
	unsigned short i,j,k,l,m;
	
	//Unpack arguments.

	Parameters* args=(Parameters*)parameters;

	//Initialize parameters.
//printf("Top-level row thread running\n");	
	for(i=0;i<9;i++)
	{
		params[i].index=i;
	//	initGrid(params[i].sudokuGrid,9);
		params[i].mutexLock=args->mutexLock;
		for(j=0;j<9;j++)
			for(k=0;k<9;k++)
				params[i].sudokuGrid[j][k]=args->sudokuGrid[j][k];
		params[i].rows=args->rows;
		params[i].errorFile=args->errorFile;
	}
	
	//Use a thread to evaluate each column.
	
	rowThreads[0]=CreateThread(NULL,0,verifyRow,(void*)&params[0],0,&threadIDs[0]);
	rowThreads[1]=CreateThread(NULL,0,verifyRow,(void*)&params[1],0,&threadIDs[1]);
	rowThreads[2]=CreateThread(NULL,0,verifyRow,(void*)&params[2],0,&threadIDs[2]);
	rowThreads[3]=CreateThread(NULL,0,verifyRow,(void*)&params[3],0,&threadIDs[3]);
	rowThreads[4]=CreateThread(NULL,0,verifyRow,(void*)&params[4],0,&threadIDs[4]);
	rowThreads[5]=CreateThread(NULL,0,verifyRow,(void*)&params[5],0,&threadIDs[5]);
	rowThreads[6]=CreateThread(NULL,0,verifyRow,(void*)&params[6],0,&threadIDs[6]);
	rowThreads[7]=CreateThread(NULL,0,verifyRow,(void*)&params[7],0,&threadIDs[7]);
	rowThreads[8]=CreateThread(NULL,0,verifyRow,(void*)&params[8],0,&threadIDs[8]);
	
	//Wait for all of the threads to complete.
	
	WaitForMultipleObjects(9,rowThreads,TRUE,INFINITE);
		
	//Close the thread handles.
	
	for(l=0;l<9;l++)
		CloseHandle(rowThreads[l]);
/*	for(m=0;m<9;m++)
		freeGrid(params[m].sudokuGrid,9);*/
}

//Verify each box individually using threads.

DWORD WINAPI verify3x3Boxes(LPVOID parameters)
{
	DWORD threadIDs[9];	//Store thread ID's.
	HANDLE boxThreads[9];
	Parameters params[9];	//Holds column numbers and mutex locks to pass to threads.
	unsigned short i,j,k,l,m;

	//Unpack arguments.

	Parameters* args=(Parameters*)parameters;
	
	//Initialize parameters.
//printf("Top-level box thread running\n");	
	for(i=0;i<9;i++)
	{
		params[i].index=i;
//		initGrid(params[i].sudokuGrid,9);
		params[i].mutexLock=args->mutexLock;
		for(j=0;j<9;j++)
			for(k=0;k<9;k++)
				params[i].sudokuGrid[j][k]=args->sudokuGrid[j][k];
		params[i].rows=args->rows;
		params[i].errorFile=args->errorFile;
	}
	
	//Use a thread to evaluate each 3x3 box.
	
	boxThreads[0]=CreateThread(NULL,0,verify3x3Box,(void*)&params[0],0,&threadIDs[0]);
	boxThreads[1]=CreateThread(NULL,0,verify3x3Box,(void*)&params[1],0,&threadIDs[1]);
	boxThreads[2]=CreateThread(NULL,0,verify3x3Box,(void*)&params[2],0,&threadIDs[2]);
	boxThreads[3]=CreateThread(NULL,0,verify3x3Box,(void*)&params[3],0,&threadIDs[3]);
	boxThreads[4]=CreateThread(NULL,0,verify3x3Box,(void*)&params[4],0,&threadIDs[4]);
	boxThreads[5]=CreateThread(NULL,0,verify3x3Box,(void*)&params[5],0,&threadIDs[5]);
	boxThreads[6]=CreateThread(NULL,0,verify3x3Box,(void*)&params[6],0,&threadIDs[6]);
	boxThreads[7]=CreateThread(NULL,0,verify3x3Box,(void*)&params[7],0,&threadIDs[7]);
	boxThreads[8]=CreateThread(NULL,0,verify3x3Box,(void*)&params[8],0,&threadIDs[8]);
	
	//Wait for all of the threads to complete.
	
	WaitForMultipleObjects(9,boxThreads,TRUE,INFINITE);
		
	//Close the thread handles.
	
	for(l=0;l<9;l++)
		CloseHandle(boxThreads[l]);
/*	for(m=0;m<9;m++)
		freeGrid(params[m].sudokuGrid,9);*/
}

//Verify that each column contains the numbers 1-9.

DWORD WINAPI verifyColumn(LPVOID params)
{
  //Unpack arguments.

	Parameters* parms=(Parameters*)params;
	char i;
/*  short blankPosX=column;
  short blankPosY=-1;
  unsigned short numsFound=0;
  unsigned short blanksFound=0;
  */
	short rowNum=0;                         //starting row number

/*	//Determine whether grid is full and take action based on that.
						
	if(gridIsFull(parms.sudokuGrid))
	{*/
		//Search for each number. Print an error if a number is not found, and print an error if the number is duplicated.
//printf("Column thread running\n");      
		for(i='1';i<='9';i++)
		{
			searchColumn(parms->sudokuGrid,parms->rows,parms->index,&rowNum,i);

			//If the number is not found, access the file using a mutex and print an error.

			if(rowNum==-1)
			{
				DWORD waitResult;
				waitResult=WaitForSingleObject(parms->mutexLock,INFINITE);
				switch(waitResult)
				{
					case WAIT_OBJECT_0:
						printNotFoundInColumnError(parms->errorFile,parms->index,i);
						ReleaseMutex(parms->mutexLock);
						break;
				}

			}

			//Check to see if the number is found again. If it is, print an error for each duplicate.

			else
			{
				do
				{
					rowNum++;
					searchColumn(parms->sudokuGrid,parms->rows,parms->index,&rowNum,i);
					if(rowNum!=-1)
					{
						DWORD waitResult;
						waitResult=WaitForSingleObject(parms->mutexLock,INFINITE);
						switch(waitResult)
						{
							case WAIT_OBJECT_0:
								printDuplicateInColumnError(parms->errorFile,parms->index,rowNum,i);
								ReleaseMutex(parms->mutexLock);
								break;
						}
					}
				}
				while(rowNum!=-1&&rowNum<9);
			}

			//Set row back to 0 for next iteration.

			rowNum=0;
		}

//	}

	      /*  else
    {
          //Check the column for numbers 1 through 9. Every time one of these is found, increment counter.
      //NOTE: Number will only be found once. This will not find duplicates.

      for(char key='1';key<'9';key++)
	{
      searchColumn(column,key)
	if(blankPosX!=-1)
	  numsFound++;
	}

      //Get the number of blanks in the column.

      getNextBlank(&blankPosX, &blankPosY);
      while(blankPosX!=-1)
	numsFound++;

      //Verify that blanks+numbers=9 (in other words, there are no duplicates).
      
      if(numsFound+blanksFound==9)
	return true;
      else
      return false;
      }*/
} 

//Verify that each row contains the numbers 1-9.

DWORD WINAPI verifyRow(LPVOID params)
{
	short colNum=0;                         //starting row number
  //Unpack arguments.

	Parameters* parms=(Parameters*)params;
	char i;
	
/*  short blankPosX=column;
  short blankPosY=-1;
  unsigned short numsFound=0;
  unsigned short blanksFound=0;
  */

/*	//Determine whether grid is full and take action based on that.
						
	if(gridIsFull(parms.sudokuGrid))
	{*/
		//Search for each number. Print an error if a number is not found, and print an error if the number is duplicated.
//printf("Row thread running\n");        
		for(i='1';i<='9';i++)
		{
			searchRow(parms->sudokuGrid,parms->rows,parms->index,&colNum,i);

			  //If the number is not found, access the file using a mutex and print an error.

				if(colNum==-1)
				{
					DWORD waitResult;
					waitResult=WaitForSingleObject(parms->mutexLock,INFINITE);
					switch(waitResult)
					{
						case WAIT_OBJECT_0:
							printNotFoundInRowError(parms->errorFile,parms->index,i);
							ReleaseMutex(parms->mutexLock);
							break;
					}
				}

				//Check to see if the number is found again. If it is, print an error for each duplicate.

				else
				{
					do
					{
						colNum++;
						searchRow(parms->sudokuGrid,parms->rows,parms->index,&colNum,i);
						if(colNum!=-1)
						{
							DWORD waitResult;
							waitResult=WaitForSingleObject(parms->mutexLock,INFINITE);
							switch(waitResult)
							{
								case WAIT_OBJECT_0:
									printDuplicateInRowError(parms->errorFile,parms->index,colNum,i);
									ReleaseMutex(parms->mutexLock);
									break;
							}
						}
					}
					while(colNum!=-1&&colNum<9);
				}
				
				//Set column back to 0 for next iteration.

				colNum=0;
			}
//	}

	      /*  else
    {
          //Check the column for numbers 1 through 9. Every time one of these is found, increment counter.
      //NOTE: Number will only be found once. This will not find duplicates.

      for(char key='1';key<'9';key++)
	{
      searchColumn(column,key)
	if(blankPosX!=-1)
	  numsFound++;
	}

      //Get the number of blanks in the column.

      getNextBlank(&blankPosX, &blankPosY);
      while(blankPosX!=-1)
	numsFound++;

      //Verify that blanks+numbers=9 (in other words, there are no duplicates).
      
      if(numsFound+blanksFound==9)
	return true;
      else
      return false;
      }*/
} 

//Verify that each 3x3 box contains the numbers 1-9.

DWORD WINAPI verify3x3Box(LPVOID params)
{
  //Unpack arguments.

	Parameters* parms=(Parameters*)params;
	char i;
/*  short blankPosX=column;
  short blankPosY=-1;
  unsigned short numsFound=0;
  unsigned short blanksFound=0;
  */
	short row,col,startRow,startCol;            //starting row and column numbers
	unsigned short indexForOutput;				//conversion of index number from row-major form to output form
	
	//Assign starting row and column numbers.
	
	startCol=(parms->index/3)*3;
	startRow=(parms->index%3)*3;
	row=startRow;
	col=startCol;

	//Convert index to output index.
	switch(parms->index)
	{
		case 0:
			indexForOutput=0;
			break;
		case 1:
			indexForOutput=3;
			break;
		case 2:
			indexForOutput=6;
			break;
		case 3:
			indexForOutput=1;
			break;
		case 4:
			indexForOutput=4;
			break;
		case 5:
			indexForOutput=7;
			break;
		case 6:
			indexForOutput=2;
			break;
		case 7:
			indexForOutput=5;
			break;
		case 8:
			indexForOutput=8;
			break;
	}
		
/*	//Determine whether grid is full and take action based on that.
						
	if(gridIsFull(parms.sudokuGrid))
	{*/
		//Search for each number. Print an error if a number is not found, and print an error if the number is duplicated.
//printf("Box thread running\n");        
		for(i='1';i<='9';i++)
		{
			search3x3Box(parms->sudokuGrid,parms->rows,&row,&col,i);

			  //If the number is not found, access the file using a mutex and print an error.

				if(row==-1)
				{
					DWORD waitResult;
					waitResult=WaitForSingleObject(parms->mutexLock,INFINITE);
					switch(waitResult)
					{
						case WAIT_OBJECT_0:
							printNotFoundIn3x3BoxError(parms->errorFile,indexForOutput,i);
							ReleaseMutex(parms->mutexLock);
							break;
					}
				}

				//Check to see if the number is found again. If it is, print an error for each duplicate.

				else
				{
					//Iterate as necessary.

					if(col<(2+startCol))
						col++;
					else
					{
						col=startCol;
						row++;
					}

					//While still in this 3x3 box and the search has not returned null, perform another search.

					while(row!=-1&&(row-startRow)<3)
					{
						search3x3Box(parms->sudokuGrid,parms->rows,&row,&col,i);
						if(row!=-1)
						{
							DWORD waitResult;
							waitResult=WaitForSingleObject(parms->mutexLock,INFINITE);
							switch(waitResult)
							{
								case WAIT_OBJECT_0:
									printDuplicateIn3x3BoxError(parms->errorFile,indexForOutput,col,row,i);
									ReleaseMutex(parms->mutexLock);
									break;
							}

							//Iterate as necessary.

							if(col<(2+startCol))
								col++;
							else
							{
								col=startCol;
								row++;
							}
						}
					}
				}

				//Set row and column back to start for next iteration.

				row=startRow;
				col=startCol;
			}
//	}

	      /*  else
    {
          //Check the column for numbers 1 through 9. Every time one of these is found, increment counter.
      //NOTE: Number will only be found once. This will not find duplicates.

      for(char key='1';key<'9';key++)
	{
      searchColumn(column,key)
	if(blankPosX!=-1)
	  numsFound++;
	}

      //Get the number of blanks in the column.

      getNextBlank(&blankPosX, &blankPosY);
      while(blankPosX!=-1)
	numsFound++;

      //Verify that blanks+numbers=9 (in other words, there are no duplicates).
      
      if(numsFound+blanksFound==9)
	return true;
      else
      return false;
      }*/
} 


//Search for a specific key in a given column, starting at a specified row number.
//Return the row position of the first instance of the key.

void searchColumn(Box* sudokuGrid[][9], unsigned short rows, unsigned short column, short* row, char key)
{
	short x=*row;	//starting row position
	unsigned short breakFromLoop=0;	//sentinel flag
	
	//Starting at the given row position, loop through until first instance of key is found.
//printf("Searching column %d\n",column);	
	while(x<9&&breakFromLoop==0)
	{
		//If key is found, return position where found.

		if(sudokuGrid[x][column]->entry==key)
		{
			*row=x;
			breakFromLoop=1;
		}
		else x++;
	}
	
	//If key was not found, return -1.
	
	if(x>=9)
		*row=-1;
}

//Search for a specific key in a given row, starting at a specified row number.
//Return the column position of the first instance of the key.

unsigned short searchRow(Box* sudokuGrid[][9], unsigned short rows, unsigned short row, short* column, char key)
{
	short y=*column;	//starting column position
	unsigned short breakFromLoop=0;	//sentinel flag
	
	//Starting at the given row position, loop through until first instance of key is found.
//printf("Searching row %d\n",row);		
	while(y<9&&breakFromLoop==0)
	{
		//If key is found, return position where found.
	
		if(sudokuGrid[row][y]->entry==key)
		{
			*column=y;
			breakFromLoop=1;
		}
		else y++;
	}
	
	//If key was not found, return -1.
	
	if(y>=9)
		*column=-1;
}

//Search for a specific key in a box given the coordinates.
//Return the coordinates of the first instance of the key.

void search3x3Box(Box* sudokuGrid[][9], unsigned short rows, short* xCoord, short* yCoord, char key)
{
	unsigned short breakFromLoop=0;	//sentinel flag
	short xLimit=(*xCoord)+(3-((*xCoord)%3));
	short yLimit=(*yCoord)+(3-((*yCoord)%3));

	//Loop through all rows in the box, starting at the given row.
//printf("Searching column %d row %d in box\n",*xCoord,*yCoord);		
	while((*xCoord)<xLimit&&breakFromLoop==0)
	{
		//Loop through all columns in the box, starting at the given column.
	
		while((*yCoord)<yLimit&&breakFromLoop==0)
		{
			//If the entry matches the key, break from the loop.

			if(sudokuGrid[*xCoord][*yCoord]->entry==key)
				breakFromLoop=1;
			else (*yCoord)++;
		}

		//Reset x (and increase y) when it has increased beyond the scope of this 3x3 box and there is no value to return.

		if(breakFromLoop==0)
		{
			(*yCoord)-=3;
			(*xCoord)++;
		}
	}
	
	//If key was not found, return (-1,-1) as the coordinates.
	
	if(breakFromLoop==0)
	{
		*xCoord=-1;
		*yCoord=-1;
	}
		
/*		if(boxNum<3)
		{
			unsigned short x=boxNum*3;
			for(unsigned short y=0;y<3;y++)
			{
				for(unsigned short i=0;i<3;i++,x++)
				  if(sudokuGrid[x,y].entry==key)
				    match=true;
				x=boxNum*3;
			}
		}
		else if(boxNum<6)
		{
			unsigned short x=(boxNum-3)*3;
			for(unsigned short y=3;y<6;y++)
			{
				for(unsigned short i=0;i<3;i++,x++)
				  if(sudokuGrid[x,y].entry==key)
				    match=true;
				x=(boxNum-3)*3;
			}
		}
		else
		{
			unsigned short x=(boxNum-6)*3;
			for(unsigned short y=6;y<9;y++)
			{
				for(unsigned short i=0;i<3;i++,x++)
				  if(sudokuGrid[x,y].entry==key)
				    match=true;
				x=(boxNum-6)*3;
			}
		}
*/	
}
/*
void addOptionToColumn(PointerGroup group, char option, unsigned short expectedNumberOfBlanks)
{
	unsigned short numBlanks=0;
	
	for(unsigned short i=0;i<9;i++)
	{
		if(group[i].entry=='\0')
			numBlanks++;
	}
	if(numBlanks==expectedNumberOfBlanks)
	{
		for(unsigned short i=0;i<9;i++)
		{
			if(group[i].entry=='\0')
				appendOption(option,group[i].list);
		}
	}
	else //Print error file and return error status.
}

void checkOptionsAtCoordinate(Grid sudokuGrid, unsigned short x, unsigned short y);

void addOptions()
{
	unsigned short numBlanksExpected=0;
	bool numberFound=false;
	short blank_x=0;
	short blank_y=0;
	unsigned short boxNum=0;
	char key='1';
	DWORD threadIDs[3];
	HANDLE threads[3];
	getNextBlank(sudokuGrid,&blank_x,&blank_y);

	//Find the number of the box in which these coordinates are contained.

	while(blank_x!=-1)
	{
	  //For each blank, sequentially check each number in parallel (column, row, box).
	  for(;key<'9';key++)
	    {
	  threads[0]=CreateThread(NULL,0,searchColumn,&sudokuGrid,&x,&key,0,&threadIDs[0];
	  threads[1]=CreateThread(NULL,0,searchRow,&sudokuGrid,&y,&key,0,&threadIDs[1];
	  threads[2]=CreateThread(NULL,0,search3x3Box,&sudokuGrid,&boxNum,&key,0,&threadIDs[2];
				  }
				  getNextBlank(sudokuGrid,&blank_x,&blank_y);
	}

	if(!searchColumn(sudokuGrid,columNum,'1'))
	{
		numBlanksExpected++;
		addOptionToGroup(group,'1',numBlanksExpected);
	}
	if(!searchColumn(sudokuGrid,columnNum,'2'))
	{
		numBlanksExpected++;
		addOptionToGroup(group,'2',numBlanksExpected);
	}
	if(!searchColumn(sudokuGrid,columnNum,'3'))
	{
		numBlanksExpected++;
		addOptionToGroup(group,'3',numBlanksExpected);
	}
	if(!searchColumn(sudokuGrid,columnNum,'4'))
	{
		numBlanksExpected++;
		addOptionToGroup(group,'4',numBlanksExpected);
	}
	if(!searchColumn(sudokuGrid,columnNum,'5'))
	{
		numBlanksExpected++;
		addOptionToGroup(group,'5',numBlanksExpected);
	}
	if(!searchColumn(sudokuGrid,columnNum,'6'))
	{
		numBlanksExpected++;
		addOptionToGroup(group,'6',numBlanksExpected);
	}
	if(!searchColumn(sudokuGrid,columnNum,'7'))
	{
		numBlanksExpected++;
		addOptionToGroup(group,'7',numBlanksExpected);
	}
	if(!searchColumn(sudokuGrid,columnNum,'8'))
	{
		numBlanksExpected++;
		addOptionToGroup(group,'8',numBlanksExpected);
	}
	if(!searchColumn(sudokuGrid,columnNum,'9'))
	{
		numBlanksExpected++;
		addOptionToGroup(group,'9',numBlanksExpected);
	}
}

void getNextBlank(Grid sudokuGrid, short* xCoord, short* yCoord)
{
	short x=*xCoord;
	short y=*yCoord;
	for(;y<9;y++)
	{
		for(;x<9;x++)
		{
			if(sudokuGrid[x,y].entry=='\0')
			{
				*xCoord=x;
				*yCoord=y;
			}

		}
	}
	if(y>9)
	{
		*xCoord=-1;
		*yCoord=-1
	}
}
*/

//Check to see if the error file is empty. Print message to command line accordingly.

unsigned short hasErrors(FILE* errorFile) 
{
	if(fgetc(errorFile)==EOF)
		return 0;
	else
		return 1;
}

