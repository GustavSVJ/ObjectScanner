#pragma once
#include "cv.h" 


using namespace cv;
using namespace std;

class DotMaker
{
public:
	DotMaker();
	~DotMaker();

	void AddDots(int x, int size, Scalar color, int distance);
	void AddLine(Point pt1, Point pt2, Scalar color, int thickness);
	void ShowImage();

	Mat img;
};

