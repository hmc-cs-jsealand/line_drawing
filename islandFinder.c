// Flags
// N, S, E, W
// a = NE, b = SE, c = SW, d = NW

void getIsland(char* linePic, char* newPic, int height, int width,
		  int row, int col, int island, char flag) {
	if(flag == 'N') {
		if(row == 0) {}
		else {
			row -= 1;
			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else {
				newPic[row*width + col] = island;
				getIsland(linePic, newPic, width, height, row, col, island, 'd');
				getIsland(linePic, newPic, width, height, row, col, island, 'N');
				getIsland(linePic, newPic, width, height, row, col, island, 'a');
			}
		}
	}
	if(flag == 'E') {
		if(col == width - 1){}
		else {
			col += 1;
			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else {
				newPic[row*width + col] = island;
				getIsland(linePic, newPic, width, height, row, col, island, 'a');
				getIsland(linePic, newPic, width, height, row, col, island, 'E');
				getIsland(linePic, newPic, width, height, row, col, island, 'b');
			}
		}
	}
	if(flag == 'S') {
		if(row == height - 1) {}
		else {
			row += 1;
			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else {
				newPic[row*width + col] = island;
				getIsland(linePic, newPic, width, height, row, col, island, 'b');
				getIsland(linePic, newPic, width, height, row, col, island, 'S');
				getIsland(linePic, newPic, width, height, row, col, island, 'c');
			}
		}
	}
	if(flag == 'W') {
		if(col == 0) {}
		else {
			col -= 1;
			if(linePic[row*width + col] == 0)
				newPic[row*width + col] = 1;
			else {
				newPic[row*width + col] = island;
				getIsland(linePic, newPic, width, height, row, col, island, 'c');
				getIsland(linePic, newPic, width, height, row, col, island, 'W');
				getIsland(linePic, newPic, width, height, row, col, island, 'd');
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
			else {
				newPic[row*width + col] = island;
				getIsland(linePic, newPic, width, height, row, col, island, 'd');
				getIsland(linePic, newPic, width, height, row, col, island, 'N');
				getIsland(linePic, newPic, width, height, row, col, island, 'a');
				getIsland(linePic, newPic, width, height, row, col, island, 'E');
				getIsland(linePic, newPic, width, height, row, col, island, 'b');
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
			else {
				newPic[row*width + col] = island;
				getIsland(linePic, newPic, width, height, row, col, island, 'a');
				getIsland(linePic, newPic, width, height, row, col, island, 'E');
				getIsland(linePic, newPic, width, height, row, col, island, 'b');
				getIsland(linePic, newPic, width, height, row, col, island, 'S');
				getIsland(linePic, newPic, width, height, row, col, island, 'c');
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
			else {
				newPic[row*width + col] = island;
				getIsland(linePic, newPic, width, height, row, col, island, 'b');
				getIsland(linePic, newPic, width, height, row, col, island, 'S');
				getIsland(linePic, newPic, width, height, row, col, island, 'c');
				getIsland(linePic, newPic, width, height, row, col, island, 'W');
				getIsland(linePic, newPic, width, height, row, col, island, 'd');
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
			else {
				newPic[row*width + col] = island;
				getIsland(linePic, newPic, width, height, row, col, island, 'c');
				getIsland(linePic, newPic, width, height, row, col, island, 'W');
				getIsland(linePic, newPic, width, height, row, col, island, 'd');
				getIsland(linePic, newPic, width, height, row, col, island, 'N');
				getIsland(linePic, newPic, width, height, row, col, island, 'a');
			}
		}
	}
}


char* islandFinder(char* linePic, char* newPic, int height, int width) {



	// set island counter to 0
	int island = 2;

	// loop through newPic
	// if pixel is 0
	//		if pixel is not part of line
	//			set pixel to 1
	//		else
	//			set pixel to island
	//			run getIsland on that pixel
	for(int row = 0; row < height; row++) {
		for(int col = 0; col < width; col++) {
			if(newPic[row*width + col] == 0) {
				if(linePic[row*width + col] == 0)
					newPic[row*width + col] = 1;
				else {
					newPic[row*width + col] = island;
					char flag[8] = {'N','S','E','W','a','b','c','d'};
					for(int i = 0; i < 8; i++) {
						getIsland(linePic, newPic, height, width, row, col, island, flag[i]);
					}
					island++;
				}
			}
		}
	}
	return newPic;
}

