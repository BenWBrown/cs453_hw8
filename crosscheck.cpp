/* crosscheck.cpp
 *
 * disparity cross-checking and filling of resulting holes
 */


#include <stdio.h>
#include "imageLib.h"
#include "sgm.h"       // for OFFSET and ABS


bool inBounds(CShape sh, int x, int y) {
    int w = sh.width, h = sh.height;
    return ((0 <= x) && (x < w) && (0 <= y) && (y < h));
}



// Crosscheck disparity maps disp1 and disp2 and set all inconsistent
// disparities in disp1 to 0.  In detail:
//
// For each pixel in disp1, set it to 0 (meaning invalid disparity)
// if
// (- it is already 0)
//  - the corresponding pixel in disp2 is out of bounds or is 0
//  - its own disparity d1 differs by more than 'cthresh' from -d2,
//    where d2 is the corresponding disparity
// (don't forget to subtract OFFSET to convert values > 0 to disparities)
void crosscheck(CByteImage disp1, CByteImage disp2, int cthresh)
{
    CShape sh = disp1.Shape();
    int w = sh.width, h = sh.height;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int disp = disp1.Pixel(x, y, 0) - OFFSET;
            if (disp == 0 ||
                !inBounds(sh, x + disp, y) ||
                disp2.Pixel(x + disp, y, 0) == 0 ||
                ABS(disp1.Pixel(x, y, 0) + disp2.Pixel(x + disp, y, 0)) > cthresh)
				disp1.Pixel(x, y, 0) = 0;
        }
    }
}



// fill holes (0 values) in disp by copying previous non-zero value
// on each scanline moving in direction xincr
// (the pixels on the beginning of each line need to be filled from the other direction!)
void fillHoles(CByteImage disp, int xincr)
{
    CShape sh = disp.Shape();
    int w = sh.width, h = sh.height;
    bool beginning, right = (xincr == 1);
    int numBeginning, beginningFill;

    for (int y = 0; y < h; y++) {
        beginning = true;
        numBeginning = 0;
        for (int x = (right ? 0 : w - 1);
			right ? (x < w - 1) : (x > 0);
			x += xincr) {
			
            int d = disp.Pixel(x, y, 0);
            if (beginning && (d != 0) ) {	// actual beginning
                numBeginning = x;
                beginningFill = d;
				beginning = false;		// finished with beginning
            }
            if (disp.Pixel(x + xincr, y, 0) == 0)
                disp.Pixel(x + xincr, y, 0) = d;
        }
        int end = right ? 0 : w - 1;
        for (/* \(^_^)/ */; numBeginning >= 0; numBeginning--)
            disp.Pixel(end + xincr * numBeginning, y, 0) = beginningFill;
    }
}
