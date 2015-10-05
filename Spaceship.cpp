#include "Spaceship.h"
#include "GL/freeglut.h"

GLuint display_list_handle = GLuint(-1);

Spaceship::Spaceship()
{
	
}

void Spaceship::position()
{
	
}

void Spaceship::draw()
{
	if (display_list_handle == GLuint(-1))
	{
		GLUquadric* q = gluNewQuadric();
		display_list_handle = glGenLists(1);
		glNewList(display_list_handle, GL_COMPILE);
		glColor3d(1.0, 0.0, 0.0);
		glScaled(1.0, 4.0, 1.0);
		gluSphere(q, 1.0, 50, 50);
		glScaled(1.0, 0.25, 1.0);
		glPushMatrix();
		glTranslated(0.0, -1.75, 0.0);
		for (int i = 0; i <= 3; i++)
		{
			glRotated(90, 0.0, 1.0, 0.0);
			drawEngine();
			drawConnector();
			//glutSolidCone(1.0, 2.0, 50, 50);
		}
		glPopMatrix();
		/*glTranslated(-2.0, -1.75, 2.0);
		glScaled(0.5, 0.5, 0.5);
		gluSphere(q, 1.0, 50, 50);
		glTranslated(0.0, -3.0, 0.0);
		gluSphere(q, 1.0, 50, 50);*/
		glEndList();
		gluDeleteQuadric(q);
	}
	glCallList(display_list_handle);
}

void Spaceship::drawEngine(/*GLUquadric* &q*/)
{
	GLUquadric* q = gluNewQuadric();
	glPushMatrix();
	glColor3d(0.0, 1.0, 0.0);
	glTranslated(-2.0, 0.0, 0.0);
	//draw
	glScaled(0.5, 0.5, 0.5);
	gluSphere(q, 1.0, 50, 50);
	glTranslated(0.0, -3.0, 0.0);
	gluSphere(q, 1.0, 50, 50);
	glTranslated(0.0, 2.75, 0.0);
	glRotated(90, 1.0, 0.0, 0.0);
	gluCylinder(q, 1.0, 1.0, 3.0, 50, 50);
	glPopMatrix();
	gluDeleteQuadric(q);
}

void Spaceship::drawConnector()
{
	GLUquadric* q = gluNewQuadric();
	glPushMatrix();
	glColor3d(0.0, 0.0, 1.0);
	glTranslated(0.0, -1.0, 0.0);
	glScaled(1.0, 0.5, 0.1);
	gluCylinder(q, 2.0, 0.2, 2.5, 50, 50);
	//glutSolidCone(1.0, 2.5, 50, 50);
	glPopMatrix();
	gluDeleteQuadric(q);
}