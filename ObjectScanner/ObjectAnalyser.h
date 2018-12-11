#pragma once
#include "cv.h" 

using namespace cv;
using namespace std;

class ObjectAnalyser
{
private:
	int ImageWidth;
	int histogramyaxis[256];
	int histogramxaxis[1100];

	void histoy(IplImage* input);
	void histox(IplImage* input);

	double xpos(IplImage* input);
	double ypos(IplImage* input);

public:

	Point TopLeft, BottomRight;
	IplImage * ObjectImage;

	ObjectAnalyser();
	ObjectAnalyser(int imageHeight, int imageWidth);
	~ObjectAnalyser();

	void UpdateRoI(int pixel);

	int GetObjectHeight();

	int GetObjectWidth();

	IplImage * GetObjectImage(IplImage * fullImage, IplImage * objectImage);

	void GetObjectCenter(IplImage * input, double * xValue, double *yValuet);

	IplImage* ScaleFilter(IplImage* Orginalimg);

	static void FindXY(ObjectAnalyser input[], int PointCounter, IplImage *frame, double Xstor[], double Ystor[]);

	static void SortingArray(double Red[], double Blue[], double Green[], double Y_color[], double xRed[], double xBlue[], double xGreen[], double X_color[]);

};