#pragma once


#define MAX_INPUT_FILES 250
#include <string>
#include <filesystem>
#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 
#include <conio.h> 

using namespace std;
namespace fs = std::experimental::filesystem;

class InputHandler {
public:
	InputHandler();
	~InputHandler();

	int FindFiles(string dir);
	string paths[MAX_INPUT_FILES];

	int CheckIfDirExists(const fs::path& p);
	int CheckArguments(int argc, char* argv[]);
	IplImage* WebcamCapture(int deviceID);
};
