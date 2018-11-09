#include "FileHandler.hpp"

#include <string>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::experimental::filesystem;

int InputFiles::findFiles(string dir, string filePaths[]) {
	int fileCounter = 0;
	for (fs::path p : fs::directory_iterator(dir)) {
		if ()
		paths[fileCounter] = p.string();
		filePaths[fileCounter] = p.string();
		fileCounter++;
	}

	return fileCounter;
}

InputFiles::InputFiles() {
	memset(paths, NULL, MAX_INPUT_FILES);
}

