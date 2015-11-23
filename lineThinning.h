void printPicture(char* pic, int height, int width) {
	for(int row = 0; row < height; row++) {
		int o = row * height;
		printf("%d%d%d%d%d%d%d%d%d%d\n", pic[o], pic[o+1], pic[o+2], pic[o+3], pic[o+4], pic[o+5], pic[o+6], pic[o+7], pic[o+8], pic[o+9]);
	}
}

char* lineThinning(char* edgePic, int height, int width, int step) {

	// allocate new array for marking pixels
	char thinLinePic[height * width];

	// initialize array at 0's
	for(int row = 0; row < height; row++) {
		for(int col = 0; col < width; col ++) {
			thinLinePic[row*width + col] = 0;
		}
	}

	// if step is true
	// 		mark S, E, NE, NW pixels
	// else
	//		mark N, W, SE, SW pixels
	// count marked pixels to know when we done
	int count = 0;
	for(int row = 1; row < height - 1; row++) {
		for(int col = 1; col < width - 1; col ++) {
			if(edgePic[row*width + col] == 1) {

				// if you add these to row and col
				// you go around a pixel in a clockwise circle
				// starting at N and ending at NW
				int rowOffset[8] = {-1,-1,0,1,1,1,0,-1};
				int colOffset[8] = {0,1,1,1,0,-1,-1,-1};
				int P[8];
				for(int i = 0; i < 8; i++) {
					P[i] = edgePic[(row+rowOffset[i])*width + (col+colOffset[i])];
				}

				// find number of nonzero neightbors and 0 -> 1 transitions
				int Ncount = 0;
				int Scount = 0;
				for(int i = 0; i < 8; i++) {
					if(P[i] == 1) {
						Ncount++;
						if(i == 0) {
							if(P[7] == 0)
								Scount++;
						}
						else {
							if(P[i - 1] == 0)
								Scount++;
						}
					}
				}
				
				// mark S, E, NE, NW directions
				if(step) {
					int TwoFourSix = P[0] * P[2] * P[4];
					int FourSixEight = P[2] * P[4] * P[6];
					if( (Ncount >= 2) && (Ncount <= 6) && (Scount == 1) && 
						(TwoFourSix == 0) && (FourSixEight == 0) && (P[5] != 0) ) {
						thinLinePic[row*width + col] = 1;
						count++;
					}
				}
				// mark N, W, SE, SW directions
				else {
					int TwoFourEight = P[0] * P[2] * P[6];
					int TwoSixEight = P[0] * P[4] * P[6];
					if( (Ncount >= 2) && (Ncount <= 6) && (Scount == 1) && 
						(TwoFourEight == 0) && (TwoSixEight == 0) && (P[5] != 0) ) {
						thinLinePic[row*width + col] = 1;
						count++;
					}
				}
			}
		}
	}
	// if we didn't find any pixels to delete, return modified edgePic
	if(count == 0 && step == 0) {
		// printf("returning\n");
		return edgePic;
	}

	// else modify edgePic and recurse
	else {
		// printf("deleting\n");
		// printPicture(thinLinePic, height, width);
		// delete pixels
		for(int row = 1; row < height - 1; row++) {
			for(int col = 1; col < width - 1; col ++) {
				if(thinLinePic[row*width + col] == 1)
					edgePic[row*width + col] = 0;
			}
		}
		if(step == 0)
			step = 1;
		else
			step = 0;
		return lineThinning(edgePic, height, width, step);
	}
}

