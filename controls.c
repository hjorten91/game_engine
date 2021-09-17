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
	// cameras vectors
	vec3 z_axis = VectorSub(cam->lookatpoint,cam->pos); // forward
	vec3 x_axis = CrossProduct(z_axis, cam->upvector);  // side

	// translation/speed variable
	const float speed = 0.5;

	// w,s,a,d changes value of relevant axis of the cameraposition
	if(glutKeyIsDown('w'))
	{
		cam->pos = VectorAdd(cam->pos,ScalarMult(Normalize(z_axis),speed));
	}
	else if(glutKeyIsDown('s'))
	{
		cam->pos = VectorAdd(cam->pos,ScalarMult(Normalize(z_axis),-speed));
	}
	if(glutKeyIsDown('a'))
	{
		cam->pos = VectorAdd(cam->pos,ScalarMult(Normalize(x_axis),-speed));
	}
	else if(glutKeyIsDown('d'))
	{
		cam->pos = VectorAdd(cam->pos,ScalarMult(Normalize(x_axis),speed));
	}
}

void fps_mouse(int deltax, int deltay, struct camera * cam)
{
	vec3 z_axis = VectorSub(cam->lookatpoint,cam->pos);
	vec3 x_axis = CrossProduct(z_axis, cam->upvector);
	mat4 translation_matrix = T(z_axis.x, z_axis.y, z_axis.z);
	const float rotate_value = M_PI/200;
	mat4 r = IdentityMatrix();

	// up/down roations
	if(deltay > 0)
	{
		// ArbRotate deals with roation issues for when angle is pi/2, pi and pi/4
		r = ArbRotate(x_axis,-rotate_value);
	}
	else if(deltay < 0)
	{
		r = ArbRotate(x_axis,rotate_value);
	}

	// left/right roations
	if(deltax > 0)
	{
		r = Mult(Ry(-rotate_value),r);
	}
	else if(deltax < 0)
	{
		r = Mult(Ry(rotate_value),r);
	}

	cam->lookatpoint = MultVec3(Mult(r, translation_matrix), Normalize(z_axis));
}
