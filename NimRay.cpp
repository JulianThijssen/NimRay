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

#include "Window.h"
#include "scene/Scene.h"
#include "Renderer.h"

////
#include "scene/Sphere.h"
////

#include <ctime>
#include <iostream>

int main() {
	Window window("NimRay 0.1.0", 512, 512);
	Scene scene;
	Renderer renderer;

	scene.sphere = new Sphere(0.5);
	renderer.setScene(&scene);
	
	time_t old_time;
	time_t new_time;

	time(&old_time);
	
	int ticks = 0;
	while (!window.isClosed()) {
		renderer.update();
		window.update();
		ticks++;

		time(&new_time);
		double seconds = difftime(new_time, old_time);
		if (seconds >= 1) {
			std::cout << ticks << std::endl;
			ticks = 0;
			time(&old_time);
		}
	}

	return 0;
}
