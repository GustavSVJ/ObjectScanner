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

	DotMaker dotImage = DotMaker();
	dotImage.AddDots(900, 3, Scalar(255, 255, 255), 10);
	dotImage.ShowImage();

	namedWindow("Capture Display", WINDOW_AUTOSIZE);


	while (1) {
		IplImage *frame = input.WebcamCapture(0);
		if (frame != NULL) {
			cvShowImage("Capture Display", frame);
		}
		
		waitKey(0);
	}

	return 0;

}




