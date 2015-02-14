/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** NimRay.cpp
** The main class of the NimRay application
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#include <iostream>

#include "Window.h"
#include "Renderer.h"

#include <ctime>
#include <iostream>

int main() {
	Window window("NimRay 0.1.0", 512, 512);
	Renderer renderer;
	
	time_t old_time;
	time_t new_time;

	time(&old_time);
	
	int ticks = 0;
	while (!window.isClosed()) {
		renderer.update();
		window.update();
		ticks++;

		time(&new_time);
		int seconds = difftime(new_time, old_time);
		if (seconds >= 1) {
			std::cout << ticks << std::endl;
			ticks = 0;
			time(&old_time);
		}
	}

	return 0;
}
