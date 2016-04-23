/* sgm.cpp
 *
 * SGM stereo matching
 * CS 453 HW 8
 *
 * Name: Ben Bill Brown, Felix Yanwei Wang, Tao Peter Wang
 */

#include <stdio.h>
#include <climits>
#include <cstring>
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
	// *cost is a row
    int oldb[ndisp];
    
    // copy b into oldb and also precompute min
    std::memcpy(oldb, b, sizeof oldb);
	
	int i, d, currentMin;
	for (d = 0; d < ndisp; d++) {		// copy over
		b[d] = cost[d];
		currentMin = oldb[d];
		
		for (i = 0; i < ndisp; i++) {
			if (i == d)		// smaller that INT_MAX
				continue;
			
			if (ABS(d - i) == 1 && oldb[i] + p1 < currentMin)
				currentMin = oldb[i] + p1;
			else if (oldb[i] + p2 < currentMin)
				currentMin = oldb[i] + p2;
		}
		b[i] += currentMin;
	}
	
    // compute new best costs b and also add b to sumbest
	for (d = 0; d < ndisp; d++)
		sumbest[d] += b[d];
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
    // compute absolute differences for all pixels and all d = 0...ndisp-1
    // to convert d into actual disparities, add dmin, i.e.,
    // pixel (x, y) in im1 corresponds to pixel (x + d + dmin, y) in im2
    for (d = 0; d < ndisp; d++) {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				cost.Pixel(x, y, d) = !inBounds(x + d + dmin, y, w, h) ?
							255 :
							im1.Pixel(x, y, 0) 
						  - im2.Pixel(x + d + dmin, y, 0);
			}
		}
	}
    
    // aggregate best costs in 4 directions for now (real SGM uses 8!)

    sumbest.ClearPixels();
    int b[ndisp]; // current best costs, this is just an array
	
    // left-to-right
    for (y = 0; y < h; y++) {
		clear_best(b, ndisp);
		for (x = 0; x < w; x++) {
			update_best(b, &cost.Pixel(x, y, 0), &sumbest.Pixel(x, y, 0), p1, p2, ndisp);
		}
    }
	
    // right-to-left
	for (y = 0; y < h; y++) {
		clear_best(b, ndisp);
		for (x = w - 1; x >= 0; x--) {
			update_best(b, &cost.Pixel(x, y, 0), &sumbest.Pixel(x, y, 0), p1, p2, ndisp);
		}
	}
	
	// top-to-bottom
	for (x = 0; x < w; x++) {
		clear_best(b, ndisp);
		for (y = 0; y < h; y++) {
			update_best(b, &cost.Pixel(x, y, 0), &sumbest.Pixel(x, y, 0), p1, p2, ndisp);
		}
	}
	
	// bottom-to-top
	for (x = 0; x < w; x++) {
		clear_best(b, ndisp);
		for (y = h - 1; y >= 0; y--) {
			update_best(b, &cost.Pixel(x, y, 0), &sumbest.Pixel(x, y, 0), p1, p2, ndisp);
		}
	}
	
    
    // find best disparity
    // for each pixel, find d with smallest sumcost
    // store d + dmin + OFFSET in disp image
    int minCost;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			minCost = INT_MAX;
			for (d = 0; d < ndisp; d++) {
				if (sumbest.Pixel(x, y, d) < minCost) {
					minCost = sumbest.Pixel(x, y, d);
					disp.Pixel(x, y, 0) = d + dmin + OFFSET;
				}
			}
		}
	}
}

bool inBounds(int x, int y, int w, int h) {
    return x >= 0 && x < w && y >= 0 && y < h;
}
