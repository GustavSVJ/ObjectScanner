#pragma once

#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 


class ImageHandler
{
public:
	static IplImage * MakeBinary(IplImage * input, int threshold);
	static IplImage * RemoveBackground(IplImage * input, IplImage * background)
	static void DisplayHistogram(IplImage * image);

private:
	ImageHandler();
	~ImageHandler();

};

