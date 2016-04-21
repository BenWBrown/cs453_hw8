// Crosscheck disparity maps disp1 and disp2 and set all inconsistent
// disparities in disp1 to 0
void crosscheck(CByteImage disp1, CByteImage disp2, int cthresh);

// fill holes (0 values) in disp by copying previous non-zero value
// on each scanline moving in direction xincr
void fillHoles(CByteImage disp, int xincr);
