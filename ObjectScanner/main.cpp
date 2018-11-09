#include "FileHandler.hpp"

#include <stdio.h>
#include <string.h>
#include "cv.h" 
#include "cxcore.h" 
#include "highgui.h" 
#include <conio.h> 

int main(int argc, char* argv[]) {
	printf("Hello World!\n");
	InputFiles tester = InputFiles();

	string paths[100];
	tester.findFiles("C:\\Users\\s164846\\Desktop", paths);

}