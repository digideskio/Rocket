#pragma once
#include <GL/freeglut.h>

class Spaceship
{
public:
	Spaceship();
	void position();
	void draw();
private:
	void drawEngine();
	void drawConnector();
};