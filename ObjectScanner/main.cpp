#include "InputHandler.hpp"
#include "OutputHandler.hpp"
#include "DotMaker.hpp"

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



class Objects {

private:
	int ImageWidth;

public:
	Point topLeft, bottomRight;

	Objects() {
		topLeft = cvPoint(0,0);
		bottomRight = cvPoint(0, 0);
		ImageWidth = 0;
	}

	Objects(int imageHeight, int imageWidth) {
		topLeft = cvPoint(imageHeight - 1, imageWidth - 1 );
		bottomRight = cvPoint(0, 0);
		ImageWidth = imageWidth;
	}

	void updateRoI(int pixel) {
		int x = pixel % ImageWidth;
		int y = pixel / ImageWidth;

		if (topLeft.x + 2 > x) topLeft.x = x - 2;
		if (topLeft.y + 2 > y) topLeft.y = y - 2;
		if (bottomRight.x - 2 < x) bottomRight.x = x + 2;
		if (bottomRight.y - 2 < y) bottomRight.y = y + 2;
	}

};

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

			unsigned int hist[256];
			double max = 0;
			unsigned char commonValue;

			for (int i = 0; i < 256; i++) {
				hist[i] = 0;
			}

			for (int i = 0; i < frameGrey->width*frameGrey->height; i++) {
				unsigned char pixelValue = (unsigned char)frameGrey->imageData[i];

				hist[pixelValue]++;

				if (hist[pixelValue] > max) {
					max = hist[pixelValue];
					commonValue = pixelValue;
				}
			}



			IplImage *histogram = cvCreateImage(cvSize(512, 512), 8, 3);

			double scale = 512 / max;

			for (int i = 0; i < 512; i += 2) {
				cvLine(histogram, cvPoint(i, 512), cvPoint(i, 512 - (hist[i / 2] * scale)), cvScalar(40, 150, 0), 2);
			}

			cvShowImage("Histogram Display", histogram);
			printf("Image: %s\n", input.paths[j].c_str());
			printf("Most common value: %d\n\n", commonValue);

			IplImage *frameBinary = cvCreateImage(cvSize(frameColor->width, frameColor->height), IPL_DEPTH_8U, 1);

			cvCopy(frameGrey, frameBinary, 0);

			for (int i = 0; i < frameBinary->width*frameBinary->height; i++) {
				unsigned char pixelValue = (unsigned char)frameBinary->imageData[i];

				if (pixelValue < 15) {
					frameBinary->imageData[i] = 0;
				}
				else {
					frameBinary->imageData[i] = 255;
				}

			}

			cvShowImage("Binary Display", frameBinary);


			IplImage *objectMarkings = cvCreateImage(cvSize(frameBinary->width, frameBinary->height), IPL_DEPTH_8U, 1);
			cvSet(objectMarkings, cvScalar(0));

			Objects RoI[300];
			int RoICounter = 255;

			for (int i = 0; i < frameBinary->width*frameBinary->height; i++) {
				unsigned char pixelValue = (unsigned char)frameBinary->imageData[i];
				unsigned char objectPixelValue = (unsigned char)objectMarkings->imageData[i];
	
				if (pixelValue && objectPixelValue == 0 && checkPixel(frameBinary, objectMarkings, i)) {

					RoI[(unsigned char)objectMarkings->imageData[i]].updateRoI(i);
					frameBinary->imageData[i] = 150;
				}
		
				else if (pixelValue && objectPixelValue == 0) {

					RoI[RoICounter] = Objects(frameBinary->height, frameBinary->width);
					int startPixel = i;
					int currentPixel = startPixel;
					int pixelToCheck = -1;
					Direction movingTowards = East;
					char moveOn = 0;

					while (startPixel != pixelToCheck) {
						objectMarkings->imageData[currentPixel] = RoICounter;
						RoI[(unsigned char)objectMarkings->imageData[currentPixel]].updateRoI(i);
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
				
					RoICounter--;

				}

			}
			
			for (int i = 255; i > RoICounter; i--) {
				cvRectangle(frameBinary, RoI[i].topLeft, RoI[i].bottomRight, CV_RGB(0, 0, 255), 1, 8);
			}

			for (int i = 0; i < objectMarkings->width*objectMarkings->height; i++) {
				unsigned char pixelValue = (unsigned char)objectMarkings->imageData[i];
				if (pixelValue == 2) {
					Point pt = cvPoint(i % objectMarkings->width, i / objectMarkings->width);
					cvLine(frameColor, pt, pt, CV_RGB(255, 255, 0), 1, 8);
				}
				else if (pixelValue == 255) {
					Point pt = cvPoint(i % objectMarkings->width, i / objectMarkings->width);
					cvLine(frameColor, pt, pt, CV_RGB(255, 0, 0), 1, 8);
				}
			
			}
			printf("Done!");


		}

		waitKey(0);
	}

	return 0;

}



/*
int checkPixel(IplImage *image, int pixelToCheck, int *currentPixel, int firstToCheck, Direction *movingTowards, int *lookingAt) {
	if (!(pixelToCheck < 0) && (unsigned char)image->imageData[pixelToCheck] == 1) {
		*currentPixel = pixelToCheck;
		*movingTowards = North;
		//cvCircle(frameColor, cvPoint(*currentPixel / frameBinary->width, *currentPixel % frameBinary->width), 1, CV_RGB(0, 0, 255), 1, 8);
		return 1;
	}
	else if (firstToCheck != NorthEast) {
		*lookingAt = NorthEast;
		return 0;
	}
	else {
		*movingTowards = East;
		*currentPixel++;
		printf("Error!");
		return 1;
	}
}

*/


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