/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Vector3f.cpp
** Implements a vector consisting of 3 float values and its helper functions
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#include "Vector3f.h"
#include <cmath>
#include <sstream>

const Vector3f Vector3f::ZERO = Vector3f(0, 0, 0);
const Vector3f Vector3f::FORWARD = Vector3f(0, 0, -1);
const Vector3f Vector3f::UP = Vector3f(0, 1, 0);

/* Core */
Vector3f::Vector3f() : x(0), y(0), z(0) {}

Vector3f::Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

void Vector3f::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3f::set(const Vector3f& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

Vector3f* Vector3f::add(const Vector3f& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return this;
}

Vector3f* Vector3f::sub(const Vector3f& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return this;
}

Vector3f* Vector3f::scale(const float scale) {
	x *= scale;
	y *= scale;
	z *= scale;
	return this;
}

Vector3f* Vector3f::normalise() {
	float l = length();
	x /= l;
	y /= l;
	z /= l;
	return this;
}

float Vector3f::length() const {
	return sqrt(x * x + y * y + z * z);
}

std::string Vector3f::str() const {
	std::stringstream ss;
	ss << "(" << x << ", " << y << ", " << z << ")";
	return ss.str();
}

/* Operator overloads */
bool Vector3f::operator==(const Vector3f& v) const {
	return x == v.x && y == v.y && z == v.z;
}

bool Vector3f::operator!=(const Vector3f& v) const {
	return x != v.x || y != v.y || z != v.z;
}

Vector3f* Vector3f::operator+=(const Vector3f& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return this;
}

Vector3f* Vector3f::operator-=(const Vector3f& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return this;
}

Vector3f* Vector3f::operator*=(const Vector3f& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return this;
}

Vector3f* Vector3f::operator/=(const Vector3f& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return this;
}

Vector3f Vector3f::operator+(const Vector3f& v) const {
	return Vector3f(x + v.x, y + v.y, z + v.z);
}

Vector3f Vector3f::operator-(const Vector3f& v) const {
	return Vector3f(x - v.x, y - v.y, z - v.z);
}

Vector3f Vector3f::operator-() const {
	return Vector3f(-x, -y, -z);
}

Vector3f Vector3f::operator*(const float scale) const {
	return Vector3f(x * scale, y * scale, z * scale);
}

Vector3f Vector3f::operator/(const float divisor) const {
	return Vector3f(x / divisor, y / divisor, z / divisor);
}

/* Utility functions */
float dot(const Vector3f& v1, const Vector3f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3f cross(const Vector3f& v1, const Vector3f& v2) {
	Vector3f v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v2.x * v1.z - v2.z * v1.x;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

Vector3f negate(const Vector3f& v) {
	return Vector3f(-v.x, -v.y, -v.z);
}

Vector3f normalise(const Vector3f& v) {
	float l = v.length();
	return Vector3f(v.x / l, v.y / l, v.z / l);
}
