/*
 *  Name: Alberto Camacho					Section: U-06
 *
 * 	I affirm that this program is entirely my own work and none of it is the work of any other person.
 *
 *
 *  Program Description: This program will read in a bmp image file and can perform three transformations: 1) enlarge an image by a certain multiplier, 2) rotate an image by 90 degrees,
 *  3) flip an image horizontally. These three operations will be implemented with a PIXELO structure which contains RGB value. These PIXEL structures will be moved around in order to change
 *  the whole image...
 *
*/	
	
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "bmplib.h"

/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */
int enlarge(PIXEL* original, int rows, int cols, int scale, 
	    PIXEL** new, int* newrows, int* newcols) 
{
	// Making sure we don't have an empty image file, and return a failure status
	if (rows <= 0 || cols <= 0)
	{
		 return -1;
	}
	
	// Variables being used inside the for loops and initialzing the new dimensions
	int row1, col1, row2, col2;
	*newrows = rows * scale;
	*newcols = cols * scale;

	// Creating new dimensions for the picture
	*new = (PIXEL*)malloc((*newrows) * (*newcols) * sizeof(PIXEL));
	
	// Using formulas and nested for loops, the image will be enlarged in small n/2 subsquares
	for (row1 = 0; row1 < rows; row1++)
	{
		for (col1 = 0; col1 < cols; col1++)
		{
			PIXEL* o = original + row1*cols + col1;
			for (row2 = 0; row2 < scale; row2++)
			{
				for(col2 = 0; col2 < scale; col2++)
				{
					PIXEL *n = (*new) + (row1 * scale + col2) * (*newcols) + (col1 * scale + row2);
					*n = *o;
				} 	
			}
		}
	}
	
  	return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read 
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 * 
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation, 
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL* original, int rows, int cols, int rotation,
	   PIXEL** new, int* newrows, int* newcols)
{
	// Variables to be used inside the for loops
	PIXEL* n;		
	int row = 0;
	int col = 0;
	int result = rotation / 90;

	// Making sure we don't have an empty image file, and return a failure status
	if (rows <= 0 || cols <= 0)
	{
		return -1;
	}

	// Checking if the rotation degree will affect the bounds of the array
	if (result % 2 == 0)
	{
		*newrows = rows;
		*newcols = cols;
	}
	else
	{
		*newrows = cols;	
		*newcols = rows;
	}
	
	// Dividing the rotation by 90 to get a simpler number and readaibility
	result %= 4;		
			
	// Allocating memory space for the modified picture		
	*new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));
	
	// Nested for loops and if statements will perform the rotate
	for (row = 0; row < rows; row++)
	{
		for (col = 0; col < cols; col++)
		{
			PIXEL* o = original + row * cols + col;
			
			if (result == 1 || result == -3)	// Checking for cases 90 and -270 which are the same
			{
				n = (*new) + (cols - 1 - col) * rows + row;
				*n = *o;
			}
			else if (result == 2 || result == -2)	// Checking for cases 180 and -180 which are the same
			{
				n = (*new) + (rows - 1 - row) * cols + (cols - 1 - col);
				*n = *o;
			}
			else if (result == 3 || result == -1)	// Checking for cases -90 and 270 which are the same
			{
				n = (*new) + (col * rows) + (rows - 1 - row);
				*n = *o;
			}
			else if (result == 0)	// Checking for the case where its any multiple of 360 
			{
				n = (*new) + (row*cols) + col;
				*n = *o;
			}
		}
	}
	
  	return 0;
}

/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile(). 
 * 
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip (PIXEL *original, PIXEL **new, int rows, int cols) 
{
	// Variables to be used inside the for loop
  	int row = 0;
	int col = 0;
	
	// Checking if an empty bmp image was given
	if ((rows <= 0) || (cols <= 0))
	{
		return -1;
	}
	
	// Allocating memory for the new flipped image
  	*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  	for (row=0; row < rows; row++)
    		for (col=0; col < cols; col++) 
			{
      			PIXEL* o = original + row*cols + col;
      			PIXEL* n = (*new) + row*cols + (cols-1-col);
      			*n = *o;
    		}

  	return 0;
}

