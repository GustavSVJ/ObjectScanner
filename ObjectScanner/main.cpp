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
	
	OutputHandler cube = OutputHandler();

	cube.AddPoint(0, 0, 0);
	cube.AddPoint(0, 0, 2);
	cube.AddPoint(0, 2, 2);
	cube.AddPoint(0, 2, 0);
	cube.AddPoint(2, 0, 0);
	cube.AddPoint(2, 0, 2);
	cube.AddPoint(2, 2, 2);
	cube.AddPoint(2, 2, 0);

	cube.WriteFile(argv[2]);

	return 0;

}