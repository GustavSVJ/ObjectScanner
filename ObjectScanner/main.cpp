#include "InputHandler.hpp"
#include "OutputHandler.hpp"
#include "DotMaker.hpp"
#include "ObjectAnalyser.h"
#include "ImageHandler.h"

#include <stdio.h>
#include <string>
#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 
#include <conio.h> 



#define HIGH_THRESHOLD 75
#define LOW_THRESHOLD 10

using namespace cv;
using namespace std;

enum Direction { North, NorthEast, East, SouthEast, South, SouthWest, West, NorthWest };
int checkPixel(IplImage *inputImage, IplImage *outputImage, int pixelToCheck);



int main(int argc, char* argv[]) {

	InputHandler input = InputHandler();
	if (input.CheckArguments(argc, argv)) {
		return 1;
	}

	int fileCount = input.FindFiles(argv[1]);



	OutputHandler fileSaver = OutputHandler();
	DotMaker dotImage = DotMaker();

	/*

	dotImage.DisplayBlackImage();
	cvWaitKey(0);
	IplImage *frameBackground = input.WebcamCapture(1);
	frameBackground = input.WebcamCapture(1);
	fileSaver.SaveImage(frameBackground);

	dotImage.Distance = 50;

	int i = 0;

	while(1){
		dotImage.DisplayDotImage(10 * i);
		i++;
		IplImage *frameInput = input.WebcamCapture(1);
		fileSaver.SaveImage(frameInput);




		printf("done!");

	}

	return 0;

	*/

	namedWindow("Capture Display", WINDOW_AUTOSIZE);
	namedWindow("Greyscale Display", WINDOW_AUTOSIZE);
	namedWindow("Histogram Display", WINDOW_AUTOSIZE);
	namedWindow("Binary Display", WINDOW_AUTOSIZE);

	IplImage *frameBackground = cvLoadImage(input.paths[0].c_str(), 1);


	for (int j = 0; j < fileCount -1; j++) {

		IplImage *frameInput = cvLoadImage(input.paths[j+1].c_str(), 1);

		IplImage *frameColor = ImageHandler::RemoveBackground(frameInput, frameBackground);


		if (frameColor != NULL) {

			cvShowImage("Capture Display", frameColor);


			IplImage *frameGrey = cvCreateImage(cvSize(frameColor->width, frameColor->height), IPL_DEPTH_8U, 1);
			cvCvtColor(frameColor, frameGrey, COLOR_RGB2GRAY);

			cvShowImage("Greyscale Display", frameGrey);

			IplImage *frameBinary = cvCreateImage(cvSize(frameColor->width, frameColor->height), IPL_DEPTH_8U, 1);
			frameBinary = ImageHandler::MakeBinary(frameGrey, 15);

			cvShowImage("Binary Display", frameBinary);

			IplImage *objectMarkings = cvCreateImage(cvSize(frameBinary->width, frameBinary->height), IPL_DEPTH_8U, 1);
			cvSet(objectMarkings, cvScalar(0));

			IplImage *strongColors = ImageHandler::Colorize(frameColor, HIGH_THRESHOLD);

			IplImage *strongGrey = cvCreateImage(cvSize(frameColor->width, frameColor->height), IPL_DEPTH_8U, 1);
			cvCvtColor(strongColors, strongGrey, COLOR_RGB2GRAY);

			IplImage *highThresholdBinary = cvCreateImage(cvSize(strongGrey->width, strongGrey->height), IPL_DEPTH_8U, 1);

			highThresholdBinary = ImageHandler::MakeBinary(strongGrey, 10);



			ObjectAnalyser RoI[100];
			int RoICounter = 1;

			for (int i = 0; i < frameBinary->width*frameBinary->height; i++) {
				unsigned char pixelValue = (unsigned char)frameBinary->imageData[i];
				unsigned char objectPixelValue = (unsigned char)objectMarkings->imageData[i];

				if (pixelValue && objectPixelValue == 0 && checkPixel(frameBinary, objectMarkings, i)) {
					//RoI[(unsigned char)objectMarkings->imageData[i]].UpdateRoI(i);

				}

				else if (pixelValue && objectPixelValue == 0) {

					RoI[RoICounter] = ObjectAnalyser(frameBinary->height, frameBinary->width);
					int startPixel = i;
					int currentPixel = startPixel;
					int pixelToCheck = -1;
					Direction movingTowards = East;
					char moveOn = 0;

					while (startPixel != pixelToCheck) {
						objectMarkings->imageData[currentPixel] = RoICounter;
						RoI[(unsigned char)objectMarkings->imageData[currentPixel]].UpdateRoI(currentPixel);
						frameBinary->imageData[currentPixel] = 150;
						int firstToCheck = movingTowards - 2;
						if (firstToCheck < 0) {
							firstToCheck += 8;
						}

						int lookingAt = firstToCheck;

						while (moveOn == 0) {
							switch (lookingAt) {
							case North:
								pixelToCheck = currentPixel - frameBinary->width;
								if (!(pixelToCheck < 0) && (unsigned char)frameBinary->imageData[pixelToCheck] != 0) {
									moveOn = 1;
									currentPixel = pixelToCheck;
									movingTowards = North;
								}
								else if (firstToCheck != NorthEast) {
									lookingAt = NorthEast;
								}
								else {
									moveOn = 1;
									movingTowards = East;
									startPixel = pixelToCheck;
									printf("Error!");
								}
								break;
							case NorthEast:
								pixelToCheck = currentPixel - frameBinary->width + 1;
								if (!(pixelToCheck < 0) && (unsigned char)frameBinary->imageData[pixelToCheck] != 0) {
									moveOn = 1;
									currentPixel = pixelToCheck;
									movingTowards = NorthEast;

								}
								else if (firstToCheck != East) {
									lookingAt = East;
								}
								else {
									moveOn = 1;
									movingTowards = East;
									startPixel = pixelToCheck;
									printf("Error!");
								}
								break;
							case East:
								pixelToCheck = currentPixel + 1;
								if (!(pixelToCheck > frameBinary->width*frameBinary->height) && (unsigned char)frameBinary->imageData[pixelToCheck] != 0) {
									moveOn = 1;
									currentPixel = pixelToCheck;
									movingTowards = East;

								}
								else if (firstToCheck != SouthEast) {
									lookingAt = SouthEast;
								}
								else {
									moveOn = 1;
									movingTowards = East;
									startPixel = pixelToCheck;
									printf("Error!");
								}
								break;
							case SouthEast:
								pixelToCheck = currentPixel + frameBinary->width + 1;
								if (!(pixelToCheck > frameBinary->width*frameBinary->height) && (unsigned char)frameBinary->imageData[pixelToCheck] != 0) {
									moveOn = 1;
									currentPixel = pixelToCheck;
									movingTowards = SouthEast;
								}
								else if (firstToCheck != South) {
									lookingAt = South;
								}
								else {
									moveOn = 1;
									movingTowards = East;
									startPixel = pixelToCheck;
									printf("Error!");
								}
								break;
							case South:
								pixelToCheck = currentPixel + frameBinary->width;
								if (!(pixelToCheck > frameBinary->width*frameBinary->height) && (unsigned char)frameBinary->imageData[pixelToCheck] != 0) {
									moveOn = 1;
									currentPixel = pixelToCheck;
									movingTowards = South;
								}
								else if (firstToCheck != SouthWest) {
									lookingAt = SouthWest;
								}
								else {
									moveOn = 1;
									movingTowards = East;
									startPixel = pixelToCheck;
									printf("Error!");
								}
								break;
							case SouthWest:
								pixelToCheck = currentPixel + frameBinary->width - 1;
								if (!(pixelToCheck > frameBinary->width*frameBinary->height) && (unsigned char)frameBinary->imageData[pixelToCheck] != 0) {
									moveOn = 1;
									currentPixel = pixelToCheck;
									movingTowards = SouthWest;
								}
								else if (firstToCheck != West) {
									lookingAt = West;
								}
								else {
									moveOn = 1;
									movingTowards = East;
									startPixel = pixelToCheck;
									printf("Error!");
								}
								break;
							case West:
								pixelToCheck = currentPixel - 1;
								if (!(pixelToCheck < 0) && (unsigned char)frameBinary->imageData[pixelToCheck] != 0) {
									moveOn = 1;
									currentPixel = pixelToCheck;
									movingTowards = West;
								}
								else if (firstToCheck != NorthWest) {
									lookingAt = NorthWest;
								}
								else {
									moveOn = 1;
									movingTowards = East;
									startPixel = pixelToCheck;
									printf("Error!");
								}
								break;
							case NorthWest:
								pixelToCheck = currentPixel - frameBinary->width - 1;
								if (!(pixelToCheck < 0) && (unsigned char)frameBinary->imageData[pixelToCheck] != 0) {
									moveOn = 1;
									currentPixel = pixelToCheck;
									movingTowards = NorthWest;
								}
								else if (firstToCheck != North) {
									lookingAt = North;
								}
								else {
									moveOn = 1;
									movingTowards = East;
									startPixel = pixelToCheck;
									printf("Error!");
								}
								break;
							default:
								printf("Error!");
								break;
							}
						}
						//cvShowImage("Capture Display", objectMarkings);
						moveOn = 0;
					}

					IplImage* img;
					img = cvCreateImage(cvSize(RoI[RoICounter].GetObjectWidth(), RoI[RoICounter].GetObjectHeight()), IPL_DEPTH_8U, 1);
					RoI[RoICounter].GetObjectImage(frameGrey, img);
					cvRectangle(frameColor, RoI[RoICounter].TopLeft, RoI[RoICounter].BottomRight, CV_RGB(0, 255, 255), 1, 8);

					if (RoI[RoICounter].CheckForNoise(highThresholdBinary) == 0) {
						RoICounter++;
					}
					else {
						cvRectangle(frameColor, RoI[RoICounter].TopLeft, RoI[RoICounter].BottomRight, CV_RGB(255, 255, 0), 1, 8);
						RoI[RoICounter] = ObjectAnalyser(frameBinary->height, frameBinary->width);
					}

				}

			}


			ObjectAnalyser redRoI[25];
			int redRoICounter = 0;

			ObjectAnalyser greenRoI[25];
			int greenRoICounter = 0;

			ObjectAnalyser blueRoI[25];
			int blueRoICounter = 0;


			IplImage* img;
			for (int i = 1; i < RoICounter; i++) {
				img = cvCreateImage(cvSize(RoI[i].GetObjectWidth(), RoI[i].GetObjectHeight()), IPL_DEPTH_8U, 1);
				RoI[i].GetObjectImage(frameGrey, img);
				cvRectangle(frameColor, RoI[i].TopLeft, RoI[i].BottomRight, CV_RGB(255, 255, 255), 1, 8);
				if (RoI[i].CheckForNoise(highThresholdBinary) == 0) {

					dotColor color = RoI[i].CheckDotColor(strongColors);

					if (color == COLOR_RED) {
						redRoI[redRoICounter] = RoI[i];
						redRoICounter++;
						cvRectangle(frameColor, RoI[i].TopLeft, RoI[i].BottomRight, CV_RGB(255, 0, 0), 1, 8);
					}
					else if (color == COLOR_BLUE) {
						blueRoI[blueRoICounter] = RoI[i];
						blueRoICounter++;
						cvRectangle(frameColor, RoI[i].TopLeft, RoI[i].BottomRight, CV_RGB(0, 0, 255), 1, 8);
					}
					else if (color == COLOR_GREEN) {
						greenRoI[greenRoICounter] = RoI[i];
						greenRoICounter++;
						cvRectangle(frameColor, RoI[i].TopLeft, RoI[i].BottomRight, CV_RGB(0, 255, 0), 1, 8);
					}
					else {
						printf("The color couldn't be determined!");
					}
				}
			}



			// Her må vi starte med at rette
			double Xred[25] = { 0.0 };
			double Yred[25] = { 0.0 };
			// Her finder vi den absolutte X,Y position for prikken i billedet
			ObjectAnalyser::FindXY(redRoI, redRoICounter, frameGrey, Xred, Yred);


			double Xgreen[25] = { 0.0 };
			double Ygreen[25] = { 0.0 };

			// Her finder vi den absolutte X,Y position for prikken i billedet
			ObjectAnalyser::FindXY(greenRoI, greenRoICounter, frameGrey, Xgreen, Ygreen);

			double Xblue[25] = { 0.0 };
			double Yblue[25] = { 0.0 };

			// Her finder vi den absolutte X,Y position for prikken i billedet
			ObjectAnalyser::FindXY(blueRoI, blueRoICounter, frameGrey, Xblue, Yblue);
			double Ysort[80] = { 0.0 };
			double Xsort[80] = { 0.0 };
			// Sorteringsfunktionen. Sorterer prikkerne efter farve, så de rigtige koordinater kommer i rækkefølge
			ObjectAnalyser::SortingArray(Yred, Yblue, Ygreen, Ysort, Xred, Xblue, Xgreen, Xsort, 0);

			// Udskrivning af test af sorteringsfunktion
			for (int i = 0; i < 80; i++) {
				printf("y %lf\n", Ysort[i]);
				printf("x %lf\n", Xsort[i]);
			}

			// Test af højde bestemmelses funktionen
			ObjectAnalyser hej;

			hej.init_Height(0.54, 0.24, 0.00517, 0.000002677);
			hej.Reference_Calc(962);
			double Height = hej.CalcObjectHeight(999);

			printf("Height = %lf \n", Height);
			printf("hej");

		}

		//waitKey(0);
	}

	return 0;
}



