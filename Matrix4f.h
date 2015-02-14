/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Matrix4f.h
** Declares a 4x4 matrix consisting of 16 float values and its helper functions
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#pragma once
#ifndef MATRIX4F_H
#define MATRIX4F_H

#include "Vector3f.h"

#include <string>

class Matrix4f {
public:
	float a[16];

	/* Core */
	Matrix4f();
	void setIdentity();
	void translate(const Vector3f& v);
	void rotate(float angle, float x, float y, float z);
	void rotate(const Vector3f& euler);
	void scale(const Vector3f& scale);
	float determinant();
	void inverse();
	Vector3f transform(const Vector3f& v, float w);

	float* toArray();
	std::string str() const;

	/* Operator overloads */
	bool operator==(const Matrix4f& m) const;
	bool operator!=(const Matrix4f& m) const;
	Matrix4f operator*(const Matrix4f& m) const;
};

#endif /* MATRIX4F_H */
