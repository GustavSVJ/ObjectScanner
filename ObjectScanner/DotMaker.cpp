#include "DotMaker.hpp"
#include "highgui.h" 

DotMaker::DotMaker()
{
	img = Mat::zeros(cvSize(1920, 1080), CV_8UC3);
}


DotMaker::~DotMaker()
{
}

void DotMaker::AddLine(Point pt1, Point pt2, Scalar color, int thickness) {
	
	line(img, pt1, pt2, color, thickness, 8);
}

void DotMaker::AddDots(int x, int size, Scalar color, int distance) {
	int height = img.rows;

	int i = 0;

	while (i < height) {
		circle(img, Point(x,i), size, color, -1, 8, 0);
		i += distance;
	}
}

void DotMaker::ShowImage() {
	namedWindow("Dot Display", WINDOW_AUTOSIZE);
	imshow("Dot Display", img);
}


