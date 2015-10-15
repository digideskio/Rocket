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
double fovY = 40.0; //fov Y
//const unsigned char* TextX = "X Axis View";

vec3 camera;
vec3 camera2;
vec2 orientation;

GLuint win1; //first-person window
GLuint win2; //third-person window
bool wireframe_mode = true;
Spaceship ships[2]; //one Spaceship for first-person mode, one for third person mode
GLuint sphere_handle = GLuint(-1);

void drawText()
{
	//glutStrokeString() to draw text
}

void drawShips(int i)
{
	//i must be 0 or 1

	glPushMatrix();
	glTranslated(-11.5, 0.0, 11.5); //figure out real math for this
	//glScaled(10.0, 10.0, 10.0);
	//draw the grid of rockets
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ships[i].draw();
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

void drawSphere()
{
	if (sphere_handle == GLuint(-1))
	{
		sphere_handle = glGenLists(1);
		glNewList(sphere_handle, GL_COMPILE);
		glPushMatrix();
		glColor3d(0.8, 0.8, 0.8);
		glRotated(90.0, 0.0, 1.0, 0.0);
		glutWireSphere(75.0, 50, 50);
		glPopMatrix();
		glEndList();
	}
	glCallList(sphere_handle);
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
	gluPerspective(fovY, width / double(height), 1.0, 100);
	glViewport(0, 10, width, height);
	glEnable(GL_DEPTH_TEST);
	//set the models (rockets)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.x, camera.y, camera.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	drawAxes();
	//glScaled(5.0, 5.0, 5.0);
	drawShips(0);
	//glScaled(0.2, 0.2, 0.2);
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
	glOrtho(-80, 80, -80, 80, 1, 100);
	glViewport(0, 0, w2 / 2, h2);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//drawShips(1);
	glPushMatrix();
	//wireframe_mode = true;
	glTranslated(50.0, 0.0, 0.0);
	double s1 = 0.9;
	glScaled(s1, s1, s1);
	drawSphere();
	glPopMatrix();
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glScaled(-100.0, -100.0, -100.0);
	//glTranslated()
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)"X Axis View");
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	glPushMatrix();
	glScaled(3.0, 3.0, 3.0);
	drawShips(0);
	glPopMatrix();
	//glPopMatrix();
	//second camera
	//glRotated(90, 0.0, 1.0, 0.0);
	//glTranslated(w2 / 2.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	glOrtho(-80, 80, -80, 80, 1, 200);
	glViewport(w2 / 2, 0, w2 / 2, h2);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	gluLookAt(50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glRotated(90.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	double s = 72.5;
	glScaled(s, s, s);
	glTranslated(0.0, 0.0, -5.0);
	drawSphere();
	glPopMatrix();
	glutSwapBuffers();
}

//First Person
void ReshapeFunc(int w, int h)
{
	width = w;
	height = h;
	glutPostWindowRedisplay(win1);
}

//Third Person
void ReshapeFunc2(int w, int h)
{
	w2 = w;
	h2 = h;
	glutPostWindowRedisplay(win2);
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, wireframe_mode ? GL_LINE : GL_FILL);
		wireframe_mode = !wireframe_mode;
		glutPostWindowRedisplay(win1);
		//glutPostWindowRedisplay(win2);
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
		fovY < 80.0 ? fovY += 1.0 : fovY += 0;
		break;
	case GLUT_KEY_PAGE_DOWN:
		fovY > 10.0 ? fovY -= 1.0 : fovY -= 0;
		break;
	default:
		break;
	}
	glutPostWindowRedisplay(win1);
	glutPostWindowRedisplay(win2);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutSetOption(GLUT_RENDERING_CONTEXT, GLUT_USE_CURRENT_CONTEXT);
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