#pragma once


#define MAX_INPUT_FILES 250
#include <string>

using namespace std;

class InputFiles {
public:
	InputFiles();

	int findFiles(string dir, string filePaths[]);
private:
	string paths[MAX_INPUT_FILES];
};
