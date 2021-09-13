#include "controls.h"
#include "common/VectorUtils3.h"
#include "common/Linux/MicroGlut.h"

void test()
{
}

/*
This function handles all user keyboard inputs i.e keyboard controls
*/
void handle_keys(struct camera * cam)
{
	// translation/speed variable
	const float speed = 0.5;

	// w,s,a,d changes value of relevant axis of the cameraposition, which in turn
	// translates the world when lookAt
	if(glutKeyIsDown('w'))
	{
		cam->pos.z += speed;
	}
	else if(glutKeyIsDown('s'))
	{
		cam->pos.z -= speed;
	}
	if(glutKeyIsDown('a'))
	{
		cam->pos.x += speed;
	}
	else if(glutKeyIsDown('d'))
	{
		cam->pos.x -= speed;
	}
}

void fps_mouse(int deltax, int deltay, struct camera * cam)
{
	vec3 f = VectorSub(cam->lookatpoint,cam->pos);

	const float rotate_value = M_PI/200;

	// printf("origo: (%f, %f, %f) \n", f.x, f.y, f.z);
	mat4 r;
	r = IdentityMatrix();

	if(deltax > 0)
	{
		// lookatpoint.x = cos(-rotate_value)*f.x + sin(-rotate_value)*f.z;
		// lookatpoint.y = f.y;
		// lookatpoint.z = -sin(-rotate_value)*f.x + cos(-rotate_value)*f.z;
		r = Mult(Ry(-rotate_value),r);
	}
	else if(deltax < 0)
	{
		// lookatpoint.x = cos(rotate_value)*f.x + sin(rotate_value)*f.z;
		// lookatpoint.y = f.y;
		// lookatpoint.z = -sin(rotate_value)*f.x + cos(rotate_value)*f.z;
		r = Mult(Ry(rotate_value),r);
	}

	if(deltay > 0)
	{
		// lookatpoint.x = f.x;
		// lookatpoint.y = cos(rotate_value)*f.y - sin(rotate_value)*f.z;
		// lookatpoint.z = sin(rotate_value)*f.y + cos(rotate_value)*f.z;
		r = Mult(Rx(rotate_value),r);
	}
	else if(deltay < 0)
	{
		// lookatpoint.x = f.x;
		// lookatpoint.y = cos(-rotate_value)*f.y - sin(-rotate_value)*f.z;
		// lookatpoint.z = sin(-rotate_value)*f.y + cos(-rotate_value)*f.z;
		r = Mult(Rx(-rotate_value),r);
	}
	// printf("%f,%f,%f,%f\n, %f,%f,%f,%f\n, %f,%f,%f,%f\n, %f,%f,%f,%f\n", r.m[0],r.m[1],r.m[2],r.m[3],r.m[4],r.m[5],r.m[6],r.m[7],r.m[8],r.m[9],r.m[10],r.m[11],r.m[12],r.m[13],r.m[14],r.m[15]);
	cam->lookatpoint = MultVec3(r, f);
}
