#pragma once

#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 


class DotDetector
{
public:
	DotDetector();
	~DotDetector();

	void DisplayHistogram(IplImage * image);

};

