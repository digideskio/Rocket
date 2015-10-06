#include <iostream>
#include <gl/freeglut.h>
#include "Spaceship.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

int width = 512; //first window width
int height = 512; //first window height
int w2 = 1024; //second window width
int h2 = 512; // second window height
double aspectY = 40.0;

double cameraX = 0.0;
double cameraY = 0.0;
double cameraZ = 4.5;

GLuint win1; //first-person window
GLuint win2; //third-person window
bool wireframe_mode = true;
Spaceship ships[2]; //one Spaceship for first-person mode, one for third person mode

void TimerFunc(int period)
{
	glutTimerFunc(period, TimerFunc, period);
	glutPostRedisplay();
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, wireframe_mode ? GL_LINE : GL_FILL);
		wireframe_mode = !wireframe_mode;
		break;
	case 27:
	case 'x':
	case 'q':
		glutLeaveMainLoop();
		break;

	default:
		break;
	}
}

bool GLReturnedError(char* s)
{
	bool return_error = false;
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}
	return return_error;
}

//First Person
void DisplayFunc()
{
	GLReturnedError("Entering DisplayFunc");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //set background colot to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color and depth buffers
	//set the camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(aspectY, width / double(height), 1.0, 100.0);
	glViewport(0, 10, width, height);
	glEnable(GL_DEPTH_TEST);
	//set the models (rockets)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslated(0.0, 0.0, -10.0);
	glPushMatrix();
	glTranslated(-11.0, -3.0, 0.0); //this translated was apparently so that the 4x4 grid would be more centered
	//draw the grid of rockets
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//glTranslated(7.5, 0.0, 0.0);
			//Spaceship s1;
			ships[0].draw();
			//GLUquadric* q = gluNewQuadric();
			//gluSphere(q, 1.0, 10, 10);
			glTranslated(7.5, 0.0, 0.0);
		}
		if (i % 2 == 0)
		{
			glTranslated(0.0, 0.0, -7.5);
			glRotated(180.0, 0.0, 1.0, 0.0);
			glTranslated(7.5, 0.0, 0.0);
		}
		else
		{
			glRotated(180.0, 0.0, 1.0, 0.0);
			glTranslated(7.5, 0.0, -7.5);
		}
	}
	glPopMatrix();
	glutSwapBuffers();
}

//Third Person
void DisplayFunc2()
{
	GLReturnedError("Entering DisplayFunc2");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//first camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (w2 / 2.0) / double(h2), 1.0, 100.0);
	glViewport(0, 10, w2 / 2, h2);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, w2 / 4.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	GLUquadric* q = gluNewQuadric();
	glTranslated(30.0, 0.0, 5.0);
	glPushMatrix();
	wireframe_mode = true;
	gluSphere(q, 5, 50, 50);
	glPopMatrix();
	//second camera
	

	gluDeleteQuadric(q);
	glutSwapBuffers();
}

//First Person
void ReshapeFunc(int w, int h)
{
	width = w;
	height = h;
	glutPostRedisplay();
}

//Third Person
void ReshapeFunc2(int w, int h)
{
	w2 = w;
	h2 = h;
	glutPostRedisplay();
}

void SpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	/*case GLUT_KEY_UP:

	case GLUT_KEY_DOWN:

	case GLUT_KEY_LEFT:

	case GLUT_KEY_RIGHT:
		
		glutPostRedisplay();
		break;*/
	case GLUT_KEY_PAGE_UP:
		aspectY < 80.0 ? aspectY += 5.0 : aspectY += 0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		aspectY > 10.0 ? aspectY -= 5.0 : aspectY -= 0;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	win1 = glutCreateWindow("First-Person");
	glutTimerFunc(1000 / 60, TimerFunc, 1000 / 60);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutDisplayFunc(DisplayFunc);
	glutSpecialFunc(SpecialFunc);

	//glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(600, 0);
	glutInitWindowSize(w2, h2);
	win2 = glutCreateWindow("Third-Person");
	glutTimerFunc(1000 / 60, TimerFunc, 1000 / 60);
	glutReshapeFunc(ReshapeFunc2);
	glutKeyboardFunc(KeyboardFunc);
	glutDisplayFunc(DisplayFunc2);
	glutSpecialFunc(SpecialFunc);
	glutMainLoop();
}