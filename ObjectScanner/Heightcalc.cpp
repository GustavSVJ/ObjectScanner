#include "Heightcalc.h"
#include <string>

using namespace cv;
using namespace std;

void Heightcalc::init_Height(double lightToObject, double baseLine, double focalLength, double aftandPrPixel) {
	LightToObject = lightToObject;
	Baseline = baseLine;
	FocalLength = focalLength;
	AfstandPrPixel = aftandPrPixel;
}

void Heightcalc::Reference_Calc(double x, int i) {

	double laengde = fabs((x - Mid))*AfstandPrPixel;

	double vinkel_scene = atan(LightToObject / Baseline);
	double vinkel_i = atan(laengde / FocalLength);
	double temp = vinkel_scene + vinkel_i;
	K[i] = tan(temp);
	C[i] = Baseline * tan(vinkel_scene);

}

double Heightcalc::CalcObjectHeight(double y, double ref, int i) {

	if (ref < Mid && y > Mid) {
		double x = fabs(ref - Mid) * AfstandPrPixel;
		double temp = fabs((Baseline * (K[i] - (x / FocalLength)) / (1 + K[i] * (x / FocalLength))) - C[i]);
		x = (fabs(y - ref) - fabs(ref - Mid))*AfstandPrPixel;
		double temp1 = fabs((Baseline * (K[i] - (x / FocalLength)) / (1 + K[i] * (x / FocalLength))) - C[i]);
		return (temp + temp1);
	}
	else {
		double x = fabs(y - Mid) * AfstandPrPixel;
		return (fabs((Baseline * (K[i] - (x / FocalLength)) / (1 + K[i] * (x / FocalLength))) - C[i]));
	}
}