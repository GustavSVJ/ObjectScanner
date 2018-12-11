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

IplImage * ImageHandler::RemoveBackground(IplImage * input, IplImage * background) {
	IplImage * output = cvCreateImage(cvSize(input->width, input->height), IPL_DEPTH_8U, 3);

	for (int i = 0; i < input->height; i++) {
		for (int j = 0; j < input->width * 3; j++) {
			int pixelValue = (unsigned char)input->imageData[j + i * (input->widthStep)] - (unsigned char)background->imageData[j + i * (input->widthStep)];
			if (pixelValue < 0) pixelValue = 0;
			output->imageData[j + i * (input->widthStep)] = pixelValue;
		}
	}

	return output;
}

IplImage * ImageHandler::GetRedChannel(IplImage * input) {
	IplImage * output = cvCreateImage(cvSize(input->width, input->height), IPL_DEPTH_8U, 1);

	for (int i = 0; i < input->height; i++) {
		for (int j = 0; j < input->width; j++) {
			int pixelValue = (unsigned char)input->imageData[j * 3 + 2 + i * (input->widthStep)];
			output->imageData[j + i * (input->width)] = pixelValue;
		}
	}

	return output;
}

IplImage * ImageHandler::GetBlueChannel(IplImage * input) {
	IplImage * output = cvCreateImage(cvSize(input->width, input->height), IPL_DEPTH_8U, 1);

	for (int i = 0; i < input->height; i++) {
		for (int j = 0; j < input->width; j++) {
			int pixelValue = (unsigned char)input->imageData[j * 3 + i * (input->widthStep)];
			output->imageData[j + i * (input->width)] = pixelValue;
		}
	}

	return output;
}

IplImage * ImageHandler::GetGreenChannel(IplImage * input) {
	IplImage * output = cvCreateImage(cvSize(input->width, input->height), IPL_DEPTH_8U, 1);

	for (int i = 0; i < input->height; i++) {
		for (int j = 0; j < input->width; j++) {
			int pixelValue = (unsigned char)input->imageData[j * 3 + 1 + i * (input->widthStep)];
			output->imageData[j + i * (input->width)] = pixelValue;
		}
	}

	return output;
}



IplImage * ImageHandler::Colorize(IplImage * input, int threshold) {
	IplImage * output = cvCreateImage(cvSize(input->width, input->height), IPL_DEPTH_8U, 3);

	for (int i = 0; i < input->height; i++) {
		for (int j = 0; j < input->width; j++) {

			int redColor = (unsigned char)input->imageData[j * 3 + 2 + i * (input->widthStep)];
			int blueColor = (unsigned char)input->imageData[j * 3 + 0 + i * (input->widthStep)];
			int greenColor = (unsigned char)input->imageData[j * 3 + 1 + i * (input->widthStep)];

			if (blueColor > redColor && blueColor > greenColor && blueColor > threshold) {
				output->imageData[j * 3 + 0 + i * (output->widthStep)] = 255;
			}
			else {
				output->imageData[j * 3 + 0 + i * (output->widthStep)] = 0;
			}

			if (redColor > blueColor && redColor > greenColor && redColor > threshold) {
				output->imageData[j * 3 + 2 + i * (output->widthStep)] = 255;
			}
			else {
				output->imageData[j * 3 + 2 + i * (output->widthStep)] = 0;
			}

			if (greenColor > redColor && greenColor > blueColor && greenColor > threshold) {
				output->imageData[j * 3 + 1 + i * (output->widthStep)] = 255;
			}
			else {
				output->imageData[j * 3 + 1 + i * (output->widthStep)] = 0;
			}
		}
	}

	return output;


}

//int ImageHandler::FindObjects()

void ImageHandler::DisplayHistogram(IplImage * image, int lowValue, int highValue) {
	unsigned int hist[256];
	double max = 0;
	unsigned char commonValue;

	for (int i = 0; i < 256; i++) {
		hist[i] = 0;
	}

	for (int i = 0; i < image->width*image->height; i++) {
		unsigned char pixelValue = (unsigned char)image->imageData[i];

		hist[pixelValue]++;
	}

	for (int i = lowValue; i < highValue; i++) {
		if (hist[i] > max) {
			max = hist[i];
			commonValue = i;
		}
	}

	IplImage *histogram = cvCreateImage(cvSize(512, 512), 8, 3);

	double scale = 512 / max;

	for (int i = lowValue * 2; i < highValue * 2; i += 2) {
		cvLine(histogram, cvPoint(i, 512), cvPoint(i, 512 - (hist[i / 2] * scale)), cvScalar(40, 150, 0), 2);
	}

	cvShowImage("Histogram Display", histogram);
	printf("Most common value: %d\n\n", commonValue);
}