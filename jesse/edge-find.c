#include <stdlib.h>
#include <stdio.h>


char *file = "obama.png";
char *edgesfile = "edges.png";
char *thin_edgesfile = "thinedges.png";

FILE *infile;
char *edges;
char *thin_edges;

int width, height;



#include "load-image.c"
#include "compute-edges.c"
#include "lineThinning.c"

void main(int argc, char *argv[])
{
	int exitStatus = loadimage(file);
	printf("image loaded? %d \n",exitStatus);

	if(png_ptr == NULL)
		printf("png_ptr is null\n");
	if(info_ptr == NULL)
		printf("info_ptr is null\n");

	png_byte bit_depth 	= 	png_get_bit_depth(png_ptr, info_ptr);
	png_byte color_type	=	png_get_color_type(png_ptr, info_ptr);
	width 	=	png_get_image_width(png_ptr, info_ptr);
	height	=	png_get_image_height(png_ptr, info_ptr);

	printf("width: %d \n", width);
	printf("height: %d \n", height);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);		//standardize paletted images

    if (bit_depth == 16)
        png_set_strip_16(png_ptr);		//standardize 16 b/channel images to 8 b/channel

    if (color_type == PNG_COLOR_TYPE_GRAY &&
        bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);	//standardize grayscale images

    if (color_type & PNG_COLOR_MASK_ALPHA)	
        png_set_strip_alpha(png_ptr);		//get rid of unneeded alpha level data

    png_read_update_info(png_ptr,info_ptr);

    printf("allocating memory... \n");
    png_bytep row_pointers[height];
    int y;
  	for(y = 0; y < height; y++) {
    	row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));
  	}


  	printf("reading image into array... \n");
  	png_read_image(png_ptr, row_pointers);
  	fclose(infile);

  	printf("finding edges... \n");
	if(color_type == PNG_COLOR_TYPE_GRAY){
   		edges = find_edges_grayscale(row_pointers);
	}
	else{
		edges = find_edges_color(row_pointers);
	}

	for(y=0; y < height; y++){
		free(row_pointers[y]);
	}


	printf("writing edges file...\n");
	write_png_file(edgesfile,edges);

	char *thinedges = malloc(height*width*sizeof(char));
	printf("thinning edges...\n");
	for(int i = 0; i < height*width; i++){
		printf("%d \n",edges[i]);
		if(edges[i] = 255){
			thinedges[i] = 0;

		}
		else{
			thinedges[i] = 1;
		}
	}


	


	thinedges = lineThinning(thinedges, height, width, 1);


	for(int i = 0; i < height*width; i++){
		if(thinedges[i] = 0){
			edges[i] = 255;
		}
		else{
			edges[i] = 0;
		}
	}

	printf("writing output file...\n");
	write_png_file(thin_edgesfile,edges);

	free(edges);
	free(thinedges);

}

