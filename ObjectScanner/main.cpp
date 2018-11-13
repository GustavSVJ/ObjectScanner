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


void webcamCapture(int i) {
	CvCapture* capture = 0;
	IplImage *frame, *frame_copy = 0; // pointers to images
	capture = cvCaptureFromCAM(1); // initialize capture device - CHANGED TO DEVICE 0 FOR INTERNAL WEBCAM
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 1920); // Set a cam property
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 1080); // Set a cam property
	cvSetCaptureProperty(capture, CV_CAP_PROP_EXPOSURE, -6); // Exposure -5s -> 2^(-5)
	cvSetCaptureProperty(capture, CV_CAP_PROP_GAIN, 1); // Set to 1, doesn't seem to make difference
	//cvSetCaptureProperty(capture, CV_CAP_WHITE_BALANCE_U, 0.1);
	//cvSetCaptureProperty(capture, CV_CAP_WHITE_BALANCE_V, 0.1);


	if (capture)
	{
		//if (!cvGrabFrame(capture))
		//	return;
		frame = cvRetrieveFrame(capture);
		frame = cvRetrieveFrame(capture);

		if (!frame)
			return;
		
		cvShowImage("Image", frame);

		//cvDestroyWindow("Image");
		char filename[25];

		sprintf(filename, "captures\\picture%d.png", i);
		cvSaveImage(filename, frame);
	}
}

int main(int argc, char* argv[]) {
	
	Mat img = Mat::zeros(cvSize(1920, 1080), CV_8UC3);

	line(img, Point(0, 800), Point(1920, 800), Scalar(255, 255, 255), 1, CV_AA);
	line(img, Point(0, 150), Point(1920, 150), Scalar(255, 255, 255), 1, CV_AA);
	addDots(img, 1450, 1, 15);
	namedWindow("Image", WINDOW_AUTOSIZE);// Create a window for display.
	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	//cvSetWindowProperty("Display window", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	imshow("Display window", img);                   // Show our image inside it.

	waitKey(0);                                          // Wait for a keystroke in the window
	//Do some image processing!
	
	int i = 0;

	while (1) {
		webcamCapture(i);
		i++;
		waitKey(0);
	}

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
	circle(img, center, size, Scalar(255, 255, 255), -1, 8, 0);
}

