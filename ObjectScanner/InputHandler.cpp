#include "InputHandler.hpp"

#include <string>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::experimental::filesystem;
static string fileExtensions[] = {".jpg", ".jpeg", ".png", ".PNG"};

int InputHandler::findFiles(string dir) {
	int fileCounter = 0;
	for (fs::path p : fs::directory_iterator(dir)) {
		for (int i = 0; i < (sizeof(fileExtensions) / sizeof(*fileExtensions)); i++) {
			if (fileExtensions[i] == p.extension().string()) {
				paths[fileCounter] = p.string();
				fileCounter++;
			}
		}

	}
	return fileCounter;
}

InputHandler::InputHandler() {
	memset(paths, NULL, MAX_INPUT_FILES);
}

InputHandler::~InputHandler() {
}

int InputHandler::CheckIfDirExists(const fs::path& p)
{
	if (fs::exists(p))
		return 0;
	else
		return 1;
}

int InputHandler::CheckArguments(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Did you remember to enter two arguments when launching the program?\n");
		return 1;
	}

	else if (argc > 3) {
		printf("There was more than two argument entered... THAT IS UNACCEPTABLE!\n");
		return 1;
	}

	else if (CheckIfDirExists(argv[1])) {
		printf("The input directory entered doesn't exist!\n");
		printf("The program checked the path: %s\n", argv[1]);
		return 1;
	}

	else if (CheckIfDirExists(argv[2])) {
		printf("The output directory entered doesn't exist!\n");
		printf("The program checked the path: %s\n", argv[2]);
		return 1;
	}
	else {
		printf("The entered arguments are accepted. You may procede!\n");
		return 0;
	}
}



