/* stereo2.cpp
 *
 * SGM (semi-global matching) stereo matcher
 * CS 453, HW 8
 *
 * optionally performs
 * - cross-checking
 * - hole filling
 */

static char usage[] = "usage: %s [options] p1 p2 dmin dmax im1 im2 disp1 disp2 \n\
\n\
  options:\n\
    -c cthresh    use crosschecking with given threshold\n\
    -f            fill holes in disparity maps\n\
    -s scale      scale factor for disparities (full range by default)\n\
    -q            quiet (turn off debugging output) \n";

#include <stdio.h>
#include <getopt.h>
#include "imageLib.h"
#include "sgm.h"
#include "crosscheck.h"

// global variable for debugging output
int verbose=1;

// prototype (defined below)
void mapdisp(CByteImage disp, int dmin, float scale);

int main(int argc, char **argv)
{
    // parameters controlled via command-line options:
    int cthresh = -1;
    int holeFilling = 0;
    float scale = -1;
    verbose = 1;

    // parse command-line arguments using the "getopt" utility
    int o;
    while ((o = getopt(argc, argv, "c:fs:q")) != -1)
		switch (o) {
		case 'c': cthresh = atoi(optarg); break;
		case 'f': holeFilling = 1; break;
		case 's': scale = atof(optarg); break;
		case 'q': verbose = 0; break;
		default: 
			fprintf(stderr, "Ignoring unrecognized option\n");
	}

    if (optind != argc-8) {
		fprintf(stderr, usage, argv[0]);
		exit(1);
    }

    int p1 = atoi(argv[optind++]);
    int p2 = atoi(argv[optind++]);
    int dmin = atoi(argv[optind++]);
    int dmax = atoi(argv[optind++]);
    char *im1name = argv[optind++];
    char *im2name = argv[optind++];
    char *disp1name = argv[optind++];
    char *disp2name = argv[optind++];

    try {
		CByteImage im1, im2;      // input images (gray or color)
		CByteImage disp1, disp2;  // gray images encoding disparities with offset 128
		ReadImageVerb(im1, im1name, verbose);
		ReadImageVerb(im2, im2name, verbose);

		// convert to grayscale in case images are color
		im1 = ConvertToGray(im1);
		im2 = ConvertToGray(im2);
		
		CShape sh = im1.Shape();
		CShape sh2 = im2.Shape();
		if (sh != sh2)
			throw CError("image shapes don't match");

		// assumes sh.nBands == 1
		disp1.ReAllocate(sh);
		disp2.ReAllocate(sh);

		if (verbose)
			fprintf(stderr, "matching 1->2, p1 = %d, p2 = %d, d = %d..%d\n",
				p1, p2, -dmax, -dmin);
		computeSGM(im1, im2, disp1, p1, p2, -dmax, -dmin);
		if (verbose)
			fprintf(stderr, "matching 2->1, p1 = %d, p2 = %d, d = %d..%d\n",
				p1, p2, dmin, dmax);
		computeSGM(im2, im1, disp2, p1, p2,  dmin,  dmax);
		
		if (cthresh >= 0) {
			if (verbose)
				fprintf(stderr, "cross-checking with cthresh = %d\n", cthresh);
			crosscheck(disp1, disp2, cthresh);
			crosscheck(disp2, disp1, cthresh);
		}

		if (holeFilling) {
			if (verbose)
				fprintf(stderr, "filling holes\n");
				fillHoles(disp1, 1);  // from left
				fillHoles(disp2, -1); // from right
		}

		// map disparities to full gray-level range
		if (scale < 0)
			scale = 255.0 / (dmax - dmin);
		if (verbose)
			fprintf(stderr, "scaling disparities by %.1f\n", scale);
		mapdisp(disp1, -dmin, -scale);
		mapdisp(disp2,  dmin,  scale);
	  
		WriteImageVerb(disp1, disp1name, verbose);
		WriteImageVerb(disp2, disp2name, verbose);
    }
    catch (CError &err) {
		fprintf(stderr, err.message);
		fprintf(stderr, "\n");
		exit(1);
    }

    return 0;
}


// map disparities to gray levels
void mapdisp(CByteImage disp, int dmin, float scale) {
    CShape sh = disp.Shape();
    int w = sh.width, h = sh.height;

    for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int val = disp.Pixel(x, y, 0);

			// leave 0 values alone
			if (val == 0)
				continue;

			disp.Pixel(x, y, 0) = __min(255, __max(0, (int)((val - dmin - OFFSET) * scale)));
		}
    }
}
