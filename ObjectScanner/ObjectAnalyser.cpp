#include "ObjectAnalyser.h"
#include <string>



using namespace cv;
using namespace std;


ObjectAnalyser::ObjectAnalyser()
{
	TopLeft = cvPoint(0, 0);
	BottomRight = cvPoint(0, 0);
	ImageWidth = 0;
	memset(histogramyaxis, 0, 256);
	memset(histogramxaxis, 0, 1100);
}

ObjectAnalyser::ObjectAnalyser(int imageHeight, int imageWidth) {
	TopLeft = cvPoint(imageHeight - 1, imageWidth - 1);
	BottomRight = cvPoint(0, 0);
	ImageWidth = imageWidth;
	memset(histogramyaxis, 0, 256);
	memset(histogramxaxis, 0, 1100);
}


ObjectAnalyser::~ObjectAnalyser()
{
}


void ObjectAnalyser::UpdateRoI(int pixel) {
	int x = pixel % ImageWidth;
	int y = pixel / ImageWidth;

	if (TopLeft.x + 2 > x) TopLeft.x = x - 2;
	if (TopLeft.y + 2 > y) TopLeft.y = y - 2;
	if (BottomRight.x - 2 < x) BottomRight.x = x + 2;
	if (BottomRight.y - 2 < y) BottomRight.y = y + 2;
}

int ObjectAnalyser::GetObjectHeight() {
	return BottomRight.y - TopLeft.y - 1;
}

int ObjectAnalyser::GetObjectWidth() {
	return BottomRight.x - TopLeft.x - 1;
}

IplImage * ObjectAnalyser::GetObjectImage(IplImage * fullImage, IplImage * objectImage) {

	int fullImageStartPixel = (TopLeft.x * TopLeft.y);
	int fullImageEndPixel = ((BottomRight.x - TopLeft.x) * (BottomRight.y - TopLeft.y));

	for (int j = 0; j < objectImage->height; j++) {
		int firstPixel = (TopLeft.y + 1 + j) * 1920 + (TopLeft.x + 1);

		for (int i = 0; i < objectImage->width; i++) {
			objectImage->imageData[i + j * (objectImage->widthStep)] = fullImage->imageData[i + firstPixel];
		}
	}

	ObjectImage = cvCloneImage(objectImage);
	return objectImage;
}

CvPoint ObjectAnalyser::GetObjectCenter(IplImage * input) {


	histox();
	histoy();

	return cvPoint(xpos(), ypos());



	/*

	unsigned int area = 0;
	unsigned int xSum = 0;
	unsigned int ySum = 0;
	unsigned int h[1080];
	unsigned int v[1920];


	for (int i = 0; i < ObjectImage->width; i++) {
		for (int j = 0; j < ObjectImage->height; j++) {
			v[i] += (unsigned char)ObjectImage->imageData[i + j * ObjectImage->widthStep];
			area += (unsigned char)ObjectImage->imageData[i + j * ObjectImage->widthStep];
		}
		xSum += i * v[i];
	}


	for (int j = 0; j < ObjectImage->height; j++) {
		for (int i = 0; i < ObjectImage->width; i++) {
			h[i] += (unsigned char)ObjectImage->imageData[i + j * ObjectImage->widthStep];
		}
		ySum += j * h[j];
	}

	CvPoint test = cvPoint((unsigned int)(xSum / area), (unsigned int)(ySum / area));

	cvDrawLine(input, test, test, cvScalar(255), 1, 8);

	return test;

	*/
}


void ObjectAnalyser::histoy() {
	for (int j = 0; j < ObjectImage->height; j++) {
		for (int i = 0; i < ObjectImage->width; i++) {
			unsigned char value = (unsigned char)ObjectImage->imageData[i + j * ObjectImage->widthStep];
			histogramyaxis[j] += value / 15;
		}
	}
}

void ObjectAnalyser::histox() {
	for (int j = 0; j < ObjectImage->height; j++) {
		for (int i = 0; i < ObjectImage->width; i++) {
			unsigned char value = (unsigned char)ObjectImage->imageData[i + j * ObjectImage->widthStep];
			histogramxaxis[i % ObjectImage->width] += value / 15;
		}
	}
}


int ObjectAnalyser::xpos() {
	int xplace = 0;
	int maxx = 0;
	for (int xscan = 0; xscan < ObjectImage->width; xscan++) {
		if (histogramxaxis[xscan] > maxx) {
			maxx = histogramxaxis[xscan];
			xplace = xscan;
		}
	}
	return xplace;
}

