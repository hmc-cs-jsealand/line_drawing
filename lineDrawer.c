#include <stdio.h>
#include <stdlib.h>
#include "lineThinning.c"
#include "islandFinder.c"

char picture[100] = {0,0,0,0,0, 0,0,0,0,0,
					 0,1,1,1,1, 1,0,0,0,0,
					 0,1,1,0,1, 1,1,1,1,0,
					 0,1,1,0,0, 1,1,1,1,0,
					 0,1,1,1,0, 0,1,1,0,0,

					 0,1,1,1,0, 0,0,0,0,0,
					 0,1,1,1,0, 0,0,0,0,0,
					 0,1,1,1,1, 1,1,1,1,0,
					 0,1,1,1,1, 1,0,0,1,0,
					 0,0,0,0,0, 0,0,0,0,0};

char simplePic[100] = {0,0,0,0,0, 0,0,0,0,0,
					   0,0,0,0,0, 0,0,0,0,0,
					   0,0,0,0,0, 0,0,0,0,0,
					   0,0,0,0,0, 1,1,0,0,0,
					   0,0,0,0,0, 0,1,1,0,0,

 					   0,0,0,1,0, 0,0,0,0,0,
					   0,0,0,1,0, 0,0,0,0,0,
					   0,0,0,1,0, 0,0,1,1,0,
					   0,0,0,1,0, 0,0,0,1,0,
					   0,0,0,0,0, 0,0,0,0,0};


void main() {
	int height = 10;
	int width = 10;

	char* linePic = malloc(height * width * sizeof(char));
	for(int row = 0; row < height; row++) {
		for(int col = 0; col < width; col ++) {
			linePic[row*width + col] = picture[row*width + col];
		}
	}

	// allocate memory for new island pic
	char* islandPic = malloc(height * width * sizeof(char));

	// initialize array of 0's for new island pic
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			islandPic[i*width + j] = 0;
		}
	}

	printf("Old picture\n");
	printPicture(linePic, height, width);
	linePic = lineThinning(linePic, height, width, 0);
	printf("New picture\n");
	printPicture(linePic, height, width);

	islandPic = islandFinder(linePic, islandPic, height, width);
	printPicture(islandPic, height, width);

	free(islandPic);
	free(linePic);
}
