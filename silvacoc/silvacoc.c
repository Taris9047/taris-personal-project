#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parsor.h"

#ifndef __SILVACOC_C__
#define __SILVACOC_C__

#define VER "0.0.0.1"
#define BUFFER_SIZE 65535
#define LINE_BUFFER 200

int main(int argc, char *argv[])
{
	// Program title
	const char version_string[10] = VER;
	char program_name[255] = "Silvaco Compiler ";
	
	// File handlers
	FILE *fp;
	char* input_file = (char*)malloc(BUFFER_SIZE);
	char* line = (char*)malloc(LINE_BUFFER);
	char* filename = (char*)malloc(500);

	strcat(program_name, version_string);
	printf("%s\n", program_name);

	// Ok let's start the job
	//
	// Reading in deck file.
	if (argc <= 1) {
		printf("No input deck file has been given!!\n");
		return(-1);
	}

	// Take the first argument as input file
	filename = argv[1];
	fp = fopen(filename, "r");
	printf("Reading file: %s\n", filename);

	// Read in the file into 'input_file' buffer
	while( fgets(line, LINE_BUFFER, fp) != NULL ) {
		strcat(input_file, line);
	}

	// Print out the input file.
	printf("%s\n", input_file);
	
	// Finish up and close file pointer
	fclose(fp);
	free(input_file);
	free(filename);
	return(0);
}


#endif