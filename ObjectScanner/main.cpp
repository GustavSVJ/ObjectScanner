#include "FileHandler.hpp"

#include <stdio.h>
#include <string.h>
#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 
#include <conio.h> 

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {

	InputHandler inputHandler = InputHandler();

	if (inputHandler.CheckArguments(argc, argv)) {
		return 1;
	}

	if (!inputHandler.findFiles(argv[1])) {
		printf("No image files where found :/\n");
		return 1;
	}
	
	for (int i = 0; inputHandler.paths[i] != ""; i++) {
		Mat image;
		image = imread(inputHandler.paths[i], CV_LOAD_IMAGE_COLOR);
		
		namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
		imshow("Display window", image);                   // Show our image inside it.

		waitKey(0);                                          // Wait for a keystroke in the window
		//Do some image processing!
	}
	return 0;

}