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

	DotMaker dotImage = DotMaker();
	dotImage.AddDots(900, 3, Scalar(255, 255, 255), 10);
	dotImage.ShowImage();

	namedWindow("Capture Display", WINDOW_AUTOSIZE);
	namedWindow("Greyscale Display", WINDOW_AUTOSIZE);
	namedWindow("Histogram Display", WINDOW_AUTOSIZE);
	namedWindow("Binary Display", WINDOW_AUTOSIZE);

	for (int j = 0; j < fileCount; j++) {

		IplImage *frameColor = cvLoadImage(input.paths[j].c_str(), 1);
		//IplImage *frameColor = input.WebcamCapture(0);

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

			ObjectAnalyser RoI[50];
			int RoICounter = 1;

			for (int i = 0; i < frameBinary->width*frameBinary->height; i++) {
				unsigned char pixelValue = (unsigned char)frameBinary->imageData[i];
				unsigned char objectPixelValue = (unsigned char)objectMarkings->imageData[i];

				if (pixelValue && objectPixelValue == 0 && checkPixel(frameBinary, objectMarkings, i)) {

					RoI[(unsigned char)objectMarkings->imageData[i]].UpdateRoI(i);
					frameBinary->imageData[i] = 150;
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
						RoI[(unsigned char)objectMarkings->imageData[currentPixel]].UpdateRoI(i);
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

					RoICounter++;

				}

			}

			IplImage *highThresholdBinary = cvCreateImage(cvSize(frameGrey->width, frameGrey->height), IPL_DEPTH_8U, 1);

			highThresholdBinary = ImageHandler::MakeBinary(frameGrey, 100);


			IplImage* img;
			int j = 0;
			for (int i = 1; i < RoICounter; i++) {
				img = cvCreateImage(cvSize(RoI[i].GetObjectWidth(), RoI[i].GetObjectHeight()), IPL_DEPTH_8U, 1);
				RoI[i].GetObjectImage(frameGrey, img);
				if (RoI[i].CheckForNoise(highThresholdBinary) == 0) {
					RoI[j] = RoI[i];
					j++;
				}
			}

			RoICounter = j;

			
			//Bearbejder fundne regions of interest
			for (int i = 0; i < RoICounter; i++) {
				img = cvCreateImage(cvSize(RoI[i].GetObjectWidth(), RoI[i].GetObjectHeight()), IPL_DEPTH_8U, 1);
				cvRectangle(frameGrey, RoI[i].TopLeft, RoI[i].BottomRight, CV_RGB(255, 255, 255), 1, 8);

				RoI[i].GetObjectImage(frameGrey, img);
				CvPoint smallImageCenter = RoI[i].GetObjectCenter(img);
				CvPoint bigImageCenter = cvPoint(smallImageCenter.x + 1 + RoI[i].TopLeft.x, smallImageCenter.y + 1 + RoI[i].TopLeft.y);

				cvLine(img, smallImageCenter, smallImageCenter, cvScalar(255), 1, 8);
				cvLine(frameColor, bigImageCenter, bigImageCenter, CV_RGB(255,0,0), 1, 8);

			}

			printf("Done!");

		}

		waitKey(0);
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