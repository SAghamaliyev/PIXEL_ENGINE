#include "Engine.h"

int main() {
	Engine OurEngine;
	
	if (!OurEngine.initailize()) {
		cout << "Something went wrong";
		return -1;
	}

	OurEngine.run();

	OurEngine.terminate();
	return 1;
}