/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Entity.h
** Declares the structure of an entity containing a position, rotation and scale
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "../Vector3f.h"

class Entity {
public:
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;

	Entity() :
		position(0, 0, 0),
		rotation(0, 0, 0),
		scale(1, 1, 1) {}
};

#endif /* ENTITY_H */
