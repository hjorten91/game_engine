#ifndef CONTROLS
#define CONTROLS

#ifdef __APPLE__
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/gl3.h>
#else
	#if defined(_WIN32)
		#include "glew.h"
	#endif
	#include <GL/gl.h>
#endif
#include <math.h>
#include <stdio.h>
#include "common/VectorUtils3.h"



struct camera
{
	vec3 pos; // camera position
	vec3 lookatpoint; // point that camera looks at
	vec3 upvector; // "orthoganal" vector point up from camera
};

void test();
void fps_mouse(int deltax, int deltay, struct camera *cam);
void handle_keys(struct camera *cam);



#endif
