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

void main(void) {
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



void mazeFollower(int** maze, int height, int width) {
	pioInit();
	spiInit();
	int settings = 0x00000000;
	spiStart(100000, settings);
	int message = spiSendReceive()
	spiSendReceive()
}