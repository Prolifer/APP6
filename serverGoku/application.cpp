//#include <iostream> //TODO: Delete if not necessary
#include <fstream>
#include "application.h"

using namespace std;

bool saveAfterReception(char message[]) {
	ofstream log;
	log.open("log.txt");
	log << message;
	log.close();
	return false;
}

bool terminationMecanism() {
	return false;
}

