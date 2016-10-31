#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "imageloader.h"

using namespace std;

//GLfloat angle = 0.0f;

/*GLuint loadTexture(Image *image) {
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}*/

class FactorySmall {
	public:
		float length, width, height;
		GLuint fTexture, bTexture, sTexture, tTexture;
		FactorySmall() {
			length = 10.0f;
			width = 20.0f;
			height = 10.0f;
		}
		
		FactorySmall(float h, float l, float w) {
			length = l;
			width = w;
			height = h;
		}
		
		void setTexture(char *front, char *back, char *sides, char *top) {
                       Image *image = loadBMP(front);
                       fTexture = loadTexture(image);
                       delete image;
                       image = loadBMP(back);
                       bTexture = loadTexture(image);
                       delete image;
                       image = loadBMP(sides);
                       sTexture = loadTexture(image);
                       delete image;
                       image = loadBMP(top);
                       tTexture = loadTexture(image);
                       delete image;
		}

		void draw() {
			glEnable(GL_TEXTURE_2D);
			
			glBindTexture(GL_TEXTURE_2D, tTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glPushMatrix();
			glRotatef(90, 0, 1, 0);
			//top plane
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(length / 2, height / 2, -width / 2);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-length / 2, height / 2, -width / 2);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, sTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//side plane 1
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(length / 2, -height / 2, width / 2);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-length / 2, -height / 2, width / 2);
			glEnd();

			//side plane2
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-length / 2, height / 2, -width / 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(length / 2, height / 2, -width / 2);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(length / 2, -height / 2, -width / 2);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-length / 2, -height / 2, -width / 2);
			glEnd();

			//bottom plane
			glBegin(GL_QUADS);
				glNormal3f(0.0, -1.0, 0.0);
				glVertex3f(-length / 2, -height / 2, width / 2);
				glVertex3f(length / 2, -height / 2, width / 2);
				glVertex3f(length / 2, -height / 2, -width / 2);
				glVertex3f(-length / 2, -height / 2, -width / 2);
			glEnd();
			
			glBindTexture(GL_TEXTURE_2D, fTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//front plane
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(-length / 2, height / 2, -width / 2);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(-length / 2, -height / 2, -width / 2);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-length / 2, -height / 2, width / 2);
			glEnd();
			
			glBindTexture(GL_TEXTURE_2D, bTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//back plane
			glBegin(GL_QUADS);
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(length / 2, height / 2, -width / 2);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(length / 2, -height / 2, -width / 2);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(length / 2, -height / 2, width / 2);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

};

class FactoryLarge {
	public:
		float length, width, height;
		GLuint fTexture, bTexture, sTexture, tTexture;
		FactoryLarge() {
			length = 10.0f;
			width = 20.0f;
			height = 10.0f;
		}
		
		FactoryLarge(float h, float l, float w) {
			length = l;
			width = w;
			height = h;
		}
		
		void setTexture(char *front, char *back, char *sides, char *top) {
                       Image *image = loadBMP(front);
                       fTexture = loadTexture(image);
                       delete image;
                       image = loadBMP(back);
                       bTexture = loadTexture(image);
                       delete image;
                       image = loadBMP(sides);
                       sTexture = loadTexture(image);
                       delete image;
                       image = loadBMP(top);
                       tTexture = loadTexture(image);
                       delete image;
		}

		void draw() {
			glEnable(GL_TEXTURE_2D);
			
			glBindTexture(GL_TEXTURE_2D, tTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glPushMatrix();
			glRotatef(90, 0, 1, 0);
			//top plane
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(length / 2, height / 2, -width / 2);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-length / 2, height / 2, -width / 2);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, sTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//side plane 1
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(length / 2, -height / 2, width / 2);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-length / 2, -height / 2, width / 2);
			glEnd();

			//side plane2
			glBegin(GL_QUADS);
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-length / 2, height / 2, -width / 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(length / 2, height / 2, -width / 2);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(length / 2, -height / 2, -width / 2);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-length / 2, -height / 2, -width / 2);
			glEnd();

			//bottom plane
			glBegin(GL_QUADS);
				glNormal3f(0.0, -1.0, 0.0);
				glVertex3f(-length / 2, -height / 2, width / 2);
				glVertex3f(length / 2, -height / 2, width / 2);
				glVertex3f(length / 2, -height / 2, -width / 2);
				glVertex3f(-length / 2, -height / 2, -width / 2);
			glEnd();
			
			glBindTexture(GL_TEXTURE_2D, fTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//front plane
			glBegin(GL_QUADS);
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(-length / 2, height / 2, -width / 2);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(-length / 2, -height / 2, -width / 2);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-length / 2, -height / 2, width / 2);
			glEnd();
			
			glBindTexture(GL_TEXTURE_2D, bTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//back plane
			glBegin(GL_QUADS);
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(length / 2, height / 2, width / 2);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(length / 2, height / 2, -width / 2);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(length / 2, -height / 2, -width / 2);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(length / 2, -height / 2, width / 2);
			glEnd();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}

};
/*
FactorySmall *factorySmall;
FactoryLarge *factoryLarge;
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
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(angle, 0, 1, 0);
//	glScalef(0.1, 0.1, 0.1);
	factoryLarge->draw();
	glutSwapBuffers();
	angle += 0.02;
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
	factoryLarge = new FactoryLarge(1, 1, 4);
	factoryLarge->setTexture("flfront.bmp", "flback.bmp", "flside.bmp", "fltop.bmp");
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
