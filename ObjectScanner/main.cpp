#include "InputHandler.hpp"
#include "OutputHandler.hpp"

#include <stdio.h>
#include <string>
#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 
#include <conio.h> 

using namespace cv;
using namespace std;

void addDot(Mat img, Point center, int size);
void addDots(Mat img, int x, int size, int distance);


void webcamCapture() {
	CvCapture* capture = 0;
	IplImage *frame, *frame_copy = 0; // pointers to images
	capture = cvCaptureFromCAM(0); // initialize capture device - CHANGED TO DEVICE 0 FOR INTERNAL WEBCAM
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 1920); // Set a cam property
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 1080); // Set a cam property
	//cvSetCaptureProperty(capture, CV_CAP_PROP_EXPOSURE, -1); // Exposure -5s -> 2^(-5)
	//cvSetCaptureProperty(capture, CV_CAP_PROP_GAIN, 1); // Set to 1, doesn't seem to make difference
	//cvSetCaptureProperty(capture, CV_CAP_WHITE_BALANCE_U, 0.1);
	//cvSetCaptureProperty(capture, CV_CAP_WHITE_BALANCE_V, 0.1);


	if (capture)
	{

		if (!cvGrabFrame(capture))
			return;

		frame = cvRetrieveFrame(capture);

		namedWindow("Image", WINDOW_AUTOSIZE);// Create a window for display.


		if (!frame)
			return;
		if (!frame_copy)
		{
			printf("Frame settings:\n Width: %d\n Height: %d\n", frame->width, frame->height);
			frame_copy = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, frame->nChannels);
			cvResizeWindow("Image", frame->width, frame->height);
		}
		if (frame->origin == IPL_ORIGIN_TL)
			cvCopy(frame, frame_copy, 0);
		else
			cvFlip(frame, frame_copy, 0);
		cvShowImage("Image", frame);
		if (cvWaitKey(5) > 0)
			return;




		/*
				if (!frame)
					return;
				if (!frame_copy)
				{
					frame_copy = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, frame->nChannels);
					cvResizeWindow(wName, frame->width, frame->height);
				}
				if (frame->origin == IPL_ORIGIN_TL)
					cvCopy(frame, frame_copy, 0);
				else
					cvFlip(frame, frame_copy, 0);
				cvShowImage(wName, frame_copy);


				cvSaveImage("myPicture.png", frame_copy);

				cvReleaseImage(&frame_copy);
				cvDestroyWindow("Hello World");
				*/

	}
}

int main(int argc, char* argv[]) {

	Mat img = Mat::zeros(cvSize(1920, 1080), CV_8UC3);

	line(img, Point(0, 1070), Point(1920, 1070), Scalar(255, 255, 255), 1, CV_AA);
	line(img, Point(0, 250), Point(1920, 250), Scalar(255, 255, 255), 1, CV_AA);
	addDots(img, 1300, 2, 40);

	namedWindow("Display window", WINDOW_NORMAL);// Create a window for display.
	cvSetWindowProperty("Display window", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	imshow("Display window", img);                   // Show our image inside it.

	waitKey(0);                                          // Wait for a keystroke in the window
	//Do some image processing!

	webcamCapture();

	return 0;

}

void addDots(Mat img, int x, int size, int distance) {
	int height = img.rows;

	int i = 0;

	while (i < height) {
		addDot(img, Point(x, i), size);
		i += distance;
	}


}

void addDot(Mat img, Point center, int size) {
	circle(img, center, size, Scalar(0, 0, 255), -1, 8, 0);
}

