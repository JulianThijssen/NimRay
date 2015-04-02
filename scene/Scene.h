/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Scene.h
** Declares the structure of a sphere primitive
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Sphere.h"

class Scene {
public:
	/* Core */
	Scene() {}

	Sphere* sphere;
};

#endif /* SCENE_H */
