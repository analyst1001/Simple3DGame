#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "md2model.h"

using namespace std;

//GLfloat angle = 0.0f;


class Pole {
	public:
		GLUquadricObj *quadratic;
		float height, radius;

		Pole() {
			height = 30;
			radius = 5;
			quadratic = gluNewQuadric();
		}

		void draw() {
			glPushMatrix();
			glPushMatrix();
			glTranslatef(0, 5, 0);
			glRotatef(90, 1, 0, 0);
			gluQuadricNormals(quadratic, GLU_SMOOTH);
			glColor3f(1.0, 0.0, 0.0);
			gluCylinder(quadratic, radius, radius, height, 32, 32);
			glPopMatrix();
			glPushMatrix();
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(0, radius, 0);
			gluSphere(quadratic, radius * 1.5, 32, 32);
			glPopMatrix();
			glPopMatrix();
		}

};
/*
Pole *pole;
void display(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat lightPos[] = {-0.2f, 0.3f, -1, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);


	glTranslatef(0.0, 0.0, -40.0);
	glRotatef(angle, 0, 1, 0);
	pole->draw();
//	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0,1.0, 0.0);
	glutSwapBuffers();
	angle += 0.5;
	if (angle > 360) {
		angle -= 360;
	}
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / (float)h, 1.0, 100.0);
}

void initRendering(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	pole = new Pole();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Test");
	initRendering();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}*/
