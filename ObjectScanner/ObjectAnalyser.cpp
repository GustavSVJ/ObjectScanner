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
	TopLeft = cvPoint(imageWidth - 1, imageHeight - 1);
	BottomRight = cvPoint(0, 0);

	ImageWidth = imageWidth;
	memset(histogramyaxis, 0, 256);
	memset(histogramxaxis, 0, 1100);
}


ObjectAnalyser::~ObjectAnalyser()
{
	if (!ObjectImage) {
		cvReleaseImage(&ObjectImage);
	}
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

void ObjectAnalyser::GetObjectImage(IplImage * fullImage, IplImage * objectImage) {

	int fullImageStartPixel = (TopLeft.x * TopLeft.y);
	int fullImageEndPixel = ((BottomRight.x - TopLeft.x) * (BottomRight.y - TopLeft.y));

	for (int j = 0; j < objectImage->height; j++) {
		int firstPixel = (TopLeft.y + 1 + j) * 1920 + (TopLeft.x + 1);

		for (int i = 0; i < objectImage->width; i++) {
			objectImage->imageData[i + j * (objectImage->widthStep)] = fullImage->imageData[i + firstPixel];
		}
	}

	ObjectImage = cvCloneImage(objectImage);
}

int ObjectAnalyser::CheckForNoise(IplImage * referenceBinary) {
	int fullImageStartPixel = (TopLeft.x * TopLeft.y);
	int fullImageEndPixel = ((BottomRight.x - TopLeft.x) * (BottomRight.y - TopLeft.y));

	for (int j = 0; j < ObjectImage->height; j++) {
		int firstPixel = (TopLeft.y + 1 + j) * 1920 + (TopLeft.x + 1);

		for (int i = 0; i < ObjectImage->width; i++) {
			if (referenceBinary->imageData[i + firstPixel] != 0 && ObjectImage->imageData[i + j * (ObjectImage->widthStep)] != 0) {
				return 0;
			}
		}
	}

	return 1;
}

dotColor ObjectAnalyser::CheckDotColor(IplImage * strongColor) {
	int fullImageStartPixel = (TopLeft.x * TopLeft.y);
	int fullImageEndPixel = ((BottomRight.x - TopLeft.x) * (BottomRight.y - TopLeft.y));

	int redFound = 0;
	int blueFound = 0;
	int greenFound = 0;

	for (int j = 0; j < ObjectImage->height; j++) {
		int firstPixel = (TopLeft.y + 1 + j) * 1920 * 3 + ((TopLeft.x + 1) * 3);

		for (int i = 0; i < ObjectImage->width; i++) {
			if (strongColor->imageData[i * 3 + 0 + firstPixel] != 0) blueFound = 1;
			if (strongColor->imageData[i * 3 + 1 + firstPixel] != 0) greenFound = 1;
			if (strongColor->imageData[i * 3 + 2 + firstPixel] != 0) redFound = 1;
		}
	}



	if (blueFound && !redFound && !greenFound) return COLOR_BLUE;
	if (!blueFound && redFound && !greenFound) return COLOR_RED;
	if (!blueFound && !redFound && greenFound) return COLOR_GREEN;

	return ERROR;
}

void ObjectAnalyser::GetObjectCenter(IplImage * input, double * xValue, double *yValue) {

	histox(input);
	histoy(input);

	*xValue = (xpos(input) / 2.0);
	*yValue = (ypos(input) / 2.0);



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


void ObjectAnalyser::histoy(IplImage* input) {
	for (int j = 0; j < input->height; j++) {
		for (int i = 0; i < input->width; i++) {
			unsigned char value = (unsigned char)input->imageData[i + j * input->widthStep];
			histogramyaxis[j] += value / 15;
		}
	}
}

void ObjectAnalyser::histox(IplImage* input) {
	for (int j = 0; j < input->height; j++) {
		for (int i = 0; i < input->width; i++) {
			unsigned char value = (unsigned char)input->imageData[i + j * input->widthStep];
			histogramxaxis[i % input->width] += value / 15;
		}
	}
}


double ObjectAnalyser::xpos(IplImage* input) {
	double xplace = 0;
	int maxx = 0;
	for (int xscan = 0; xscan < input->width; xscan++) {
		if (histogramxaxis[xscan] > maxx) {
			maxx = histogramxaxis[xscan];
			xplace = xscan;
		}
	}
	return xplace;
}

double ObjectAnalyser::ypos(IplImage* input) {
	double yplace = 0;
	int maxy = 0;
	for (int yscan = 0; yscan < input->height; yscan++) {
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
	for (int x = 1; x < Scaledimage->width - 1; x++)
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

void ObjectAnalyser::FindXY(ObjectAnalyser input[], int PointCounter, IplImage *frame, double Xstor[], double Ystor[]) {

	IplImage* img;

	for (int i = 0; i < PointCounter; i++) {
		double xValue = 0.0;
		double yValue = 0.0;
		img = cvCreateImage(cvSize(input[i].GetObjectWidth(), input[i].GetObjectHeight()), IPL_DEPTH_8U, 1);
		cvRectangle(frame, input[i].TopLeft, input[i].BottomRight, CV_RGB(255, 255, 255), 1, 8);
		input[i].GetObjectImage(frame, img);
		IplImage *Temp = input[i].ScaleFilter(img);
		input[i].GetObjectCenter(Temp, &xValue, &yValue);
		CvPoint smallImageCenter = cvPoint(xValue + 0.5, yValue + 0.5);
		CvPoint bigImageCenter = cvPoint(smallImageCenter.x + 1 + input[i].TopLeft.x, smallImageCenter.y + 1 + input[i].TopLeft.y);
		Xstor[i] = xValue + 1 + input[i].TopLeft.x;
		Ystor[i] = yValue + 1 + input[i].TopLeft.y;
		cvLine(img, smallImageCenter, smallImageCenter, cvScalar(255), 1, 8);
		cvLine(frame, bigImageCenter, bigImageCenter, CV_RGB(255, 0, 0), 1, 8);
		printf("Done!");
	}

}

void ObjectAnalyser::SortingArray(double yRed[], double yBlue[], double yGreen[], double Y_color[], double xRed[], double xBlue[], double xGreen[], double X_color[], int picturecounter) {


	int R = 0, G = 0, B = 0;
	int c = 0;
	int i = 0;
	char Done = 1;
	while (Done) {
		switch (c)
		{
		case 0: // Grøn
			if (G > 1) {

				Done = 0;
				break;
			}
			else {
				Y_color[i] = yGreen[G];
				X_color[i] = xGreen[G];
				G++;
				i++;
				c = 1;
				/*if (picturecounter == 0) {
					c = 1;
				}
				else {
					c = 2;
				}*/

				break;
			}


		case 1: // BLÅ
			if (yBlue[B] > 0) {
				Y_color[i] = yBlue[B];
				X_color[i] = xBlue[B];
				B++;
				i++;
				c = 2;
				break;
			}
			else
				if (yRed[R] > 0) {
					c = 2;
				}
				else
					c = 0;
			break;

		case 2: // Rød
			if (yRed[R] > 0) {
				Y_color[i] = yRed[R];
				X_color[i] = xRed[R];
				R++;
				i++;
				c = 1;
				break;
			}
			else {
				if (yBlue[B] > 0) {
					c = 1;
				}
				else {
					c = 0;
				}
				break;
			}
		}
	}
}


