/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Matrix4f.cpp
** Implements a 4x4 matrix consisting of 16 float values and its helper functions
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#include "Matrix4f.h"

#include "Math.h"
#include <math.h>
#include <sstream>

/* Core */
Matrix4f::Matrix4f() {
	setIdentity();
}

void Matrix4f::setIdentity() {
	for (int i = 0; i < 16; i++) {
		if(i % 5 == 0) {
			a[i] = 1;
		} else {
			a[i] = 0;
		}
	}
}

void Matrix4f::translate(const Vector3f& v) {
	a[12] += a[0] * v.x + a[4] * v.y + a[8] * v.z;
	a[13] += a[1] * v.x + a[5] * v.y + a[9] * v.z;
	a[14] += a[2] * v.x + a[6] * v.y + a[10] * v.z;
	a[15] += a[3] * v.x + a[7] * v.y + a[11] * v.z;
}

void Matrix4f::rotate(float angle, float x, float y, float z) {
	float c = cos(Math::toRadians(angle));
	float s = sin(Math::toRadians(angle));
	float ic = 1 - c;

	float f0 = a[0] * ((x * x * ic) + c) + a[4] * ((x * y * ic) + (z * s)) + a[8] * ((x * z * ic) - (y * s));
	float f1 = a[1] * ((x * x * ic) + c) + a[5] * ((x * y * ic) + (z * s)) + a[9] * ((x * z * ic) - (y * s));
	float f2 = a[2] * ((x * x * ic) + c) + a[6] * ((x * y * ic) + (z * s)) + a[10] * ((x * z * ic) - (y * s));
	float f3 = a[3] * ((x * x * ic) + c) + a[7] * ((x * y * ic) + (z * s)) + a[11] * ((x * z * ic) - (y * s));

	float f4 = a[0] * ((x * y * ic) - (z * s)) + a[4] * ((y * y * ic) + c) + a[8] * ((y * z * ic) + (x * s));
	float f5 = a[1] * ((x * y * ic) - (z * s)) + a[5] * ((y * y * ic) + c) + a[9] * ((y * z * ic) + (x * s));
	float f6 = a[2] * ((x * y * ic) - (z * s)) + a[6] * ((y * y * ic) + c) + a[10] * ((y * z * ic) + (x * s));
	float f7 = a[3] * ((x * y * ic) - (z * s)) + a[7] * ((y * y * ic) + c) + a[11] * ((y * z * ic) + (x * s));

	float f8 = a[0] * ((x * z * ic) + (y * s)) + a[4] * ((y * z * ic) - (x * s)) + a[8] * ((z * z * ic) + c);
	float f9 = a[1] * ((x * z * ic) + (y * s)) + a[5] * ((y * z * ic) - (x * s)) + a[9] * ((z * z * ic) + c);
	float f10 = a[2] * ((x * z * ic) + (y * s)) + a[6] * ((y * z * ic) - (x * s)) + a[10] * ((z * z * ic) + c);
	float f11 = a[3] * ((x * z * ic) + (y * s)) + a[7] * ((y * z * ic) - (x * s)) + a[11] * ((z * z * ic) + c);

	a[0] = f0; a[1] = f1; a[2] = f2; a[3] = f3;
	a[4] = f4; a[5] = f5; a[6] = f6; a[7] = f7;
	a[8] = f8; a[9] = f9; a[10] = f10; a[11] = f11;
}

void Matrix4f::rotate(const Vector3f& euler) {
	rotate(euler.x, 1, 0, 0);
	rotate(euler.y, 0, 1, 0);
	rotate(euler.z, 0, 0, 1);
}

void Matrix4f::scale(const Vector3f& scale) {
	a[0] *= scale.x;
	a[1] *= scale.x;
	a[2] *= scale.x;
	a[3] *= scale.x;
	a[4] *= scale.y;
	a[5] *= scale.y;
	a[6] *= scale.y;
	a[7] *= scale.y;
	a[8] *= scale.z;
	a[9] *= scale.z;
	a[10] *= scale.z;
	a[11] *= scale.z;
}

float Matrix4f::determinant() {
	float f =
		a[0] * (a[5] * a[10] - a[6] * a[9])
		+ a[1] * (a[6] * a[8] - a[4] * a[10])
		+ a[2] * (a[4] * a[9] - a[5] * a[8]);
	return f;
}

