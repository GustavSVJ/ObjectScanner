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

				if (pixelValue < 100) {
					frameBinary->imageData[i] = 0;
				}
				else {
					frameBinary->imageData[i] = 255;
				}

			}

			cvShowImage("Binary Display", frameBinary);


			int xTop = 0, xBottom = 0, yLeft = 0, yRight = 0;
			enum Direction { North, NorthEast, East, SouthEast, South, SouthWest, West, NorthWest };


			for (int i = 0; i < frameBinary->width*frameBinary->height; i++) {
				unsigned char pixelValue = (unsigned char)frameBinary->imageData[i];

				if (pixelValue) {

					int startPixel = i;
					int currentPixel = startPixel;
					int pixelToCheck = -1;
					Direction movingTowards = East;
					char moveOn = 0;

					while (startPixel != pixelToCheck) {
						int firstToCheck = movingTowards - 2;
						if (firstToCheck < 0) {
							firstToCheck += 8;
						}

						int lookingAt = firstToCheck;

						while (moveOn == 0) {
							switch (lookingAt) {
							case North:
								pixelToCheck = currentPixel - frameBinary->width;
								if (!(pixelToCheck < 0) && (unsigned char)frameBinary->imageData[pixelToCheck] == 1) {
									moveOn = 1;
									currentPixel = pixelToCheck;
									movingTowards = North;
								}
								else if (firstToCheck != NorthEast){
									lookingAt = NorthEast;
								}
								else {
									moveOn = 1;
									movingTowards = East;
									currentPixel++;
								}
								break;
							case NorthEast:
								printf("NorthEast");
								break;
							case East:
								printf("East");
								break;
							case SouthEast:
								printf("SouthEast");
								break;
							case South:
								printf("South");
								break;
							case SouthWest:
								printf("SouthWest");
								break;
							case West:
								printf("West");
								break;
							case NorthWest:
								printf("NorthWest");
								break;
							default:
								printf("Error!");
								break;
							}
						}

						moveOn = 0;
					}









				}

			}

		}

		waitKey(0);
	}

	return 0;

}

