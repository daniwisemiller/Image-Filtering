//******************************************************************* 
//																	
//Program:	 Project 2 - Filtering Images						
//																	
//Author:	Danielle Wisemiller									
//Email:		dw640914@ohio.edu									
//																	
//Class Section: Section 100 Chelberg								
//																	
//Description: inputs a pgm image file and runs a filtering algorithm
// 				to clean the image and outputs a less noisy pgm file
//																	
//Date:		12/7/2017											
//																	
//*******************************************************************
#include <iostream>
#include <cmath>
#include <sys/time.h>
#include <cstdlib>
#include "pnm.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;



//******************************************************************
//                                                                  
//  Function:   timeval2sec
//                                                                  
//  Purpose:    Computes the number of seconds from a timeval of microseconds  
//                                                                  
//  Parameters: const timeval& T
//                                                                  
// Member/Global Variables: 
//
// Pre Conditions: none
//
// Post Conditions: returns seconds
//
// Calls:       timeofday
//                                                                  
//******************************************************************
double timeval2sec(const timeval& T) 
{
	// Constant for time unit conversion
	const double USEC_TO_SEC = 1000000.0;

	return ((double) T.tv_sec + (double) (T.tv_usec / USEC_TO_SEC));
}

//******************************************************************
//                                                                  
//  Function:   timeofday
//                                                                  
//  Purpose:    Computes the time of day in seconds.                                  
//                                                                  
//  Parameters: none
//                                                                  
// Member/Global Variables: timeval T
//
// Pre Conditions: none
//
// Post Conditions: returns the cost in dollars of shipping a
//                  package that weighs weight pounds a distance
//                  of distance miles.
//
// Calls:       main
//                                                                  
//******************************************************************
double timeofday() 
{
	timeval T;
	gettimeofday(&T, NULL);
	return (timeval2sec(T));
}


//******************************************************************
//																
//Function:	get_max
//																
//Purpose:	Gets the maximum values in an array for radix sort 
//																
//Parameters: int *array, int size
//																
// Member/Global Variables: none
//
// Pre Conditions: array and size are passed from the radix_sort function
//
// Post Conditions: returns int max
//
// Calls:		none
//																
//******************************************************************
int get_max(int *array, int size, size_t &num_compares)
{
	int max = array[0];		//sets max to default first element
	
	//iterate through array to find max value
	for (int i = 1; i < size; i++)
	{
		if (array[i] > max)
		{
			max = array[i];
		}
		num_compares++;
	}
	return max;
}
//******************************************************************
//																
//Function:	count_sort
//																
//Purpose:	Counting sort of array by digit represented in exp variable 
//																
//Parameters: int *array, int size, int exp
//																
// Member/Global Variables: none
//
// Pre Conditions: array and size are passed from the radix_sort function
//
// Post Conditions: saves results to array
//
// Calls:		none
//																
//******************************************************************
void count_sort(int *array, int size, int exp, size_t &num_compares)
{
	int final[size]; //output array for sorted numbers
	int i, count_array[10] = {0}; //initialization of for loop counters
 
	//Store new values in count_array
	for (i = 0; i < size; i++)
	{
		count_array[(array[i]/exp)%10]++;
	}
 
	//Adjust count_array spots to reflect real value
	for (i = 1; i < 10; i++)
	{
		count_array[i] += count_array[i-1];
	}
 
	//Fill in final output array with sorted values
	for (i = size - 1; i >= 0; i--)
	{
		final[count_array[(array[i]/exp)%10]-1] = array[i];
		count_array[(array[i]/exp)%10]--;
	}
 
	// Copy the output array to arr[], so that arr[] now
	// contains sorted numbers according to current digit
	for (i = 0; i < size; i++)
	{
		array[i] = final[i];
	}
}

//******************************************************************
//																
//Function:	radix_sort
//																
//Purpose:	Calls helper functions to perform radix sort for choosing median 
//																
//Parameters: int *array, int size, double rank, double neighborhood
//																
// Member/Global Variables: none
//
// Pre Conditions: array, size, rank, and neighborhood are passed 
//					from the process_image function
//
// Post Conditions: returns value at index rank*neighborhood size
//
// Calls:		get_max, count_sort
//																
//****************************************************************** 
int radix_sort(int *array, int size, double rank, double neighborhood, size_t &num_compares)
{
	//Find the maximum number to know number of digits (10's place, 100's place, etc)
	int max = get_max(array, size, num_compares);		//largest number to know number places to sort
 
	//Run counting sort for every digit
	for (int exp = 1; max/exp>0; exp *=10)
	{
		count_sort(array, size, exp, num_compares);
	}
	int matrix_size = neighborhood*neighborhood; 	//size of neighborhood
	int index = floor(rank*matrix_size);	//index of median in sorted array
	return array[index];
}

