#pragma once
#include "cv.h" 

using namespace cv;
using namespace std;

typedef enum {
	COLOR_RED = 0,
	COLOR_GREEN = 1,
	COLOR_BLUE = 2,
	ERROR = -1
} dotColor;

class ObjectAnalyser
{
private:
	int ImageWidth;
	int histogramyaxis[256];
	int histogramxaxis[1100];

	void histoy();
	void histox();

	int xpos();
	int ypos();

public:

	Point TopLeft, BottomRight;
	IplImage * ObjectImage;

	ObjectAnalyser();
	ObjectAnalyser(int imageHeight, int imageWidth);
	~ObjectAnalyser();

	void UpdateRoI(int pixel);

	int GetObjectHeight();

	int GetObjectWidth();

	int CheckForNoise(IplImage * referenceBinary);
	dotColor CheckDotColor(IplImage * strongColor);

	void GetObjectImage(IplImage * fullImage, IplImage * objectImage);

	CvPoint GetObjectCenter(IplImage * input);

};

