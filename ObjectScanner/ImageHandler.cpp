#include "ImageHandler.h"



ImageHandler::ImageHandler()
{
}


ImageHandler::~ImageHandler()
{
}

IplImage * ImageHandler::MakeBinary(IplImage * input, int threshold) {
	IplImage *binary = cvCreateImage(cvSize(input->width, input->height), IPL_DEPTH_8U, 1);

	cvCopy(input, binary, 0);

	for (int i = 0; i < binary->width*binary->height; i++) {
		unsigned char pixelValue = (unsigned char)binary->imageData[i];

		if (pixelValue < threshold) {
			binary->imageData[i] = 0;
		}
		else {
			binary->imageData[i] = 255;
		}
	}

	return binary;

}


//int ImageHandler::FindObjects()

void ImageHandler::DisplayHistogram(IplImage * image) {
	unsigned int hist[256];
	double max = 0;
	unsigned char commonValue;

	for (int i = 0; i < 256; i++) {
		hist[i] = 0;
	}

	for (int i = 0; i < image->width*image->height; i++) {
		unsigned char pixelValue = (unsigned char)image->imageData[i];

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
	printf("Most common value: %d\n\n", commonValue);
}