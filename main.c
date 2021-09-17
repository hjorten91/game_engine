// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include <GLES3/gl3.h>
#include "common/Linux/MicroGlut.h"
#include "common/GL_utilities.h"
#include "common/VectorUtils3.h"
#include "common/LittleOBJLoader.h"
#include "common/LoadTGA.h"

// struct camera is defined here TODO: maybe create a camera.c/h
#include "controls.h"


// Globals
// Data would normally be read from files

// frustum
#define near 1.0
#define far 2000.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

// shaders
GLuint program, ground_program, skybox_program, windmill_program;

// textures
GLuint bunnyTex, groundTex, skyboxTex;

// Models
Model *bunny, *ground, *skybox;
Model *blade, *windmill_balcony, *windmill_roof, *windmill_walls;

// camera vectors
struct camera cam_obj;
struct camera * cam = &cam_obj;

// light
GLint isDirectional[] = {0,0,1,1};
vec3 lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
                                 {0.0f, 1.0f, 0.0f}, // Green light
                                 {0.0f, 0.0f, 1.0f}, // Blue light
                                 {1.0f, 1.0f, 1.0f} }; // White light

vec3 lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                                        	 {0.0f, 5.0f, 10.0f}, // Green light, positional
																           {-1.0f, 0.0f, 0.0f}, // Blue light along X
																           {0.0f, 0.0f, -1.0f} }; // White light along Z
GLfloat specularExponent[] = {100.0, 200.0, 60.0, 50.0, 300.0, 150.0};


// handles mouse and camera interaction
void mouse(int x, int y){
	int window_origox = round(glutGet(GLUT_WINDOW_WIDTH)/2);
	int window_origoy = round(glutGet(GLUT_WINDOW_HEIGHT)/2);
	int deltax = x - window_origox;
	int deltay = (y - window_origoy);
	fps_mouse(deltax,deltay,cam);
 	glutWarpPointer(window_origox, window_origoy);

	// printf("origo: (%f, %f, %f) \n", cam->lookatpoint.x, cam->lookatpoint.y, cam->lookatpoint.z);
}

void init_camera(void)
{
	cam->pos = SetVector(1,1,-1); // x,y,z
	cam->lookatpoint = SetVector(0,0,0);
	cam->upvector = SetVector(0,2,0);
}

void upload_models(void)
{
	bunny = LoadModel("objects/bunnyplus.obj");
	ground = LoadModel("objects/skybox.obj");
	skybox = LoadModel("objects/skybox.obj");

	windmill_balcony = LoadModel("models/windmill/windmill-balcony.obj");
	windmill_roof = LoadModel("models/windmill/windmill-roof.obj");
	windmill_walls = LoadModel("models/windmill/windmill-walls.obj");
	blade = LoadModel("models/windmill/blade.obj");
}

void upload_textures(void)
{
	// upload textures
	LoadTGATextureSimple("images/conc.tga", &bunnyTex);
	LoadTGATextureSimple("images/grass.tga", &groundTex);
	LoadTGATextureSimple("images/skybox512.tga", &skyboxTex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bunnyTex);
	glUniform1i(glGetUniformLocation(program, "texUnit0"), 0); // Texture unit 0
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	glUniform1i(glGetUniformLocation(ground_program, "texUnit1"), 1); // Texture unit 1
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, skyboxTex);
	glUniform1i(glGetUniformLocation(skybox_program, "texUnit2"), 2); // Texture unit 2
}