//******************************************************************
//																
//Function:	parse_args
//																
//Purpose:	Parse command line arguments into program and checks error conditions
//																
//Parameters: int argc, char **argv, double rank, double neighborhood
//																
// Member/Global Variables: none
//
// Pre Conditions: none
//
// Post Conditions: returns value at index rank*neighborhood size
//
// Calls:	none
//																
//****************************************************************** 
void parse_args(int argc, char **argv, double &rank, double &neighborhood)
{
	//Default variable values -- used when no tags are written in command line
	neighborhood = 1.0;
	rank = 0.5;

	//Reads for command line arguments (rank and neighborhood)
	for (size_t i=0; i<argc; ++i) 
	{
		//If rank tag used
		if (string(argv[i]) == "-r") 
		{
			//Parse argument
			if (i+1 < argc) 
			{
				i++;
				char *end=argv[i];
				rank=strtod(argv[i], &end);
				if (*end != '\0')
				{
					cerr << "Error in rank argument '" << argv[i] << "'" << endl;
					exit(EXIT_FAILURE);
				}
			} 
			else 
			{
				cerr << "No rank argument given." << endl;
				exit(EXIT_FAILURE);
			}
		} 
		//If neighborhood tag used
		else if (string(argv[i]) == "-n") 
		{
			//Parse argument
			if (i+1 < argc) 
			{
				i++;
				char *end=argv[i];
				neighborhood=strtod(argv[i], &end);
				if (*end != '\0') 
				{
					cerr << "Error in neighborhood argument '" << argv[i] << "'" << endl;
					exit(EXIT_FAILURE);
				}
			} 
			else 
			{
				cerr << "No neighborhood argument given." << endl;
				exit(EXIT_FAILURE);
			}
		}
	}
}

//******************************************************************
//																
//Function:	filter_image
//																
//Purpose:	filters input image to reduce noise and clarify picture 
//																
//Parameters: Image input, int size, double rank, double neighborhood
//																
// Member/Global Variables: none
//
// Pre Conditions: array, size, rank, and neighborhood are passed 
//					from the process_image function
//
// Post Conditions: returns value at index rank*neighborhood size
//
// Calls:		radix_sort
//																
//****************************************************************** 
Image filter_image(Image input, double rank, double neighborhood, size_t &num_compares)
{
	//Create an output image while reducing unusable border pixels
	int distance = (int)floor(neighborhood/2);	//measures distance in all directions from pixel
	Image output((input.rows-(distance*2)), (input.cols-(distance*2)), input.dims);		//new image size

	//Iterate by index (row,col)
	for(size_t row=distance; row<input.rows-distance; row++)
	{
		for (size_t column=distance; column<input.cols-distance; column++) 
		{
			int new_row = row-distance;
			int new_column = column-distance;
			int matrix_size = neighborhood*neighborhood;
			int all_neighbors[matrix_size];
			//find neighbors to pixel and get RGB values
			for (int i = 0; i<matrix_size; i++) 
			{
				all_neighbors[i] = input(new_row, new_column);
				if (new_column == (column + distance)) 
				{
					new_column = column - distance;
					new_row++;
				}
				else
				{
					new_column++;
				}
				num_compares++;
			}
			int value = radix_sort(all_neighbors, matrix_size, rank, neighborhood, num_compares);	//median value of neighborhood
			output(row-distance,column-distance) = value;	//sets all of neighborhood to median value
		}
	}
	return(output);
}

//******************************************************************
//																
//Function:	main
//																
//Purpose:	runs program
//																
//Parameters: int argc, char **argv
//																
// Member/Global Variables: none
//
// Pre Conditions: none
//
// Post Conditions: writes new filtered image to directory
//
// Calls:	parse_args, opengraphicfile, filter_image, writePNM
//																
//****************************************************************** 
int main (int argc, char **argv)
{
//Example call with args: prog2 -r 0.5 -n 25 < input.pgm > output.pgm
	double rank;	//rank to determine which "median" to choose
	double neighborhood;	//area of filtering
	size_t num_compares=0;
	double start=timeofday();
	
	//parse command line arguments
	parse_args(argc, argv, rank, neighborhood);
	//input image (file after '<')
	Image input;
	OpenGraphicFile(cin, input);
	//process image and set filtered image
	Image output = filter_image(input, rank, neighborhood, num_compares);
	//Comment in output file
	WritePNM(output, cout, "Noise filtered from image");
	
	cerr << "Image filter took " << timeofday() - start << " seconds" 
			 <<" on input photo of size "<<input.rows<< "x"<<input.cols<<endl;

	cerr << "Image filter took " << num_compares << " comparisons" 
			 <<" on input photo of size "<<input.rows<< "x"<<input.cols<<endl;

	return(EXIT_SUCCESS);
}
