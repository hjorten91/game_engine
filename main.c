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

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

GLuint program;

// camera vectors
struct camera cam_obj;
struct camera * cam = &cam_obj;

// vertex array object
unsigned int vertexArrayColor;

GLfloat ground[] =
{
	-100.0f, 0.0f, -100.0f,
	100.0f, 0.0f, -100.0f,
	-100.0f, 0.0f, 100.0f,

	-100.0f, 0.0f, 100.0f,
	100.0f, 0.0f, -100.0f,
	100.0f, 0.0f, 100.0f
};

Model *m;

unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;

unsigned int groundVertexArrayObjID;

void mouse(int x, int y){
	int window_origox = round(glutGet(GLUT_WINDOW_WIDTH)/2);
	int window_origoy = round(glutGet(GLUT_WINDOW_HEIGHT)/2);
	int deltax = x - window_origox;
	int deltay = (y - window_origoy);
	fps_mouse(deltax,deltay,cam);
 	glutWarpPointer(window_origox, window_origoy);

	printf("origo: (%f, %f, %f) \n", cam->lookatpoint.x, cam->lookatpoint.y, cam->lookatpoint.z);
}

// void init_ground(void)
// {
//
// }

void init_camera(void)
{
	// init camera
	cam->pos = SetVector(0,1,-1); // x,y,z
	cam->lookatpoint = SetVector(0,0,0);
	cam->upvector = SetVector(0,2,0);
	// printf("%f,%f,%f\n", cam->pos.x,cam->pos.y,cam->pos.z);
}

void init(void)
{
	// glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	// glDisable(GL_CULL_FACE);

	// Reference to shader program

	dumpInfo();

	m = LoadModel("objects/bunnyplus.obj");
	GLuint myTex;

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("main.vert", "main.frag");
	printError("init shader");

	glGenVertexArrays(1, &bunnyVertexArrayObjID);
  glGenBuffers(1, &bunnyVertexBufferObjID);
  glGenBuffers(1, &bunnyIndexBufferObjID);
  glGenBuffers(1, &bunnyNormalBufferObjID);
	glGenBuffers(1, &bunnyTexCoordBufferObjID);
	glBindVertexArray(bunnyVertexArrayObjID);

	//textures
	LoadTGATextureSimple("images/maskros512.tga", &myTex);
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

	// VBO for vertex data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

  // VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

	if (m->texCoordArray != NULL)
  {
      glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
      glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
      glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
  }

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

	// upload ground polygon to GPU
	glGenVertexArrays(1, &groundVertexArrayObjID);
	glGenBuffers(1, &groundVertexArrayObjID);
	glBindVertexArray(groundVertexArrayObjID);

	glBindBuffer(GL_ARRAY_BUFFER, groundVertexArrayObjID);
	glBufferData(GL_ARRAY_BUFFER, 6*sizeof(GLfloat), ground, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Ground"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Ground"));

	// End of upload of geometry
	init_camera();

	glutHideCursor();

	// init controls
	glutPassiveMotionFunc(mouse);

	printError("init arrays");
}

void display(void)
{
	printError("pre display");


	// clear the screen
	// glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 rot, trans, modelMatrix, totalMatrix, projectionMatrix, cameraMatrix;
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)/1000;

	// model matrix
	trans = T(0,0,3);
	rot = Mult(Mult(Ry(t), Rx(t)), Rz(t));
	modelMatrix = Mult(trans, rot);

	// cameraMatrix = Rotation * Translation
	cameraMatrix = lookAtv(cam->pos, cam->lookatpoint, cam->upvector);

	// projection matrix
	projectionMatrix = frustum(left, right, bottom, top, near, far);

	totalMatrix = Mult(Mult(projectionMatrix,cameraMatrix), modelMatrix);

	// glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "totalMatrix"), 1, GL_TRUE, totalMatrix.m);

	// glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
  glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

	glBindVertexArray(groundVertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object

	printError("display");

	glutSwapBuffers();
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
		handle_keys(cam);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
