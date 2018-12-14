#include "InputHandler.hpp"
#include "OutputHandler.hpp"
#include "DotMaker.hpp"
#include "ObjectAnalyser.h"
#include "ImageHandler.h"
#include "Heightcalc.h"

#include <stdio.h>
#include <string>
#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 
#include <conio.h> 



#define HIGH_THRESHOLD 80
#define COLOR_THRESHOLD 25
#define LOW_THRESHOLD 5
#define BACKGROUND_THRESHOLD 5
#define Y_distanceprpoint 1
#define offset 0.5
#define BottomGreen 20
#define PICTURE_PR_MOVE 1
int X_distance = 0;

int temp_index = 0;
double Xsort_ref[100];
using namespace cv;
using namespace std;

enum InputType { Camera, File };
enum Direction { North, NorthEast, East, SouthEast, South, SouthWest, West, NorthWest };
int checkPixel(IplImage *inputImage, IplImage *outputImage, int pixelToCheck);



int main(int argc, char* argv[]) {

	InputHandler input = InputHandler();
	if (input.CheckArguments(argc, argv)) {
		return 1;
	}

	InputType inputType = File;
	string argString = argv[1];
	int fileCount = 999;

	OutputHandler fileSaver = OutputHandler();
	DotMaker dotImage = DotMaker();

	if (argString == "camera") {
		inputType = Camera;
		dotImage.DisplayBlackImage();
		cvWaitKey(0);
	}
	else {
		fileCount = input.FindFiles(argv[1]);
	}

	IplImage *frameBackground;

	for (int moveX = 0; moveX < fileCount / (PICTURE_PR_MOVE + 1); moveX++) {

		if (inputType == Camera) {
			dotImage.DisplayBlackImage();
			frameBackground = input.WebcamCapture(1);
			fileSaver.SaveImage(frameBackground);
		}
		else
			frameBackground = cvLoadImage(input.paths[(PICTURE_PR_MOVE + 1) * moveX].c_str(), 1);

		for (int moveY = 0; moveY < PICTURE_PR_MOVE; moveY++) { //fileCount - 1

			IplImage *frameColor, *frameInput;

			if (inputType == Camera) {
				dotImage.Distance = 50;
				dotImage.DisplayDotImage(10 * moveY);
				frameInput = input.WebcamCapture(1);
				fileSaver.SaveImage(frameBackground);
			}
			else
				frameInput = cvLoadImage(input.paths[(PICTURE_PR_MOVE + 1) * moveX + moveY + 1].c_str(), 1);

			frameColor = ImageHandler::RemoveBackground(frameInput, frameBackground, BACKGROUND_THRESHOLD);

			IplImage *frameGrey = cvCreateImage(cvSize(frameColor->width, frameColor->height), IPL_DEPTH_8U, 1);
			cvCvtColor(frameColor, frameGrey, COLOR_RGB2GRAY);

			IplImage *frameBinary = cvCreateImage(cvSize(frameColor->width, frameColor->height), IPL_DEPTH_8U, 1);
			frameBinary = ImageHandler::MakeBinary(frameGrey, LOW_THRESHOLD);

			IplImage *objectMarkings = cvCreateImage(cvSize(frameBinary->width, frameBinary->height), IPL_DEPTH_8U, 1);
			cvSet(objectMarkings, cvScalar(0));

			IplImage *strongColors = ImageHandler::Colorize(frameColor, COLOR_THRESHOLD);

			IplImage *highThresholdBinary = cvCreateImage(cvSize(frameGrey->width, frameGrey->height), IPL_DEPTH_8U, 1);

			highThresholdBinary = ImageHandler::MakeBinary(frameGrey, HIGH_THRESHOLD);



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

					cvReleaseImage(&img);

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

			if (greenRoICounter == 2) {

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


				// Test af højde bestemmelses funktionen
				Heightcalc hej;
				// Beregner referencer for alle prikker, første gang..
				if (moveX == 0) {
					/*for (int i = 0; i < (greenRoICounter + redRoICounter + blueRoICounter); i++) {
						Xsort_ref[temp_index] = Xsort[i];
						hej.init_Height(0.52, 0.26, 0.00517, 0.000002677);
						hej.Reference_Calc(Xsort[i], temp_index);
						temp_index++;
						//printf("index = %d , K = %lf  ,  C = %lf  \n", i, hej.K[i], hej.C[i]);
					}*/

					for (int j = 0; j < 100; j++) {
						Xsort_ref[j] = Xsort[0];
						hej.init_Height(0.52, 0.26, 0.00517, 0.000002677);
						hej.Reference_Calc(Xsort[0], j);
					}


				}

				// Beregner Z for alle punkter 
				double Zsort[80] = { 0.0 };
				//if (moveX != 0) {
				for (int i = 0; i < (greenRoICounter + redRoICounter + blueRoICounter); i++) {
					Zsort[i] = hej.CalcObjectHeight(Xsort[i], Xsort_ref[moveY * (greenRoICounter + redRoICounter + blueRoICounter) + i], i);
				}
				//}

				//if (moveX % PICTURE_PR_MOVE == 0) {
					//X_distance++;
				//}
				for (int i = 0; i < (greenRoICounter + redRoICounter + blueRoICounter); i++) {
					Xsort[i] = moveX;
					if (i == 0)
						Ysort[i] = 0;
					else if (i == (greenRoICounter + redRoICounter + blueRoICounter - 1))
						Ysort[i] = BottomGreen;
					else
						Ysort[i] = Y_distanceprpoint * i + offset * moveY;

					Zsort[i] = Zsort[i] * 100;
				}


				for (int i = 0; i < (greenRoICounter + redRoICounter + blueRoICounter); i++) {
					printf("y %lf |", Ysort[i]);
					printf("x %lf |", Xsort[i]);
					printf("Z %lf\n", Zsort[i]);
					fileSaver.AddPoint(Xsort[i], Ysort[i], Zsort[i]);
				}
				printf("hej");

			}
			else
				printf("Unable to determine edges of measurement...\n");


			cvReleaseImage(&img);
			cvReleaseImage(&frameInput);
			cvReleaseImage(&frameColor);
			cvReleaseImage(&frameGrey);
			cvReleaseImage(&frameBinary);
			cvReleaseImage(&objectMarkings);
			cvReleaseImage(&strongColors);
			cvReleaseImage(&highThresholdBinary);

		}

		cvReleaseImage(&frameBackground);

		if (inputType == Camera) {
			if (waitKey(0) == 'c') {
				break;
			}
		}

	}
	fileSaver.WriteFile(argv[2]);
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