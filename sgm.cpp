/* sgm.cpp
 *
 * SGM stereo matching
 * CS 453 HW 8
 *
 * Name: Ben Bill Brown, Felix Yanwei Wang, Tao Peter Wang
 */

#include <stdio.h>
#include "imageLib.h"
#include "sgm.h"       // for OFFSET and ABS

// global variable for debugging output:
extern int verbose;

// initialize best costs b to 0
void clear_best(int *b, int ndisp)
{
    memset(b, 0, ndisp * sizeof(int)); // shorter version of the following:
	//~ for (int d = 0; d < ndisp; d++)
	//~ b[d] = 0;
}

// update best costs b according to SGM formula: b[d] = cost[d] + minimum of:
//  oldb[d]   + 0  (no disparity jump)
//  oldb[d-1] + p1 (disp jump of 1)
//  oldb[d+1] + p1 (disp jump of 1)
//  oldb[d']  + p2  (any disp jump, for any d' -- can precompute minimum!)
// once b has been computed, add its values to sumbest
void update_best(int *b, int *cost, int *sumbest, int p1, int p2, int ndisp)
{
    int oldb[ndisp];


    // FILL IN

    
    // copy b into oldb and also precompute min
    
    // compute new best costs b and also add b to sumbest

}



// compute disparity map 'disp' from images im1, im2
void computeSGM(CByteImage im1,      // source (reference) image
		CByteImage im2,      // destination (match) image
		CByteImage disp,     // computed disparities
		int p1,              // penalty for disparity jump of 1
		int p2,              // penalty for disparity jump of >= 2
		int dmin, int dmax)  // disparity search range
{
    CShape sh = im1.Shape();
    int w = sh.width, h = sh.height;
    int x, y, d;

    int ndisp = dmax - dmin + 1; // number of disparity levels

    sh.nBands = ndisp;      // need width * height * ndisp
    CIntImage cost(sh);     // cost volume, or disparity space image (DSI)
    CIntImage sumbest(sh);  // summed costs

    // compute costs

    // FILL IN

    // compute absolute differences for all pixels and all d = 0...ndisp-1
    // to convert d into actual disparities, add dmin, i.e.,
    // pixel (x, y) in im1 corresponds to pixel (x + d + dmin, y) in im2
    
    // if matching pixel out of bounds, assume max diff (255)


    
    // aggregate best costs in 4 directions for now (real SGM uses 8!)

    sumbest.ClearPixels();
    int b[ndisp]; // current best costs

    // left-to-right
    for (y = 0; y < h; y++) {
		clear_best(b, ndisp);
		for (x = 0; x < w; x++) {
			update_best(b, &cost.Pixel(x, y, 0), &sumbest.Pixel(x, y, 0), p1, p2, ndisp);
		}
    }

    // FILL IN -- add other 3 directions





    
    // find best disparity

    // FILL IN

    // for each pixel, find d with smallest sumcost

    // store d + dmin + OFFSET in disp image


}