int ObjectAnalyser::ypos() {
	int yplace = 0;
	int maxy = 0;
	for (int yscan = 0; yscan < ObjectImage->height; yscan++) {
		if (histogramyaxis[yscan] > maxy) {
			maxy = histogramyaxis[yscan];
			yplace = yscan;
		}
	}
	return yplace;
}

IplImage* ObjectAnalyser::ScaleFilter(IplImage* Orginalimg) {
	IplImage *Scaledimage = cvCreateImage(cvSize(Orginalimg->width * 2, Orginalimg->height * 2), IPL_DEPTH_8U, 1);
	IplImage *Gausimage = cvCreateImage(cvSize(Orginalimg->width * 2, Orginalimg->height * 2), IPL_DEPTH_8U, 1);
	for (int j = 0; j < Orginalimg->height; j++) {
		for (int i = 0; i < Orginalimg->width; i++) {
			Scaledimage->imageData[i * 2 + j * 2 * Scaledimage->widthStep] = Orginalimg->imageData[i + j * Orginalimg->widthStep];
			Scaledimage->imageData[i * 2 + 1 + j * 2 * Scaledimage->widthStep] = Orginalimg->imageData[i + j * Orginalimg->widthStep];
			Scaledimage->imageData[i * 2 + j * 2 * Scaledimage->widthStep + Scaledimage->widthStep] = Orginalimg->imageData[i + j * Orginalimg->widthStep];
			Scaledimage->imageData[i * 2 + 1 + j * 2 * Scaledimage->widthStep + Scaledimage->widthStep] = Orginalimg->imageData[i + j * Orginalimg->widthStep];
			Gausimage->imageData[i * 2 + j * 2 * Scaledimage->widthStep] = Orginalimg->imageData[i + j * Orginalimg->widthStep];
			Gausimage->imageData[i * 2 + 1 + j * 2 * Scaledimage->widthStep] = Orginalimg->imageData[i + j * Orginalimg->widthStep];
			Gausimage->imageData[i * 2 + j * 2 * Scaledimage->widthStep + Scaledimage->widthStep] = Orginalimg->imageData[i + j * Orginalimg->widthStep];
			Gausimage->imageData[i * 2 + 1 + j * 2 * Scaledimage->widthStep + Scaledimage->widthStep] = Orginalimg->imageData[i + j * Orginalimg->widthStep];
		}
	}
	int TEMP1 = 0, TEMP2 = 0, TEMP3 = 0, TEMP4 = 0, TEMP5 = 0, TEMP6 = 0, TEMP7 = 0, TEMP8 = 0, TEMP9 = 0;
	int tempA;
	for (int x = 1; x < Scaledimage->widthStep - 1; x++)
	{
		for (int y = 1; y < Scaledimage->height - 1; y++)
		{
			TEMP1 = (unsigned char)Scaledimage->imageData[x - 1 + (y - 1) * Scaledimage->widthStep] * 1;
			TEMP2 = (unsigned char)Scaledimage->imageData[x + (y - 1) * Scaledimage->widthStep] * 2;
			TEMP3 = (unsigned char)Scaledimage->imageData[x + 1 + (y - 1) * Scaledimage->widthStep] * 1;

			TEMP4 = (unsigned char)Scaledimage->imageData[x - 1 + y * Scaledimage->widthStep] * 2;
			TEMP5 = (unsigned char)Scaledimage->imageData[x + y * Scaledimage->widthStep] * 4;
			TEMP6 = (unsigned char)Scaledimage->imageData[x + 1 + y * Scaledimage->widthStep] * 2;

			TEMP7 = (unsigned char)Scaledimage->imageData[x - 1 + (y + 1) * Scaledimage->widthStep] * 1;
			TEMP8 = (unsigned char)Scaledimage->imageData[x + (y + 1) * Scaledimage->widthStep] * 2;
			TEMP9 = (unsigned char)Scaledimage->imageData[x + 1 + (y + 1) * Scaledimage->widthStep] * 1;

			tempA = ((TEMP1 + TEMP2 + TEMP3 + TEMP4 + TEMP5 + TEMP6 + TEMP7 + TEMP8 + TEMP9) / 16);
			Gausimage->imageData[x + y * Scaledimage->widthStep] = tempA;


		}
	}

	return Gausimage;
}
