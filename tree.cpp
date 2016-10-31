#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "md2model.h"

using namespace std;

//GLfloat angle = 0.0f;


class Banana {
	public:
		MD2Model *bananaModel;
	 	Banana(char *filename) {
			bananaModel = MD2Model::load(filename);
			if (!bananaModel) {
				cout<<"Error loading banana!!!"<<endl;
				exit(0);
			}
		}

		void draw() {
			glPushMatrix();
			glRotatef(-60, 1.0, 0.0, 0.0);
			glRotatef(180, 0.0, 0.0, 1.0);
			glScalef(0.04, 0.04, 0.04);
			bananaModel->draw();
			glPopMatrix();
		}

};

class GroundPlant {
	public:
		MD2Model *groundPlantModel;
	 	GroundPlant(char *filename) {
			groundPlantModel = MD2Model::load(filename);
			if (!groundPlantModel) {
				cout<<"Error loading ground plant!!!"<<endl;
				exit(0);
			}
		}

		void draw() {
			glPushMatrix();
			glRotatef(-60, 1.0, 0.0, 0.0);
			glRotatef(180, 0.0, 0.0, 1.0);
			glScalef(0.04, 0.04, 0.04);
			groundPlantModel->draw();
			glPopMatrix();
		}

};

class Plant {
	public:
		MD2Model *plantModel;
	 	Plant(char *filename) {
			plantModel = MD2Model::load(filename);
			if (!plantModel) {
				cout<<"Error loading plant 1 !!!"<<endl;
				exit(0);
			}
		}

		void draw() {
			glPushMatrix();
			glRotatef(-60, 1.0, 0.0, 0.0);
			glRotatef(180, 0.0, 0.0, 1.0);
			glScalef(0.04, 0.04, 0.04);
			plantModel->draw();
			glPopMatrix();
		}

};

class Fern {
	public:
		MD2Model *fernModel;
	 	Fern(char *filename) {
			fernModel = MD2Model::load(filename);
			if (!fernModel) {
				cout<<"Error loading fern!!!"<<endl;
				exit(0);
			}
		}

		void draw() {
			glPushMatrix();
			glRotatef(-60, 1.0, 0.0, 0.0);
			glRotatef(180, 0.0, 0.0, 1.0);
			glScalef(0.04, 0.04, 0.04);
			fernModel->draw();
			glPopMatrix();
		}

};

/*Banana *banana;
GroundPlant *groundPlant;
Fern *fern;
Plant *plant1, *plant2;
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



	glTranslatef(0.0, 0.0, -20.0);
	banana->draw();
//	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0,1.0, 0.0);
	glutSwapBuffers();
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
	banana = new Banana("banana.md2");
	groundPlant = new GroundPlant("plant3.md2");
	fern = new Fern("fern.md2");
	plant1 = new Plant("plant1.md2");
	plant2 = new Plant("plant2.md2");
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
