#include "GPIO_jts.h"

// base address of spi registers
#define SPI_BASE	0x3F204000

// SPI pointer that will be memory mapped when spiInit() is called
volatile unsigned int *spi_reg; //pointer to base of spi stuff


// memory mapping spi_reg register
void spiInit() {
	int  mem_fd;
	void *reg_map;

	// /dev/mem is a psuedo-driver for accessing memory in the Linux filesystem
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
	      printf("can't open /dev/mem \n");
	      exit(-1);
	}

	reg_map = mmap(
	  NULL,             	// Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       	// Size of mapped memory block
      PROT_READ|PROT_WRITE,	// Enable both reading and writing to the mapped memory
      MAP_SHARED,       	// This program does not have exclusive access to this memory
      mem_fd,           	// Map to /dev/mem
      SPI_BASE);   		    // Offset to GPIO peripheral

	if (reg_map == MAP_FAILED) {
      printf("gpio mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

	spi_reg = (volatile unsigned *)reg_map;
}

void spiStart(int freq, int settings) {
	pinMode(8, ALT0);
	pinMode(9, ALT0);
	pinMode(10, ALT0);
	pinMode(11, ALT0);

	spi_reg[2] = 250000000/freq;					// set clock rate
	spi_reg[0] = settings;
	spi_reg[0] |= 0x00000080;					// set TA bit high
}

int spiSendReceive(char send1, char send2) {
	spi_reg[0] |= 0x00000020;
	spi_reg[1] = send1;
	spi_reg[1] = send2;
	while( !(spi_reg[0] & 0x00010000));
	char message1 = spi_reg[1];
	char message2 = spi_reg[1];
	return (message1 << 7) + (message2 >> 1);
}

void adcRead(void) {
	printf("running main\n");
	pioInit();
	printf("pioInit done\n");
	spiInit();
	printf("spiInit done\n");
	int settings = 0x00000000;
	spiStart(100000, settings);
	printf("spiStart done\n");
	float message = spiSendReceive(0xD0,0x00);			// send 11010000 to enable ADC comms
	printf("Value was %5.0f \n", message);
	float val = message * 5.27 / 1024;
	printf("Voltage is %4.2f \n", val);
}





// penState 0th bit is pen up (0)/down (1), 1st bit is move enable
// 3 is movement enabled, pen down
// 2 is movement enabled, pen up
// 1 is movement disabled, pen down
// 0 is movement disabled, pen up
int penMover(char xpos, char ypos, char penState) {
	spi_reg[1] = penState;
	spi_reg[1] = xpos;
	spi_reg[1] = ypos;
	while( !(spi_reg[0] & 0x00010000));
	char message1 = spi_reg[1];
	char message2 = spi_reg[1];
	char message3 = spi_reg[1];
	if(message3 > 7)
		return 69;
	else
		return penMover(xpos, ypos, penState);
}


// Flags
// N, S, E, W
// a = NE, b = SE, c = SW, d = NW

void lineFollow(char* linePic, char* newPic, int height, int width,
		  int row, int col, int island, char flag) {
	// printf("Row: %d, Col: %d\n", row, col);
	if(flag == 'N') {
		if(row == 0) {}
		else {
			row -= 1;
			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else if(newPic[row*width + col] == 0) {
				newPic[row*width + col] = island;
				penMover(row, col, 3);
				lineFollow(linePic, newPic, height, width, row, col, island, 'N');
				lineFollow(linePic, newPic, height, width, row, col, island, 'd');
				lineFollow(linePic, newPic, height, width, row, col, island, 'a');
				penMover(row, col, 0);
			}
		}
	}
	if(flag == 'E') {
		if(col == width - 1){}
		else {
			col += 1;
			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else if(newPic[row*width + col] == 0) {
				newPic[row*width + col] = island;
				penMover(row, col, 3);
				lineFollow(linePic, newPic, height, width, row, col, island, 'E');
				lineFollow(linePic, newPic, height, width, row, col, island, 'a');
				lineFollow(linePic, newPic, height, width, row, col, island, 'b');
				penMover(row, col, 0);
			}
		}
	}
	if(flag == 'S') {
		if(row == height - 1) {}
		else {
			row += 1;
			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else if(newPic[row*width + col] == 0) {
				newPic[row*width + col] = island;
				penMover(row, col, 3);
				lineFollow(linePic, newPic, height, width, row, col, island, 'S');
				lineFollow(linePic, newPic, height, width, row, col, island, 'b');
				lineFollow(linePic, newPic, height, width, row, col, island, 'c');
				penMover(row, col, 0);
			}
		}
	}
	if(flag == 'W') {
		if(col == 0) {}
		else {
			col -= 1;
			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else if(newPic[row*width + col] == 0) {
				newPic[row*width + col] = island;
				penMover(row, col, 3);
				lineFollow(linePic, newPic, height, width, row, col, island, 'W');
				lineFollow(linePic, newPic, height, width, row, col, island, 'c');
				lineFollow(linePic, newPic, height, width, row, col, island, 'd');
				penMover(row, col, 0);
			}
		}
	}
	if(flag == 'a') {
		if( (row == 0) || (col == width - 1) ) {}
		else {
			row -= 1;
			col += 1;

			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else if(newPic[row*width + col] == 0) {
				newPic[row*width + col] = island;
				penMover(row, col, 3);
				lineFollow(linePic, newPic, height, width, row, col, island, 'a');
				lineFollow(linePic, newPic, height, width, row, col, island, 'N');
				lineFollow(linePic, newPic, height, width, row, col, island, 'E');
				lineFollow(linePic, newPic, height, width, row, col, island, 'd');
				lineFollow(linePic, newPic, height, width, row, col, island, 'b');
				penMover(row, col, 0);
			}
		}
	}
	if(flag == 'b') {
		if( (row == height - 1) || (col == width - 1) ){}
		else {
			row += 1;
			col += 1;

			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else if(newPic[row*width + col] == 0) {
				newPic[row*width + col] = island;
				penMover(row, col, 3);
				lineFollow(linePic, newPic, height, width, row, col, island, 'b');
				lineFollow(linePic, newPic, height, width, row, col, island, 'E');
				lineFollow(linePic, newPic, height, width, row, col, island, 'S');
				lineFollow(linePic, newPic, height, width, row, col, island, 'a');
				lineFollow(linePic, newPic, height, width, row, col, island, 'c');
				penMover(row, col, 0);
			}
		}
	}
	if(flag == 'c') {
		if( (row == height - 1) || (col == 0) ){}
		else {
			row += 1;
			col -= 1;

			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else if(newPic[row*width + col] == 0) {
				newPic[row*width + col] = island;
				penMover(row, col, 3);
				lineFollow(linePic, newPic, height, width, row, col, island, 'c');
				lineFollow(linePic, newPic, height, width, row, col, island, 'S');
				lineFollow(linePic, newPic, height, width, row, col, island, 'W');
				lineFollow(linePic, newPic, height, width, row, col, island, 'b');
				lineFollow(linePic, newPic, height, width, row, col, island, 'd');
				penMover(row, col, 0);
			}
		}
	}
	if(flag == 'd') {
		if( (row == 0) || (col == 0) ) {}
		else {
			row -= 1;
			col -= 1;

			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else if(newPic[row*width + col] == 0) {
				newPic[row*width + col] = island;
				penMover(row, col, 3);
				lineFollow(linePic, newPic, height, width, row, col, island, 'd');
				lineFollow(linePic, newPic, height, width, row, col, island, 'W');
				lineFollow(linePic, newPic, height, width, row, col, island, 'N');
				lineFollow(linePic, newPic, height, width, row, col, island, 'c');
				lineFollow(linePic, newPic, height, width, row, col, island, 'a');
				penMover(row, col, 0);
			}
		}
	}
}


char* mazeFollower(char* linePic, char* newPic, int height, int width) {



	// set island counter to 0
	int island = 2;

	// loop through newPic
	// if pixel is 0
	//		if pixel is not part of line
	//			set pixel to 1
	//		else
	//			set pixel to island
	//			run lineFollow on that pixel
	for(int row = 0; row < height; row++) {
		for(int col = 0; col < width; col++) {
			if(newPic[row*width + col] == 0) {
				if(linePic[row*width + col] == 0)
					newPic[row*width + col] = 1;
				else {
					newPic[row*width + col] = island;

					penMover(col, row, 2);
					penMover(col, row, 1);

					char flag[8] = {'N','S','E','W','a','b','c','d'};
					for(int i = 0; i < 8; i++) {
						// printf("flag is %c\n",flag[i]);
						lineFollow(linePic, newPic, height, width, row, col, island, flag[i]);
					}
					island++;
				}
			}
		}
	}
	return newPic;
}

