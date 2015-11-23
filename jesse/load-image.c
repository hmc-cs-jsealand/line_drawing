#include "png.h"

static png_structp png_ptr;
static png_infop info_ptr;

int loadimage(char *filename)
{
	infile = fopen(filename, "rb");

	unsigned char sig[8];
  
    fread(sig, 1, 8, infile);
    if (!png_check_sig(sig, 8))
        return 1;   /* bad signature */

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
      NULL);
    if (!png_ptr)
        return 4;   /* out of memory */
  
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 4;   /* out of memory */
    }

    if (setjmp(png_ptr->jmpbuf)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 2;
    }

	png_init_io(png_ptr, infile);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);



    //png_get_IHDR(png_ptr, info_ptr, &pwidth, &pheight, &bit_depth,
    //  &color_type, NULL, NULL, NULL);
    //*width = pwidth;
    //*height = pheight;
  
    return 0;
}


void write_png_file(char *filename, char *image_one_dim) {

 	FILE *fp = fopen(filename, "wb");
 	if(!fp) abort();

 	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
 	if (!png) abort();

 	png_infop info = png_create_info_struct(png);
 	if (!info) abort();

 	if (setjmp(png_jmpbuf(png))) abort();

 	png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
 	png_set_IHDR(
    	png,
    	info,
    	width, height,
    	8,
    	PNG_COLOR_TYPE_GRAY,
    	PNG_INTERLACE_NONE,
    	PNG_COMPRESSION_TYPE_DEFAULT,
    	PNG_FILTER_TYPE_DEFAULT
	);
  	png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  	int bytesWritten = 0;

   	png_bytep row = NULL;
   	row = (png_bytep) malloc( width );
 	int x,y;
 	for(y = 0; y < height; y++){
 		for(x = 0; x < width; x++){
 			row[x] = image_one_dim[y*width + x];
 			bytesWritten++;
 		}
 		png_write_row(png, row);
 	}

 	png_write_end(png, NULL);
 	printf("bytes written: %d\n",bytesWritten);

 	fclose(fp);
  free(row);

}