void upload_light(void)
{
	glUniform3fv(glGetUniformLocation(windmill_program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(windmill_program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	//glUniform1f(glGetUniformLocation(windmill_program, "specularExponent"), specularExponent[i]);
	glUniform1iv(glGetUniformLocation(windmill_program, "isDirectional"), 4, isDirectional);
}

void init(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("shaders/main.vert", "shaders/main.frag");
	ground_program = loadShaders("shaders/ground.vert", "shaders/ground.frag");
	skybox_program = loadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	windmill_program = loadShaders("shaders/windmill.vert", "shaders/windmill.frag");

	upload_models();
	upload_textures();

	// End of upload of geometry
	init_camera();

	// hide mouse cursor and init mouse inputs
	glutHideCursor();
	glutPassiveMotionFunc(mouse);

	printError("init() ");
}

void drawBunny(void)
{
	glUseProgram(program);
	mat4 rot, trans, modelMatrix, totalMatrix, projectionMatrix, cameraMatrix;
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;

	// model to world matrix
	trans = T(0.0f,1.0f,3.0f);
	rot = Mult(Mult(Rx(t), Ry(t)), Rz(t));

	// rotation * transformation * scale
	modelMatrix = Mult(trans, rot);

	// cameraMatrix = Rotation * Translation
	cameraMatrix = lookAtv(cam->pos, cam->lookatpoint, cam->upvector);

	// projection matrix
	projectionMatrix = frustum(left, right, bottom, top, near, far);

	// totalMatrix = projectionMatrix * cameraMatrix * modelMatrix
	totalMatrix = Mult(Mult(projectionMatrix, cameraMatrix), modelMatrix);
	glUniform1i(glGetUniformLocation(program,"texUnit0"),0);
	glUniformMatrix4fv(glGetUniformLocation(program, "totalMatrix"), 1, GL_TRUE, totalMatrix.m);
	DrawModel(bunny, program, "in_Position", "in_Normal", "inTexCoord");
}

// TODO: probably use cameraMatrix and projectionMatrix as an input to the function
void drawGround(void)
{
	glUseProgram(ground_program);
	mat4 trans, scale, modelMatrix, totalMatrix, projectionMatrix, cameraMatrix;

	// model to world matrix
	trans = T(0.0f, -3.0f, 0.0f);
	scale = S(100.0f,0.5f,100.0f);

	// modelMatrix = rotation * transformation * scale
	modelMatrix = Mult(trans, scale);

	// cameraMatrix = Rotation * Translate * Scale
	cameraMatrix = lookAtv(cam->pos, cam->lookatpoint, cam->upvector);

	// projection matrix
	projectionMatrix = frustum(left, right, bottom, top, near, far);

	totalMatrix = Mult(Mult(projectionMatrix,cameraMatrix), modelMatrix);
	glUniform1i(glGetUniformLocation(ground_program,"texUnit1"),1);
	glUniformMatrix4fv(glGetUniformLocation(ground_program, "groundMatrix"), 1, GL_TRUE, totalMatrix.m);
	DrawModel(ground, ground_program, "in_Position", NULL, "inTexCoord");
}

void drawSky(void)
{
	glUseProgram(skybox_program);
	glDisable(GL_DEPTH_TEST);
	mat4 trans, scale, modelMatrix, totalMatrix, projectionMatrix, cameraMatrix, skyboxMatrix;

	// model to world matrix
	trans = T(0.0f, -1.0f, 0.0f);
	scale = S(1.0f,2.0f,1.0f);

	// modelMatrix = rotation * transformation * scale
	modelMatrix = Mult(trans, scale);

	// cameraMatrix = Rotation * Translate * Scale
	cameraMatrix = lookAtv(cam->pos, cam->lookatpoint, cam->upvector);
	skyboxMatrix = cameraMatrix;
	skyboxMatrix.m[3] = 0.0f;
	skyboxMatrix.m[7] = 0.0f;
	skyboxMatrix.m[11] = 0.0f;

	// projection matrix
	projectionMatrix = frustum(left, right, bottom, top, near, far);

	totalMatrix = Mult(Mult(projectionMatrix, skyboxMatrix), modelMatrix);
	glUniform1i(glGetUniformLocation(skybox_program,"texUnit2"),2);
	glUniformMatrix4fv(glGetUniformLocation(skybox_program, "skyboxMatrix"), 1, GL_TRUE, totalMatrix.m);
	DrawModel(skybox, skybox_program, "in_Position", NULL, "inTexCoord");

	glEnable(GL_DEPTH_TEST);
}

void drawWindmill(void)
{
	glUseProgram(windmill_program);
	mat4 rot, trans, scale, modelMatrix, totalMatrix, projectionMatrix, cameraMatrix;
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;

	// model to world matrix
	trans = T(10.0f,-2.3f,10.0f);
	// rot = Mult(Mult(Rx(t), Ry(t)), Rz(t));

	// rotation * transformation * scale
	modelMatrix = trans;

	// cameraMatrix = Rotation * Translation
	cameraMatrix = lookAtv(cam->pos, cam->lookatpoint, cam->upvector);

	// projection matrix
	projectionMatrix = frustum(left, right, bottom, top, near, far);

	// totalMatrix = projectionMatrix * cameraMatrix * modelMatrix
	totalMatrix = Mult(Mult(projectionMatrix, cameraMatrix), modelMatrix);
	glUniform1i(glGetUniformLocation(windmill_program,"texUnit0"),0);
	glUniformMatrix4fv(glGetUniformLocation(windmill_program, "windmillMatrix"), 1, GL_TRUE, totalMatrix.m);
	DrawModel(windmill_walls, windmill_program, "in_Position", "in_Normal", "inTexCoord");

	// roof
	trans = T(10.0f,-2.3f,10.0f);
	modelMatrix = trans;
	totalMatrix = Mult(Mult(projectionMatrix, cameraMatrix), modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(windmill_program, "windmillMatrix"), 1, GL_TRUE, totalMatrix.m);
	DrawModel(windmill_roof, windmill_program, "in_Position", "in_Normal", "inTexCoord");

	//balcony
	trans = T(10.0f,-2.3f,10.0f);
	modelMatrix = trans;
	totalMatrix = Mult(Mult(projectionMatrix, cameraMatrix), modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(windmill_program, "windmillMatrix"), 1, GL_TRUE, totalMatrix.m);
	DrawModel(windmill_balcony, windmill_program, "in_Position", "in_Normal", "inTexCoord");

	//blade 1
	rot = Rx(t);
	trans = T(14.6f,6.8f,10.0f);
	mat4 trans2 = T(0.0f,0.3f,0.3f);
	scale = S(0.7,0.7,0.7);
	modelMatrix = Mult(Mult(Mult(trans,rot),trans2),scale);
	totalMatrix = Mult(Mult(projectionMatrix, cameraMatrix), modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(windmill_program, "windmillMatrix"), 1, GL_TRUE, totalMatrix.m);
	DrawModel(blade, windmill_program, "in_Position", "in_Normal", "inTexCoord");

	//blade 2
	rot = Rx(M_PI/2+t);
	// trans = T(14.6f,5.9f,9.9f);
	scale = S(0.7,0.7,0.7);
	modelMatrix = Mult(Mult(Mult(trans,rot),trans2),scale);
	totalMatrix = Mult(Mult(projectionMatrix, cameraMatrix), modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(windmill_program, "windmillMatrix"), 1, GL_TRUE, totalMatrix.m);
	DrawModel(blade, windmill_program, "in_Position", "in_Normal", "inTexCoord");

	//blade 3
	rot = Rx(M_PI+t);
	// trans = T(14.6f,6.1f,9.8f);
	scale = S(0.7,0.7,0.7);
	modelMatrix = Mult(Mult(Mult(trans,rot),trans2),scale);
	totalMatrix = Mult(Mult(projectionMatrix, cameraMatrix), modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(windmill_program, "windmillMatrix"), 1, GL_TRUE, totalMatrix.m);
	DrawModel(blade, windmill_program, "in_Position", "in_Normal", "inTexCoord");


	//blade 4
	rot = Rx(M_PI*3/2+t);
	// trans = T(14.6f,6.4f,9.9f);
	scale = S(0.7,0.7,0.7);
	modelMatrix = Mult(Mult(Mult(trans,rot),trans2),scale);
	totalMatrix = Mult(Mult(projectionMatrix, cameraMatrix), modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(windmill_program, "windmillMatrix"), 1, GL_TRUE, totalMatrix.m);
	DrawModel(blade, windmill_program, "in_Position", "in_Normal", "inTexCoord");
}

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawSky();
	drawBunny();
	drawWindmill();
	drawGround();

	printError("display");

	glutSwapBuffers();
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
		handle_keys(cam); // check for user input
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
