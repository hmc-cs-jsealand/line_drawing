#include <math.h>

const double threshold = 0.15;



double average_difference(int x, int y,
						int c, int pxsize,
						png_bytep *row_pointers)
{
	int i, j;
	double average = 0.0;
	for(i = -1; i < 2; i++){
		png_bytep row = row_pointers[y + i];
		for(j = -1; j < 2; j++){
			png_bytep px = &(row[x * pxsize]);
			average += (double) px[c]/9.0;
		}
	}
	double avediff = 0.0;
	for(i = -1; i < 2; i++){
		png_bytep row = row_pointers[y + i];
		for(j = -1; j < 2; j++){
			png_bytep px = &(row[x * pxsize]);
			avediff += fabs(px[c] - average)/9.0;
		}
	}
	return avediff;
}


double average_diff_of_image(int c, int pxsize,
							png_bytep *row_pointers)
{
	double numPixels = height*width;
	double average = 0.0;
	int y;
	int x;
	for(y = 0; y < height; y++) {
    	png_bytep row = row_pointers[y];
    	for(x = 0; x < width; x++) {
      		png_bytep px = &(row[x * pxsize]);
      		average += (double) px[c]/numPixels;
      	}
   	}
    double ave_diff_of_image = 0.0;
    for(y = 0; y < height; y++) {
    	png_bytep row = row_pointers[y];
    	for(x = 0; x < width; x++) {
      		png_bytep px = &(row[x * pxsize]);
      		ave_diff_of_image += fabs(px[c] - average)/numPixels;
   		}
    }
    return ave_diff_of_image;
}

char * find_edges_grayscale(png_bytep *row_pointers)
{
	edges = (char *) malloc(height*width);
	char valueToWrite;
	double ave_diff_of_image = average_diff_of_image(0,1, row_pointers);
	int y;
	int x;
	double aveWindow;
	for(y = 0; y < height; y++) {
    	png_bytep row = row_pointers[y];
    	for(x = 0; x < width; x++) {
      		png_bytep px = &(row[x]);
      		if (x == 0
      			|| x == width - 1
      			|| y == 0
      			|| y == height -1){
      			valueToWrite = 0x0;
      		}
      		else {
      			aveWindow = average_difference(x,y,0,1, row_pointers);
      			if(aveWindow > threshold*ave_diff_of_image){
      				valueToWrite = 0xFF;
      			}
      			else{
      				valueToWrite = 0x0;
      			}
      		}
      		edges[y*width + x] = valueToWrite;
   		}
    }
    return edges;
}

char * find_edges_color(png_bytep *row_pointers)
{
	edges = (char *) malloc(height*width);
	char valueToWrite;
	double ave_diff_of_image[3] = {average_diff_of_image(0,3, row_pointers),
									average_diff_of_image(1, 3, row_pointers),
									average_diff_of_image(2, 3, row_pointers)};
	int y;
	int x;
	int c;
	double aveWindow;
	for(y = 0; y < height; y++) {
    	png_bytep row = row_pointers[y];
    	for(x = 0; x < width; x++) {
    		valueToWrite = 0x0;
      		for(c = 0; c < 3; c++){
      			png_bytep px = &(row[x]);
      			if (x == 0
      				|| x == width - 1
      				|| y == 0
      				|| y == height -1){
      				valueToWrite = 0x0;
      			}
      			else {
      				aveWindow = average_difference(x,y,c,3, row_pointers);
      				if(aveWindow > threshold*ave_diff_of_image[c]){
      					valueToWrite |= 0xFF;
      				}
      			}
      		}

      		edges[y*width + x] = valueToWrite;
   		}
    }
    return edges;
}