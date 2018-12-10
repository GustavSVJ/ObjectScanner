#pragma once
#include "cv.h" 


using namespace cv;
using namespace std;

class DotMaker
{
public:
	DotMaker();
	~DotMaker();

	int x = 960; int ymin = 100; int ymax = 1000; int Distance = 200;

	void DisplayBlackImage();
	void DisplayDotImage(int offset);

	IplImage *img;
};

