/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Renderer.h
** Declares the renderer performing the ray tracing
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#include "Camera.h"
#include "Matrix4f.h"
#include "Vector3f.h"

class Renderer {
public:
	Renderer();
	void update();
	int getQuad();
private:
	unsigned int frameBuffer;
	unsigned int computeShader;
	unsigned int drawShader;
	unsigned int quad;
	
	Camera camera;
	Vector3f camPos;

	Matrix4f projMatrix;
	Matrix4f viewMatrix;
};
