#pragma once

#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 


class ImageHandler
{
public:
	static IplImage * MakeBinary(IplImage * input, int threshold);
	static void DisplayHistogram(IplImage * image);

private:
	ImageHandler();
	~ImageHandler();

};

