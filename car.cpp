#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "md2model.h"
#include "carparts.cpp"

using namespace std;

//GLfloat angle = 0.0f;

static int carCount = 0;

class Car {
	public:
		MD2Model *carModel;
		int count;
//		RoofRail *roofRail;
//		RailBox *railBox;
//		SearchLight *searchLight;
//		SteeringWheel *steeringWheel;
//		GLfloat wheelAngle, steeringWheelAngleOffset;
		GLfloat carX, carY, carZ;
		GLfloat carAngle, carVelocity, carAcceleration, carAngularAcceleration, carAngularVelocity;
		RoofRail *roofRail;
		RailBox *railBox;
		SearchLight *searchLight;
		Car(char *filename) {
			carModel = MD2Model::load(filename);
			if (!carModel) {
				cout<<"Error loading car!!!"<<endl;
				exit(0);
			}
			carAngle = 0.0;
			carVelocity = 0.0;
			carAcceleration = 0.0;
			carAngularVelocity = 2;
			carAngularAcceleration = 4;
			carX = carY = carZ = 0.0;
			roofRail = new RoofRail(0.25, 3.4, 0.25);
			railBox = new RailBox(0.30, 1, 0.25);
			searchLight = new SearchLight();
			railBox->zMax = 1.3;
			railBox->zMin = -1.3;
//			roofRail = new RoofRail(0.2, 0.2, 1);
//			railBox = new RailBox(0.5, 0.4, 1.0 / 5);
//			searchLight = new SearchLight();
//			steeringWheel = new SteeringWheel();
//			railBox->zMin = -0.5;
//			railBox->zMax = 0.5;
//			railBox->zOffset = 0.0;
			count = carCount++;
		}

		void draw() {
			glPushMatrix();
			glTranslatef(carX, carY, carZ);
			glPushMatrix();
			glTranslatef(0.0, 3.4, 0.0);
			glRotatef(carAngle, 0.0, 1.0, 0.0);
			roofRail->draw();
			glPushMatrix();
			glTranslatef(railBox->zOffset, 0, 0);
			railBox->draw();
			glTranslatef(0.0, 0.0125, 0.0);
			glScalef(2,2, 2);
			searchLight->draw();
			glPopMatrix();
			glPopMatrix();
			glScalef(0.04, 0.04, 0.04);
			glRotatef(-90, 1.0, 0.0, 0.0);
			glRotatef(180, 0.0, 0.0, 1.0);
			glPushMatrix();
			glRotatef(carAngle, 0.0, 0.0, 1.0);
			//cout<<"Car angle: "<<carAngle<<endl;
/*			glColor3f(1.0,0.0, 0.0);
			glBegin(GL_QUADS);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(100.0, 0.0, .0);
			glVertex3f(100.0, 100.0, 0.0);
			glVertex3f(0.0, 100.0, 0.0);
			glEnd();*/
			glEnable(GL_LIGHT1 + count);
			GLfloat color[] = {1, 1, 0, 1.0};
			GLfloat position[] = {0, -150, 25.0, 1.0};
			GLfloat direction[] = {0, -200, 0, 1.0};
			GLfloat cutoffAngle[] = {10};
			glLightfv(GL_LIGHT1 + count, GL_DIFFUSE, color);
			glLightfv(GL_LIGHT1 + count, GL_SPOT_CUTOFF, cutoffAngle);
			glLightfv(GL_LIGHT1 + count, GL_POSITION, position);
//	GLfloat attenuationFactor[] = {0.2};
//	glLightfv(GL_LIGHT1 + count, GL_LINEAR_ATTENUATION, attenuationFactor);
			glLightfv(GL_LIGHT1 + count, GL_SPOT_DIRECTION, direction);
			carModel->draw();
			glPopMatrix();
			glPopMatrix();
		}

		void moveForwardLeft(float timeElapsed, float distance) {
			if (carAngularVelocity < 2) {
				carAngularVelocity += carAngularAcceleration * timeElapsed;
			}
			carAngle += carAngularVelocity * timeElapsed;
		}

		void moveForwardRight(float timeElapsed, float distance) {
			if (carAngularVelocity < 2) {
				carAngularVelocity += carAngularAcceleration * timeElapsed;
			}
			carAngle -= carAngularVelocity * timeElapsed;
		}

		void moveStraight(float timeElapsed, float distance) {
			if (carAngularVelocity > 0) {
				carAngularVelocity = carAngularAcceleration * timeElapsed * 2;
			}
		}

		void moveBackLeft(float timeElapsed, float distance) {
			if (carAngularVelocity < 2) {
				carAngularVelocity += carAngularAcceleration * timeElapsed;
			}
			carAngle -= carAngularVelocity * timeElapsed;
		}

		void moveBackRight(float timeElapsed, float distance) {
			if (carAngularVelocity < 2) {
				carAngularVelocity += carAngularAcceleration * timeElapsed;
			}
			carAngle += carAngularVelocity * timeElapsed;
		}

		void accelerate() {
			carAcceleration = 0.5;
		}

		void deccelerate() {
			carAcceleration = -0.5;
		}

		void normalAccelerate() {
			carAcceleration = -0.2 * carVelocity;
			if (carVelocity < 0.001) {
				carAcceleration = carVelocity = 0.0;
			}   
		}

		void changeVelocity(float timeElapsed) {
			if (((carVelocity + carAcceleration) <= 20) && ((carVelocity + carAcceleration) > -20)) {
				carVelocity += carAcceleration * timeElapsed;
			}

		}
};
/*
Car *car;
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
	car->draw();
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
	car = new Car("car.md2");
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