//void Matrix4f::inverse() {
//	float det = determinant();
//
//	if (det != 0) {
//		/* do it the ordinary way
//		*
//		* inv(A) = 1/det(A) * adj(T), where adj(T) = transpose(Conjugate Matrix)
//		*
//		* m00 m01 m02
//		* m10 m11 m12
//		* m20 m21 m22
//		*/
//		float det_inv = 1.0f / det;
//
//		// get the conjugate matrix
//		float t00 = a[5] * a[10] - a[6] * a[9];
//		float t01 = -a[4] * a[10] + a[6] * a[8];
//		float t02 = a[4] * a[9] - a[5] * a[8];
//		float t10 = -a[1] * a[10] + a[2] * a[9];
//		float t11 = a[0] * a[10] - a[2] * a[8];
//		float t12 = -a[0] * a[9] + a[1] * a[8];
//		float t20 = a[1] * a[6] - a[2] * a[5];
//		float t21 = -a[0] * a[6] + a[2] * a[4];
//		float t22 = a[0] * a[5] - a[1] * a[4];
//
//		a[0] = t00 * det_inv;
//		a[5] = t11 * det_inv;
//		a[10] = t22 * det_inv;
//		a[1] = t10 * det_inv;
//		a[4] = t01 * det_inv;
//		a[8] = t02 * det_inv;
//		a[2] = t20 * det_inv;
//		a[6] = t21 * det_inv;
//		a[9] = t12 * det_inv;
//	}
//}

void Matrix4f::inverse() {
	double inv[16], det;
	int i;

	inv[0] = a[5] * a[10] * a[15] -
		a[5] * a[11] * a[14] -
		a[9] * a[6] * a[15] +
		a[9] * a[7] * a[14] +
		a[13] * a[6] * a[11] -
		a[13] * a[7] * a[10];

	inv[4] = -a[4] * a[10] * a[15] +
		a[4] * a[11] * a[14] +
		a[8] * a[6] * a[15] -
		a[8] * a[7] * a[14] -
		a[12] * a[6] * a[11] +
		a[12] * a[7] * a[10];

	inv[8] = a[4] * a[9] * a[15] -
		a[4] * a[11] * a[13] -
		a[8] * a[5] * a[15] +
		a[8] * a[7] * a[13] +
		a[12] * a[5] * a[11] -
		a[12] * a[7] * a[9];

	inv[12] = -a[4] * a[9] * a[14] +
		a[4] * a[10] * a[13] +
		a[8] * a[5] * a[14] -
		a[8] * a[6] * a[13] -
		a[12] * a[5] * a[10] +
		a[12] * a[6] * a[9];

	inv[1] = -a[1] * a[10] * a[15] +
		a[1] * a[11] * a[14] +
		a[9] * a[2] * a[15] -
		a[9] * a[3] * a[14] -
		a[13] * a[2] * a[11] +
		a[13] * a[3] * a[10];

	inv[5] = a[0] * a[10] * a[15] -
		a[0] * a[11] * a[14] -
		a[8] * a[2] * a[15] +
		a[8] * a[3] * a[14] +
		a[12] * a[2] * a[11] -
		a[12] * a[3] * a[10];

	inv[9] = -a[0] * a[9] * a[15] +
		a[0] * a[11] * a[13] +
		a[8] * a[1] * a[15] -
		a[8] * a[3] * a[13] -
		a[12] * a[1] * a[11] +
		a[12] * a[3] * a[9];

	inv[13] = a[0] * a[9] * a[14] -
		a[0] * a[10] * a[13] -
		a[8] * a[1] * a[14] +
		a[8] * a[2] * a[13] +
		a[12] * a[1] * a[10] -
		a[12] * a[2] * a[9];

	inv[2] = a[1] * a[6] * a[15] -
		a[1] * a[7] * a[14] -
		a[5] * a[2] * a[15] +
		a[5] * a[3] * a[14] +
		a[13] * a[2] * a[7] -
		a[13] * a[3] * a[6];

	inv[6] = -a[0] * a[6] * a[15] +
		a[0] * a[7] * a[14] +
		a[4] * a[2] * a[15] -
		a[4] * a[3] * a[14] -
		a[12] * a[2] * a[7] +
		a[12] * a[3] * a[6];

	inv[10] = a[0] * a[5] * a[15] -
		a[0] * a[7] * a[13] -
		a[4] * a[1] * a[15] +
		a[4] * a[3] * a[13] +
		a[12] * a[1] * a[7] -
		a[12] * a[3] * a[5];

	inv[14] = -a[0] * a[5] * a[14] +
		a[0] * a[6] * a[13] +
		a[4] * a[1] * a[14] -
		a[4] * a[2] * a[13] -
		a[12] * a[1] * a[6] +
		a[12] * a[2] * a[5];

	inv[3] = -a[1] * a[6] * a[11] +
		a[1] * a[7] * a[10] +
		a[5] * a[2] * a[11] -
		a[5] * a[3] * a[10] -
		a[9] * a[2] * a[7] +
		a[9] * a[3] * a[6];

	inv[7] = a[0] * a[6] * a[11] -
		a[0] * a[7] * a[10] -
		a[4] * a[2] * a[11] +
		a[4] * a[3] * a[10] +
		a[8] * a[2] * a[7] -
		a[8] * a[3] * a[6];

	inv[11] = -a[0] * a[5] * a[11] +
		a[0] * a[7] * a[9] +
		a[4] * a[1] * a[11] -
		a[4] * a[3] * a[9] -
		a[8] * a[1] * a[7] +
		a[8] * a[3] * a[5];

	inv[15] = a[0] * a[5] * a[10] -
		a[0] * a[6] * a[9] -
		a[4] * a[1] * a[10] +
		a[4] * a[2] * a[9] +
		a[8] * a[1] * a[6] -
		a[8] * a[2] * a[5];

	det = a[0] * inv[0] + a[1] * inv[4] + a[2] * inv[8] + a[3] * inv[12];

	if (det == 0)
		return;

	det = 1.0 / det;

	for (i = 0; i < 16; i++) {
		a[i] = inv[i] * det;
	}
}

