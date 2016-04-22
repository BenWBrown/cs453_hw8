// compute disparity map 'disp' from images im1, im2
void computeSGM(CByteImage im1,      // source (reference) image
		CByteImage im2,      // destination (match) image
		CByteImage disp,     // computed disparities
		int p1,              // penalty for disparity jump of 1
		int p2,              // penalty for disparity jump of >= 2
		int dmin, int dmax); // disparity search range

// check if a pixel is in bound
bool inBounds(int, int, int, int);


#define OFFSET 128

#define ABS(x) ((x) < 0 ? (-(x)) : (x))
