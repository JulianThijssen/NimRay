/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Renderer.h
** Implements the renderer performing the ray tracing
**
** Author: Julian Thijssen
** -------------------------------------------------------------------------*/

#include "Renderer.h"

#include <GL/glew.h>

#include "ShaderLoader.h"
#include "Camera.h"
#include "Matrix4f.h"

Renderer::Renderer() {
	glClearColor(0.8f, 0.2f, 0.2f, 1);
	glGenTextures(1, &frameBuffer);
	glBindTexture(GL_TEXTURE_2D, frameBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 512, 512, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	quad = getQuad();
	computeShader = ShaderLoader::loadCShaders("compute.glsl");
	drawShader = ShaderLoader::loadShaders("vert.glsl", "frag.glsl");

	camera.setFovy(60);
	camera.setAspectRatio(1);
	camera.setZNear(0.1);
	camera.setZFar(10);
	camPos.set(3, 2, 7);
}

void Renderer::setScene(Scene* scene) {
	this->scene = scene;
}

float i = 0;
void Renderer::update() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(computeShader);
	int workGroupSize[3];
	glGetProgramiv(computeShader, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize);
	i += 0.001;
	scene->sphere->position.set(2, 1, -1);
	scene->sphere->rotation.set(0, -sin(i) * 10 - 15, 0);
	// Camera properties
	camera.loadProjectionMatrix(projMatrix);
	viewMatrix.setIdentity();
	viewMatrix.rotate(scene->sphere->rotation);
	viewMatrix.translate(-camPos);
	Matrix4f invM = (projMatrix * viewMatrix);
	invM.inverse();
	Vector3f raybl = invM.transform(Vector3f(-1, -1, 0), 1);
	raybl.sub(camPos);
	Vector3f raybr = invM.transform(Vector3f(1, -1, 0), 1);
	raybr.sub(camPos);
	Vector3f raytl = invM.transform(Vector3f(-1, 1, 0), 1);
	raytl.sub(camPos);
	Vector3f raytr = invM.transform(Vector3f(1, 1, 0), 1);
	raytr.sub(camPos);

	glUniform3f(glGetUniformLocation(computeShader, "eye"), camPos.x, camPos.y, camPos.z);
	glUniform3f(glGetUniformLocation(computeShader, "raybl"), raybl.x, raybl.y, raybl.z);
	glUniform3f(glGetUniformLocation(computeShader, "raybr"), raybr.x, raybr.y, raybr.z);
	glUniform3f(glGetUniformLocation(computeShader, "raytl"), raytl.x, raytl.y, raytl.z);
	glUniform3f(glGetUniformLocation(computeShader, "raytr"), raytr.x, raytr.y, raytr.z);
	glUniform3f(glGetUniformLocation(computeShader, "sphere.position"), scene->sphere->position.x, scene->sphere->position.y, scene->sphere->position.z);
	glUniform1f(glGetUniformLocation(computeShader, "sphere.radius"), scene->sphere->radius);
	
	// Bind level 0 of framebuffer texture to image binding point 0
	glBindImageTexture(0, frameBuffer, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
	// Dispatch the compute shader
	glDispatchCompute(512 / workGroupSize[0], 512 / workGroupSize[1], 1);
	// Unbind the image binding point
	glBindImageTexture(0, 0, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glUseProgram(0);

	glUseProgram(drawShader);

	glBindVertexArray(quad);
	glBindTexture(GL_TEXTURE_2D, frameBuffer);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(drawShader, "framebuffer"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

int Renderer::getQuad() {
	unsigned int vao, vertexVBO, textureVBO;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertexVBO);
	glGenBuffers(1, &textureVBO);

	glBindVertexArray(vao);

	float vertexBuffer[18] = {-1, -1, 0, 1, -1, 0, -1, 1, 0, -1, 1, 0, 1, -1, 0, 1, 1, 0};
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, vertexBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);

	float texCoordBuffer[12] = { 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1 };
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, texCoordBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	return vao;
}
