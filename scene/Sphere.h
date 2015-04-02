/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Sphere.h
** Declares the structure of a sphere primitive
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Entity.h"

class Sphere : public Entity {
public:
	float radius;

	/* Core */
	Sphere(float radius) : radius(radius) {}
};

#endif /* SPHERE_H */
