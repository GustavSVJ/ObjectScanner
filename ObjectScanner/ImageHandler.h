#pragma once

#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 


class ImageHandler
{
public:
	static IplImage * MakeBinary(IplImage * input, int threshold);
	static IplImage * RemoveBackground(IplImage * input, IplImage * background, int threshhold);
	static void DisplayHistogram(IplImage * image, int lowValue, int highValue);

	static IplImage * GetRedChannel(IplImage * input);
	static IplImage * GetBlueChannel(IplImage * input);
	static IplImage * GetGreenChannel(IplImage * input);

	static IplImage * Colorize(IplImage * input, int threshold);

private:
	ImageHandler();
	~ImageHandler();

};