int checkPixel(IplImage *inputImage, IplImage *outputImage, int pixelToCheck) {

	int borderPixelToCheck = 0;

	for (int i = -1; i < 2; i++) {
		borderPixelToCheck = pixelToCheck - inputImage->width + i;
		if (borderPixelToCheck > 0 && outputImage->imageData[borderPixelToCheck] != 0) {
			outputImage->imageData[pixelToCheck] = outputImage->imageData[borderPixelToCheck];
			return 1;
		}
	}

	borderPixelToCheck = pixelToCheck + 1;
	if (borderPixelToCheck < inputImage->width * inputImage->height && outputImage->imageData[borderPixelToCheck] != 0) {
		outputImage->imageData[pixelToCheck] = outputImage->imageData[borderPixelToCheck];
		return 1;
	}

	borderPixelToCheck = pixelToCheck - 1;
	if (borderPixelToCheck > 0 && outputImage->imageData[borderPixelToCheck] != 0) {
		outputImage->imageData[pixelToCheck] = outputImage->imageData[borderPixelToCheck];
		return 1;
	}

	for (int i = -1; i < 2; i++) {
		borderPixelToCheck = pixelToCheck + inputImage->width + i;
		if (borderPixelToCheck < inputImage->width * inputImage->height && outputImage->imageData[borderPixelToCheck] != 0) {
			outputImage->imageData[pixelToCheck] = outputImage->imageData[borderPixelToCheck];
			return 1;
		}
	}
	return 0;
}