#include "OutputHandler.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 
#include <conio.h> 

using namespace std;
namespace fs = std::experimental::filesystem;

OutputHandler::OutputHandler()
{
	memset(points, NULL, MAX_OUTPUT_POINTS);
	pointCounter = 0;
	imageCounter = 0;
}


OutputHandler::~OutputHandler()
{
}

void OutputHandler::AddPoint(float x, float y, float z) {
	points[pointCounter] = PointCloud();
	points[pointCounter].x = x;
	points[pointCounter].y = y;
	points[pointCounter].z = z;
	pointCounter++;
}

int OutputHandler::WriteFile(string outputDir) {

	ofstream outputFile;
	outputFile.open(outputDir + "\\pointCloud.ply");

	outputFile << "ply\n";
	outputFile << "format ascii 1.0\n";
	outputFile << "element vertex " << pointCounter << "\n";
	outputFile << "property float x\n";
	outputFile << "property float y\n";
	outputFile << "property float z\n";
	outputFile << "end_header\n";

	for (int i = 0; i < pointCounter; i++) {
		outputFile << points[i].x << " " << points[i].y << " " << points[i].z << "\n";
	}

	outputFile.close();


	return 0;
}

void OutputHandler::SaveImage(IplImage* frame) {
	char filename[25];

	sprintf(filename, "picture%04d.png", imageCounter);
	cvSaveImage(filename, frame);
	imageCounter++;
}
