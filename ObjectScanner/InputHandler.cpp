#include "InputHandler.hpp"

#include <string>
#include <iostream>
#include <filesystem>
#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 
#include <conio.h> 

using namespace std;
namespace fs = std::experimental::filesystem;
static string fileExtensions[] = {".jpg", ".jpeg", ".png", ".PNG"};

int InputHandler::FindFiles(string dir) {
	int fileCounter = 0;
	for (fs::path p : fs::directory_iterator(dir)) {
		for (int i = 0; i < (sizeof(fileExtensions) / sizeof(*fileExtensions)); i++) {
			if (fileExtensions[i] == p.extension().string()) {
				paths[fileCounter] = p.string();
				fileCounter++;
			}
		}

	}
	return fileCounter;
}

InputHandler::InputHandler() {
	memset(paths, NULL, MAX_INPUT_FILES);
}

InputHandler::~InputHandler() {
}

int InputHandler::CheckIfDirExists(const fs::path& p)
{
	if (fs::exists(p))
		return 0;
	else
		return 1;
}

int InputHandler::CheckArguments(int argc, char* argv[]) {
	
	if (argc == 3) {
		string argString = argv[1];
		if (argString == "camera") {
			printf("Using camera as input!");
			return 0;
		}
	}

	if (argc < 3) {
		printf("Did you remember to enter two arguments when launching the program?\n");
		return 1;
	}

	else if (argc > 3) {
		printf("There was more than two argument entered... THAT IS UNACCEPTABLE!\n");
		return 1;
	}

	else if (CheckIfDirExists(argv[1])) {
		printf("The input directory entered doesn't exist!\n");
		printf("The program checked the path: %s\n", argv[1]);
		return 1;
	}

	else if (CheckIfDirExists(argv[2])) {
		printf("The output directory entered doesn't exist!\n");
		printf("The program checked the path: %s\n", argv[2]);
		return 1;
	}
	else {
		printf("The entered arguments are accepted. You may procede!\n");
		return 0;
	}
	return 1;
}

IplImage* InputHandler::WebcamCapture(int deviceID) {
	CvCapture* capture = 0;
	IplImage *frame =  0; // pointers to images
	capture = cvCaptureFromCAM(deviceID); // initialize capture device - CHANGED TO DEVICE 0 FOR INTERNAL WEBCAM
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

		return frame;

		
		
	}

	return NULL;

}



