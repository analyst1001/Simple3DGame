#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "imageloader.h"
#include "vec3f.h"
#include <fstream>

using namespace std;

GLfloat angle = 0.0f;

/*GLuint loadTexture(Image *image) {
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}
*/
GLuint LoadTextureRAW( const char * filename )
{
	GLuint texture;
	int width, height;
	unsigned char * data;
	FILE * file;
	file = fopen( filename, "rb" );
	if ( file == NULL ) return 0;
	width = 1024;
	height = 512;
	data = (unsigned char *)malloc( width * height * 3 );
	fread( data, width * height * 3, 1, file );
	fclose( file );
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,	GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
	free( data );
	return texture;
} 

typedef struct {
	int X;
	int Y;
	int Z;
	double U;
	double V;
}VERTICES;
const double PI = 3.1415926535897;
const int space = 10;
const int VertexCount = (90 / space) * (360 / space) * 4;

class Earth {
	public:
		float radius;
		int slices, stacks;
		VERTICES VERTEX[VertexCount];
		GLuint textureId;
		
		Earth() {
			radius = 2.0;
			slices = 32;
			stacks = 32;
		}

		void draw() {
			glPushMatrix();
//			DisplaySphere(radius, textureId);
			glEnable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, textureId);

			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glutSolidSphere(radius,slices,stacks);
/*	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-10, -10, -5);
	glTexCoord2f(1, 0);
	glVertex3f(10, -10, -5);
	glTexCoord2f(1, 1);
	glVertex3f(10, 10, -5);
	glTexCoord2f(0, 1);
	glVertex3f(-10, 10, -5);
	glEnd();*/
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);

			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}


		void DisplaySphere (double R, GLuint texture){
			int b;
			glScalef (0.0125 * R, 0.0125 * R, 0.0125 * R);
			glRotatef (90, 1, 0, 0);
			glBindTexture (GL_TEXTURE_2D, texture);
			glBegin (GL_TRIANGLE_STRIP);
			for ( b = 0; b <= VertexCount; b++){
				glTexCoord2f (VERTEX[b].U, VERTEX[b].V);
				glVertex3f (VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);
			}
			for ( b = 0; b <= VertexCount; b++){
				glTexCoord2f (VERTEX[b].U, -VERTEX[b].V);
				glVertex3f (VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);
			}
			glEnd();
		}

		void CreateSphere (double R, double H, double K, double Z) {
			int n;
			double a;
			double b;
			n = 0;
			for( b = 0; b <= 90 - space; b+=space){
				for( a = 0; a <= 360 - space; a+=space){
					VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
					VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
					VERTEX[n].Z = R * cos((b) / 180 * PI) - Z;
					VERTEX[n].V = (2 * b) / 360;
					VERTEX[n].U = (a) / 360;
					n++;
					VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI) - H;
					VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI) + K;
					VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
					VERTEX[n].V = (2 * (b + space)) / 360;
					VERTEX[n].U = (a) / 360;
					n++;
					VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI) - H;
					VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI) + K;
					VERTEX[n].Z = R * cos((b) / 180 * PI) - Z;
					VERTEX[n].V = (2 * b) / 360;
					VERTEX[n].U = (a + space) / 360;
					n++;
					VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b + space) /180 * PI) - H;
					VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b + space) /180 * PI) + K;
					VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
					VERTEX[n].V = (2 * (b + space)) / 360;
					VERTEX[n].U = (a + space) / 360;
					n++;
				}
			}
		}

		void setTexture(char *filename) {
			Image *image = loadBMP(filename);
			textureId = loadTexture(image);
			delete image;
//			textureId = LoadTextureRAW(filename);
		}
		
};
/*
Earth *earth;
void display(void) {
	glClearDepth(1);
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



	glTranslatef(0.0, 0.0, -30.0);
	glRotatef(angle, 0, 1, 0);
	glScalef(10, 10, 10);
	earth->draw();
//	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0,1.0, 0.0);
	glutSwapBuffers();
	angle += 0.1;
	if (angle > 360) {
		angle -= 360;
	}
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / (float)h, 0.1, 100.0);
}

void initRendering(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);
//	glEnable(GL_CULL_FACE);
	earth = new Earth();
	earth->setTexture("clouds.bmp");
	//earth->setTexture("clouds");
	earth->CreateSphere(70, 0, 0, 0);
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