Vector3f Matrix4f::transform(const Vector3f& v, float w) {
	Vector3f dest;
	dest.x = a[0] * v.x + a[4] * v.y + a[8] * v.z + a[12] * w;
	dest.y = a[1] * v.x + a[5] * v.y + a[9] * v.z + a[13] * w;
	dest.z = a[2] * v.x + a[6] * v.y + a[10] * v.z + a[14] * w;
	float d = a[3] * v.x + a[7] * v.y + a[11] * v.z + a[15] * w;
	dest.scale(1 / d);
	return dest;
}

float* Matrix4f::toArray() {
	return a;
}

std::string Matrix4f::str() const {
	std::stringstream ss;
	ss << "[" << a[0] << ", " << a[4] << ", " << a[8] << ", " << a[12] << "]\n";
	ss << "[" << a[1] << ", " << a[5] << ", " << a[9] << ", " << a[13] << "]\n";
	ss << "[" << a[2] << ", " << a[6] << ", " << a[10] << ", " << a[14] << "]\n";
	ss << "[" << a[3] << ", " << a[7] << ", " << a[11] << ", " << a[15] << "]\n";

	return ss.str();
}


/* Operator overloads */
bool Matrix4f::operator==(const Matrix4f& m) const {
	for (int i = 0; i < 16; i++) {
		if (a[i] != m.a[i]) {
			return false;
		}
	}
	return true;
}

bool Matrix4f::operator!=(const Matrix4f& m) const {
	for (int i = 0; i < 16; i++) {
		if (a[i] != m.a[i]) {
			return true;
		}
	}
	return false;
}

Matrix4f Matrix4f::operator*(const Matrix4f& m) const {
	Matrix4f dest;
	dest.a[0] = a[0] * m.a[0] + a[4] * m.a[1] + a[8] * m.a[2] + a[12] * m.a[3];
	dest.a[1] = a[1] * m.a[0] + a[5] * m.a[1] + a[9] * m.a[2] + a[13] * m.a[3];
	dest.a[2] = a[2] * m.a[0] + a[6] * m.a[1] + a[10] * m.a[2] + a[14] * m.a[3];
	dest.a[3] = a[3] * m.a[0] + a[7] * m.a[1] + a[11] * m.a[2] + a[15] * m.a[3];

	dest.a[4] = a[0] * m.a[4] + a[4] * m.a[5] + a[8] * m.a[6] + a[12] * m.a[7];
	dest.a[5] = a[1] * m.a[4] + a[5] * m.a[5] + a[9] * m.a[6] + a[13] * m.a[7];
	dest.a[6] = a[2] * m.a[4] + a[6] * m.a[5] + a[10] * m.a[6] + a[14] * m.a[7];
	dest.a[7] = a[3] * m.a[4] + a[7] * m.a[5] + a[11] * m.a[6] + a[15] * m.a[7];

	dest.a[8] = a[0] * m.a[8] + a[4] * m.a[9] + a[8] * m.a[10] + a[12] * m.a[11];
	dest.a[9] = a[1] * m.a[8] + a[5] * m.a[9] + a[9] * m.a[10] + a[13] * m.a[11];
	dest.a[10] = a[2] * m.a[8] + a[6] * m.a[9] + a[10] * m.a[10] + a[14] * m.a[11];
	dest.a[11] = a[3] * m.a[8] + a[7] * m.a[9] + a[11] * m.a[10] + a[15] * m.a[11];

	dest.a[12] = a[0] * m.a[12] + a[4] * m.a[13] + a[8] * m.a[14] + a[12] * m.a[15];
	dest.a[13] = a[1] * m.a[12] + a[5] * m.a[13] + a[9] * m.a[14] + a[13] * m.a[15];
	dest.a[14] = a[2] * m.a[12] + a[6] * m.a[13] + a[10] * m.a[14] + a[14] * m.a[15];
	dest.a[15] = a[3] * m.a[12] + a[7] * m.a[13] + a[11] * m.a[14] + a[15] * m.a[15];

	return dest;
}
