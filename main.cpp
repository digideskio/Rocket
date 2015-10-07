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
double aspectY = 40.0; //fov Y

vec3 camera;
vec2 orientation;

GLuint win1; //first-person window
GLuint win2; //third-person window
bool wireframe_mode = true;
Spaceship ships[2]; //one Spaceship for first-person mode, one for third person mode

void drawShips(int i)
{
	//i must be 0 or 1

	glPushMatrix();
	glTranslated(-11.5, 0.0, 11.5);
	//draw the grid of rockets
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//glTranslated(7.5, 0.0, 0.0);
			//Spaceship s1;
			ships[i].draw();
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
}

void positionCamera()
{
	vec4 p(50, 0, 0, 1);
	mat4 m;
	m = glm::rotate(m, radians(orientation.x), vec3(0, 1.0f, 0));
	m = glm::rotate(m, radians(orientation.y), vec3(0, 0, 1.0f));
	camera = vec3(m * p);
}

void drawAxes()
{
	glPushMatrix();
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3d(0.0, 0.5, 1.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(10.0, 0.0, 0.0);

	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 10.0, 0.0);

	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 10.0);
	glEnd();
	glPopMatrix();
}

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

	positionCamera();
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
	gluLookAt(camera.x, camera.y, camera.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	drawAxes();
	drawShips(0);
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
	//gluPerspective(40, (w2 / 2.0) / double(h2), 1.0, 100.0);
	glOrtho(-80, 80, -80, 80, -80, 80);
	glViewport(0, 10, w2 / 2, h2);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, w2 / 4.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	GLUquadric* q = gluNewQuadric();
	glPushMatrix();
	glTranslated(60.0, 0.0, 0.0);
	//glPushMatrix();
	wireframe_mode = true;
	gluSphere(q, 15.0, 30, 30); //frst sphere (left) for third person view
	glPopMatrix();

	glPushMatrix();
	glTranslated(20.0, 0.0, 0.0);
	ships[1].draw();
	glPopMatrix();
	//second camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(40, w2 / double(h2), 1.0, 100.0);

	glViewport(0, 10, w2, h2);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 200.0, w2 * 0.75, 0.0, 0.0, 0.0, 1.0, 0.0);
	GLUquadric* q2 = gluNewQuadric();
	glPushMatrix();
	glTranslated(60.0, 0.0, 200.0);
	//glPushMatrix();
	wireframe_mode = true;
	gluSphere(q2, 15.0, 30, 30); //second sphere
	glPopMatrix();

	gluDeleteQuadric(q);
	gluDeleteQuadric(q2);
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
	float offset = 1.0f;
	switch (key)
	{
	case GLUT_KEY_UP:
		if(orientation.y < 89.0)
			orientation.y += offset;
		break;
	case GLUT_KEY_DOWN:
		if(orientation.y > -89.0)
			orientation.y -= offset;
		break;
	case GLUT_KEY_LEFT:
		orientation.x -= offset;
		break;
	case GLUT_KEY_RIGHT:
		orientation.x += offset;
		break;
	case GLUT_KEY_PAGE_UP:
		aspectY < 80.0 ? aspectY += 1.0 : aspectY += 0;
		break;
	case GLUT_KEY_PAGE_DOWN:
		aspectY > 10.0 ? aspectY -= 1.0 : aspectY -= 0;
		break;
	default:
		break;
	}
	glutPostRedisplay();
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
	glutInitWindowPosition(512, 0);
	glutInitWindowSize(w2, h2);
	win2 = glutCreateWindow("Third-Person");
	glutTimerFunc(1000 / 60, TimerFunc, 1000 / 60);
	glutReshapeFunc(ReshapeFunc2);
	glutKeyboardFunc(KeyboardFunc);
	glutDisplayFunc(DisplayFunc2);
	glutSpecialFunc(SpecialFunc);
	glutMainLoop();
}