// Parses the command line and checks for possible errors
void commandLineParser(int argc, char** argv, int* sflag, int* svalue, int* rflag, int* rvalue, int* fflag, int* oflag, char** ovalue, char** ivalue, int* err)
{
	extern char* optarg;
	extern int optind;
	int c = 0;
	static char usage[] = "\nusage: % bmptool [-s scale | -r degree | -f ] [-o output_file] [input_file] \n\n";
	// Parsing through the argv argument list
	while ((c = getopt(argc, argv, "s:r:fo:")) != -1)
	{	
		switch(c)
		{	
			case 's':
				if (*sflag == 1)
				{
					printf("Warning! two -s parameters have been passed, using the last one read.\n");
				}
				*sflag = 1;
				*svalue = atoi(optarg);
				break;
			case 'r':
				if (*rflag == 1)
				{
					printf("Warning! two -r parameters have been passed, using the last one read.\n");
				}
				*rflag = 1;
				*rvalue = atoi(optarg);
				break;
			case 'f':
				if (*fflag == 1)
				{
					printf("Warning! -f argument has already been read!\n");
				}
				*fflag = 1;
				break;
			case 'o':
				if (*oflag == 1)
				{
					printf("Warning! -o has already been used, using last filename passed.\n");
				}
				*oflag = 1;
				*ovalue = optarg;
				break;
			case '?':
				*err = 1;
				break;
			default:
				break;
		}
	}
	
	// Make the svalue a positive number if it is negative	
	if ((*sflag == 1) && (*svalue < 0))
	{
		printf("Warning! A negative number for the scale was read, changing the negative value into a positive number.\n");
		*svalue *= -1;	
	}
	else if ((*sflag == 1) && (*svalue == 0))
	{
		*svalue = 1;
	}
	
	// Checking if the rotate value is a multiple of 90, if it is not, then set the err flag...
	if (*rflag == 1 && (*rvalue % 90 != 0))
	{
		printf("ERROR! The rotate value must be a multiple of 90!");
		*err = 1;
	}
		
	// If an error was found then the err flag is set and checked here to end the program
	if (*err == 1)
	{
		exit(100);
	}
	
	// Getting input file name
	if (argc > optind)
	{
		*ivalue = argv[optind];
	}
	
	// Checking for any errors, this code was taken from the website explaining how getopt works which was provided by you from major assignment 1
	if (*err == 1)
	{
		printf("\nError: Invalid Format, try the proper usage format: \n");
		printf(usage);
		exit(100);
	}
	int temp;
	if (argc > optind + 1)
	{
		temp = optind + 1;
		
			while ( temp < argc )
			{
				printf("Argument: \"%s\" is invalid\n", argv[temp]);
				temp++;
			} 
		
		printf("%s", usage);
                  exit(100);
	}

}
	
int main(int argc, char** argv)
{
	int r = 0, c = 0/* nr = 0, nc = 0*/;
	PIXEL *b, *nb1, *nb2, *nb3;
	
	// These variables hold information pertinent to doing the image transformation
	int err = 0;
	int sflag = 0;
	int svalue = 0;
	int rflag = 0;
	int rvalue = 0;
	int fflag = 0;
	int oflag = 0;
	
	// Strings that hold the file locations
	char* ivalue = NULL; 	
	char* ovalue = NULL;
	// scale roate and flip
	// Parsing the command line arguments
	commandLineParser(argc, argv, &sflag, &svalue, &rflag, &rvalue, &fflag, &oflag, &ovalue, &ivalue, &err);
	
	// Reading input from stdin or an input file supplied by the user
	readFile(ivalue, &r, &c, &b);
	
	// 7 cases that take care of all the possibilities, memory will be freed where needed
	
	// case1
	if (fflag == 0 && rflag == 0 && sflag == 1)
	{
		enlarge(b, r, c, svalue, &nb1, &r, &c);
		writeFile(ovalue, r, c, nb1);
		free(b);
		free(nb1);
	}
		
	// case2
	if (fflag == 0 && rflag == 1 && sflag == 0)
	{
		rotate(b, r, c, rvalue, &nb1, &r, &c);
		writeFile(ovalue, r, c, nb1);
		free(b);
		free(nb1);
	}
	
	// case3
	if (fflag == 0 && rflag == 1 && sflag == 1)
	{
		enlarge(b, r, c, svalue, &nb1, &r, &c);
		rotate(nb1, r, c, rvalue, &nb2, &r, &c);
		writeFile(ovalue, r, c, nb2);
		free(b);
		free(nb1);
		free(nb2);
	}	
	
	// case4
	if (fflag == 1 && rflag == 0 && sflag == 0)
	{
		flip(b, &nb1, r, c);
		writeFile(ovalue, r, c, nb1);
		free(b);
		free(nb1);
	}
	
	// case5
	if (fflag == 1 && rflag == 0 && sflag == 1)
	{
		enlarge(b, r, c, svalue, &nb1, &r, &c);
		flip(nb1, &nb2, r, c);
		writeFile(ovalue, r, c, nb2);
		free(b);
		free(nb1);
		free(nb2);
	}
		
	// case6	
	if (fflag == 1 && rflag == 1 && sflag == 0)
	{
		rotate(b, r, c, rvalue, &nb1, &r, &c);
		flip(nb1, &nb2, r, c);
		writeFile(ovalue, r, c, nb2);
		free(b);
		free(nb1);
		free(nb2);
	}
	
	// case7
	if (fflag == 1 && rflag == 1 && sflag == 1)
	{
		enlarge(b, r, c, svalue, &nb1, &r, &c);
		rotate(nb1, r, c, rvalue, &nb2, &r, &c);
		flip(nb2, &nb3, r, c);
		writeFile(ovalue, r, c, nb3);
		free(b);
		free(nb1);
		free(nb2);
		free(nb3);
	}
	
	return 0;
}
