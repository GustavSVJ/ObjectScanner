#pragma once
#include <string>
#include <filesystem>
#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 
#include <conio.h> 

#define MAX_OUTPUT_POINTS 1000

using namespace std;
namespace fs = std::experimental::filesystem;

class PointCloud {
public:
	float x;
	float y;
	float z;
};


class OutputHandler
{
public:
	OutputHandler();
	~OutputHandler();

	void AddPoint(float x, float y, float z);
	int WriteFile(string outputDir);
	void SaveImage(IplImage* frame);

	PointCloud points[MAX_OUTPUT_POINTS];
	int pointCounter;

private:
	int imageCounter;
};



