#include <iostream>
#include <GL/glut.h>
#include <math.h>

using namespace std;

static int lightCount = 0;

class Box {
	private:
		float length, width, height;
	public:
		Box() {
			length = 3.0;
			width = 1.0;
			height = 1.0;
		}

		Box (float h, float l, float w) {
			length = l;
			width = w;
			height = h;
		}

		void draw() {
			//top plane
			glBegin(GL_QUADS);
				glVertex3f(-length / 2, height / 2, width / 2);
				glVertex3f(length / 2, height / 2, width / 2);
				glVertex3f(length / 2, height / 2, -width / 2);
				glVertex3f(-length / 2, height / 2, -width / 2);
			glEnd();

			//side plane 1
			glBegin(GL_QUADS);
				glVertex3f(-length / 2, height / 2, width / 2);
				glVertex3f(length / 2, height / 2, width / 2);
				glVertex3f(length / 2, -height / 2, width / 2);
				glVertex3f(-length / 2, -height / 2, width / 2);
			glEnd();

			//side plane2
			glBegin(GL_QUADS);
				glVertex3f(-length / 2, height / 2, -width / 2);
				glVertex3f(length / 2, height / 2, -width / 2);
				glVertex3f(length / 2, -height / 2, -width / 2);
				glVertex3f(-length / 2, -height / 2, -width / 2);
			glEnd();

			//bottom plane
			glBegin(GL_QUADS);
				glVertex3f(-length / 2, -height / 2, width / 2);
				glVertex3f(length / 2, -height / 2, width / 2);
				glVertex3f(length / 2, -height / 2, -width / 2);
				glVertex3f(-length / 2, -height / 2, -width / 2);
			glEnd();
//			glPushAttrib(GL_CURRENT_BIT);
			//front plane
/*			glColor4f(0.5, 0.5, 1.0, 0.0);
			glBegin(GL_QUADS);
				glVertex3f(-length / 2, height / 2, width / 2);
				glVertex3f(-length / 2, height / 2, -width / 2);
				glVertex3f(-length / 2, -height / 2, -width / 2);
				glVertex3f(-length / 2, -height / 2, width / 2);
			glEnd();*/
//			glPopAttrib();
			//back plane
			glBegin(GL_QUADS);
				glVertex3f(length / 2, height / 2, width / 2);
				glVertex3f(length / 2, height / 2, -width / 2);
				glVertex3f(length / 2, -height / 2, -width / 2);
				glVertex3f(length / 2, -height / 2, width / 2);
			glEnd();
		}
};

class SearchLight {
	private:
		float baseRadius, baseHeight, torchRadius, torchLength;
		GLUquadricObj *base, *torch;
	public:
		float angle;
		int count;
		SearchLight() {
			baseRadius = 0.05;
			baseHeight = 0.3;
			torchRadius = 0.1;
			torchLength = 0.5;
			angle = 90;
			base = gluNewQuadric();
			torch = gluNewQuadric();
			count = lightCount++;
		}

		void draw() {
			glPushMatrix();
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluCylinder(base, baseRadius, baseRadius, baseHeight, 32, 32);
			glPopMatrix();
			glPushMatrix();
			glRotatef(angle, 0.0, 1.0, 0.0);
			glTranslatef(0.0, baseHeight, 0.0);
			gluCylinder(torch, torchRadius, torchRadius, torchLength, 32, 32);
			//cout<<"Light count: "<<count<<endl;
			glEnable(GL_LIGHT3 + count);
			GLfloat pos[] = {0, 1, 0, 1.0};
			GLfloat col[] = {1, 0, 0, 1.0};
			GLfloat dir[] = {0, 0, 1, 1.0};
			GLfloat cOAngle[] = {10};
			glLightfv(GL_LIGHT3 + count, GL_DIFFUSE, col);
			glLightfv(GL_LIGHT3 + count, GL_SPOT_CUTOFF, cOAngle);
			glLightfv(GL_LIGHT3 + count, GL_POSITION, pos);
			glLightfv(GL_LIGHT3 + count, GL_SPOT_DIRECTION, dir);
			glPopMatrix();
			update();
		}

		void update() {
			angle += 1;
		}
};

class RailBox {
	private:
		float length, height, width;
		Box *box;
		float increment;
	public:
		float zMax, zMin, zOffset;
		RailBox(float h, float l, float w) {
			width = w;
			height = h;
			length = l;
			zMax = 0.0;
			zMin = 0.0;
			zOffset = 0.0;
			increment = 0.1f;
			box = new Box(height, length, width);
		}

		void draw() {
			box->draw();
			update();
		}

		void update() {
			if (zOffset < zMin) {
				increment = 0.1f;
			}
			else if (zOffset > zMax) {
				increment = -0.1f;
			}
			zOffset += increment;
		}
		
};

class RoofRail {
	private:
		float length, height, width;
		Box *box;
	public:

		RoofRail(float h, float l, float w) {
			width = w;
			height = h;
			length = l;
			box = new Box(height, length, width);
		}

		void draw() {
			box->draw();
		}
		
};

/*void display(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	carbody.draw();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0,1.0, 0.0);
	glutSwapBuffers();
	angle += 0.01f;
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
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
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
