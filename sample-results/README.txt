Sample results

1. with only the left-right pass:

$ ./stereo2 20 30 0 15 midd-stereo/tsukuba*.png tsu-lr-pass-only1.png tsu-lr-pass-only2.png
Reading image midd-stereo/tsukuba1.png
Reading image midd-stereo/tsukuba2.png
matching 1->2, p1 = 20, p2 = 30, d = -15..0
matching 2->1, p1 = 20, p2 = 30, d = 0..15
scaling disparities by 17.0
Writing image tsu-lr-pass-only1.png
Writing image tsu-lr-pass-only2.png


2. with all 4 passes:

$ ./stereo2 20 30 0 15 midd-stereo/tsukuba*.png tsu-disp1.png tsu-disp2.png
Reading image midd-stereo/tsukuba1.png
Reading image midd-stereo/tsukuba2.png
matching 1->2, p1 = 20, p2 = 30, d = -15..0
matching 2->1, p1 = 20, p2 = 30, d = 0..15
scaling disparities by 17.0
Writing image tsu-disp1.png
Writing image tsu-disp2.png

3. also with cross-checking:

$ ./stereo2 -c 1 20 30 0 15 midd-stereo/tsukuba*.png tsu-c1-disp1.png tsu-c1-disp2.png
Reading image midd-stereo/tsukuba1.png
Reading image midd-stereo/tsukuba2.png
matching 1->2, p1 = 20, p2 = 30, d = -15..0
matching 2->1, p1 = 20, p2 = 30, d = 0..15
cross-checking with cthresh = 1
scaling disparities by 17.0
Writing image tsu-c1-disp1.png
Writing image tsu-c1-disp2.png

4. also with hole filling:

$ ./stereo2 -c 1 -f 20 30 0 15 midd-stereo/tsukuba*.png tsu-c1-f-disp1.png tsu-c1-f-disp2.png
Reading image midd-stereo/tsukuba1.png
Reading image midd-stereo/tsukuba2.png
matching 1->2, p1 = 20, p2 = 30, d = -15..0
matching 2->1, p1 = 20, p2 = 30, d = 0..15
cross-checking with cthresh = 1
filling holes
scaling disparities by 17.0
Writing image tsu-c1-f-disp1.png
Writing image tsu-c1-f-disp2.png
