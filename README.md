This multithreaded program takes in a comma-delimited file and places it in a sudoku grid, then checks to see if the
solution is valid. It also accepts a file with no delimiters. Three threads are used to evaluate the columns, rows, and 3x3 boxes. The 
threads	then branch further to evaluate each individual column, row, and 3x3 box. A search algorithm is called to find the first instance of each digit and any subsequent instances.	Errors are written to an error file by each thread, and access is controlled by a mutex lock.
