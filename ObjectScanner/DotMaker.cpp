#include "DotMaker.hpp"
#include "highgui.h" 
#include <opencv2/opencv.hpp>

DotMaker::DotMaker()
{
	img = cvCreateImage(cvSize(1920, 1080), 8, 3);
	namedWindow("DotPlot", WINDOW_AUTOSIZE);
}


DotMaker::~DotMaker()
{
}

void DotMaker::DisplayBlackImage() {

	//IplImage *img = cvCreateImage(cvSize(1920, 1080), 8, 3);

	cvZero(img);

	cvShowImage("DotPlot", img);
	cvWaitKey(500);
}

void DotMaker::DisplayDotImage(int offset) {

	int colorcount = 1;
	int y = ymin + offset;

	cvZero(img);

	while (y <= (ymax)) {

		if (colorcount == 1) {
			cvCircle(img, cvPoint(x, y), 5, CV_RGB(255, 0, 0), -1, 8, 0);
			colorcount++;
		}

		else if (colorcount == 2) {
			
			cvCircle(img, cvPoint(x, y), 5, CV_RGB(0, 0, 255), -1, 8, 0);
			colorcount = 1;
		}

		y += Distance;
	}

	cvCircle(img, cvPoint(x, ymin), 5, cvScalar(0, 255, 0), -1, 8, 0);
	cvCircle(img, cvPoint(x, ymax), 5, cvScalar(0, 255, 0), -1, 8, 0);

	cvShowImage("DotPlot", img);
	cvWaitKey(500);
}


