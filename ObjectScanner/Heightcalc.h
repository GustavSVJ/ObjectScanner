#pragma once
#include "cv.h" 

using namespace cv;
using namespace std;

class Heightcalc
{

public:


	// variabler til højde beregning
	double LightToObject, Baseline, FocalLength, AfstandPrPixel;
	double K[80], C[80];
	int Mid = 960;

	double CalcObjectHeight(double y, double ref, int i);

	void Reference_Calc(double x, int i);

	void init_Height(double lightToObject, double baseLine, double focalLength, double aftandPrPixel);
};
