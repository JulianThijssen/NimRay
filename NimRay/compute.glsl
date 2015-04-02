#version 430 core

layout(binding = 0, rgba32f) uniform image2D framebuffer;
layout (local_size_x = 8, local_size_y = 8) in;

// Camera position
uniform vec3 eye;
uniform vec3 raybl;
uniform vec3 raybr;
uniform vec3 raytl;
uniform vec3 raytr;

struct box {
	vec3 min;
	vec3 max;
};

struct Sphere {
	vec3 position;
	float radius;
};

struct hitinfo {
	vec2 lambda;
	int bi;
};

#define MAX_SCENE_BOUNDS 100.0
#define NUM_BOXES 2

const box boxes[] = {
	/* The ground */
	{vec3(-5.0, -0.1, -5.0), vec3(5.0, 0.0, 5.0)},
	/* Box */
	{vec3(-0.5, 0.0, -0.5), vec3(0.5, 1.0, 0.5)}
};

//const Sphere ball = Sphere(vec3(2, 1, -1), 1);
uniform Sphere sphere;

vec2 intersectBox(vec3 origin, vec3 dir, const box b) {
	vec3 tMin = (b.min - origin) / dir;
	vec3 tMax = (b.max - origin) / dir;
	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);
	float tNear = max(max(t1.x, t1.y), t1.z);
	float tFar = min(min(t2.x, t2.y), t2.z);
	return vec2(tNear, tFar);
}

bool intersectSphere(vec3 origin, vec3 dir, const Sphere s, out hitinfo info) {
	vec3 oc = origin - s.position;
	float a = dot(dir, dir);
	float b = 2 * dot(oc, dir);
	float c = dot(oc, oc) - (s.radius * s.radius);
	
	float D = b*b - 4*c*a;
	if (D < 0) {
		return false;
	}
	if (D == 0) {
		float t = -b/(2*a);
		info.lambda = vec2(t, MAX_SCENE_BOUNDS);
		return true;
	}
	if (D > 0) {
		float t1 = (-b - sqrt(D))/(2*a);
		float t2 = (-b + sqrt(D))/(2*a);
		if (t1 < t2) { info.lambda = vec2(t1, t2); }
		if (t2 < t1) { info.lambda = vec2(t2, t1); }
		return true;
	}
}

bool intersectBoxes(vec3 origin, vec3 dir, out hitinfo info) {
	float smallest = MAX_SCENE_BOUNDS;
	bool found = false;
	
	for (int i = 0; i < NUM_BOXES; i++) {
		vec2 lambda = intersectBox(origin, dir, boxes[i]);
		if (lambda.x > 0.0 && lambda.x < lambda.y && lambda.x < smallest) {
			info.lambda = lambda;
			info.bi = i;
			smallest = lambda.x;
			found = true;
		}
	}
	return found;
}

vec4 trace(vec3 origin, vec3 dir) {
	hitinfo i;
	vec4 gray = vec4(0, 0, 0, 1);
	float smallest = MAX_SCENE_BOUNDS;
	
	if (intersectBoxes(origin, dir, i)) {
		if (i.lambda.x < smallest) {
			gray = vec4(i.lambda.x / MAX_SCENE_BOUNDS);
			smallest = i.lambda.x;
		}
	}
	if (intersectSphere(origin, dir, sphere, i)) {
		if (i.lambda.x < smallest) {
			gray = vec4(i.lambda.x / MAX_SCENE_BOUNDS);
			smallest = i.lambda.x;
		}
	}
	
	return vec4(gray.rgb, 1.0);
}

void main(void) {
	ivec2 pix = ivec2(gl_GlobalInvocationID.xy);
	ivec2 size = imageSize(framebuffer);
	if (pix.x >= size.x || pix.y >= size.y) {
		return;
	}
	vec2 pos = vec2(pix) / vec2(size.x, size.y);
	vec3 dir = mix(mix(raybl, raytl, pos.y), mix(raybr, raytr, pos.y), pos.x);
	vec4 color = trace(eye, dir);
	imageStore(framebuffer, pix, color);
}
