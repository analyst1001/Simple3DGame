#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include "imageloader.h"
#include "vec3f.h"

using namespace std;

//float angle = 0.0f;
//float ex = 0.0f, ey = 5.0f, ez = 0.0f;
class Terrain {
	private:
		int w; //Width
		int l; //Length
		float** hs; //Heights
		Vec3f** normals;
		bool computedNormals; //Whether normals is up-to-date
	public:
		Terrain(int w2, int l2) {
			w = w2;
			l = l2;
			
			hs = new float*[l];
			for(int i = 0; i < l; i++) {
				hs[i] = new float[w];
			}
			
			normals = new Vec3f*[l];
			for(int i = 0; i < l; i++) {
				normals[i] = new Vec3f[w];
			}
			
			computedNormals = false;
		}
		
		~Terrain() {
			for(int i = 0; i < l; i++) {
				delete[] hs[i];
			}
			delete[] hs;
			
			for(int i = 0; i < l; i++) {
				delete[] normals[i];
			}
			delete[] normals;
		}
		
		int width() {
			return w;
		}
		
		int length() {
			return l;
		}
		
		//Sets the height at (x, z) to y
		void setHeight(int x, int z, float y) {
			hs[z][x] = y;
			computedNormals = false;
		}
		
		//Returns the height at (x, z)
		float getHeight(int x, int z) {
			return hs[z][x];
		}
		
		//Computes the normals, if they haven't been computed yet
		void computeNormals() {
			if (computedNormals) {
				return;
			}
			
			//Compute the rough version of the normals
			Vec3f** normals2 = new Vec3f*[l];
			for(int i = 0; i < l; i++) {
				normals2[i] = new Vec3f[w];
			}
			
			for(int z = 0; z < l; z++) {
				for(int x = 0; x < w; x++) {
					Vec3f sum(0.0f, 0.0f, 0.0f);
					
					Vec3f out;
					if (z > 0) {
						out = Vec3f(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
					}
					Vec3f in;
					if (z < l - 1) {
						in = Vec3f(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
					}
					Vec3f left;
					if (x > 0) {
						left = Vec3f(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
					}
					Vec3f right;
					if (x < w - 1) {
						right = Vec3f(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
					}
					
					if (x > 0 && z > 0) {
						sum += out.cross(left).normalize();
					}
					if (x > 0 && z < l - 1) {
						sum += left.cross(in).normalize();
					}
					if (x < w - 1 && z < l - 1) {
						sum += in.cross(right).normalize();
					}
					if (x < w - 1 && z > 0) {
						sum += right.cross(out).normalize();
					}
					
					normals2[z][x] = sum;
				}
			}
			
			//Smooth out the normals
			const float FALLOUT_RATIO = 0.5f;
			for(int z = 0; z < l; z++) {
				for(int x = 0; x < w; x++) {
					Vec3f sum = normals2[z][x];
					
					if (x > 0) {
						sum += normals2[z][x - 1] * FALLOUT_RATIO;
					}
					if (x < w - 1) {
						sum += normals2[z][x + 1] * FALLOUT_RATIO;
					}
					if (z > 0) {
						sum += normals2[z - 1][x] * FALLOUT_RATIO;
					}
					if (z < l - 1) {
						sum += normals2[z + 1][x] * FALLOUT_RATIO;
					}
					
					if (sum.magnitude() == 0) {
						sum = Vec3f(0.0f, 1.0f, 0.0f);
					}
					normals[z][x] = sum;
				}
			}
			
			for(int i = 0; i < l; i++) {
				delete[] normals2[i];
			}
			delete[] normals2;
			
			computedNormals = true;
		}
		
		//Returns the normal at (x, z)
		Vec3f getNormal(int x, int z) {
			if (!computedNormals) {
				computeNormals();
			}
			return normals[z][x];
		}
};

GLuint loadTexture(Image *image) {
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

class WorldTerrain {
	private:
		Terrain *_terrain;
		GLuint _textureId;

	public:
		WorldTerrain(const char *filename, float height) {
			_terrain = loadTerrain(filename, height);
		}

		void setTexture(char *filename) {
                       Image *image = loadBMP(filename);
                        _textureId = loadTexture(image);
                        delete image;
		}

		Terrain* loadTerrain(const char* filename, float height) {
			Image* image = loadBMP(filename);
			Terrain* t = new Terrain(image->width, image->height);
			for(int y = 0; y < image->height; y++) {
				for(int x = 0; x < image->width; x++) {
					unsigned char color =
						(unsigned char)image->pixels[3 * (y * image->width + x)];
					float h = height * ((color / 255.0f) - 0.5f);
					t->setHeight(x, y, h);
				}
			}

			delete image;
			t->computeNormals();
			return t;
		}

		void draw() {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureId);

			//Bottom
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			glRotatef(45, 1.0, 0.0, 0.0);
			//glRotatef(angle, 0.0, 1.0, 0.0);
			makeTerrain();
/*			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-10, 0, -10);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(10, 0, -10);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(10, 0, 10);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-10, 0, 10);
			glEnd();*/
			glDisable(GL_TEXTURE_2D);
			glEnd();

		}

		int max(int a, int b) {
			return (a > b)? a:b;
		}
	
		void makeTerrain() {
			glPushMatrix();
			float scale = 50.0f / max(_terrain->width() - 1, _terrain->length() - 1);
			glScalef(scale, scale, scale);
			glTranslatef(-(float)(_terrain->width() - 1) / 2,
					0.0f,
					-(float)(_terrain->length() - 1) / 2);
			glColor3f(1.0, 1.0, 1.0);
/*			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);*/
			for(int z = 0; z < _terrain->length() - 1; z++) {
				//Makes OpenGL draw a triangle at every three consecutive vertices
				glBegin(GL_TRIANGLE_STRIP);
				for(int x = 0; x < _terrain->width(); x++) {
					Vec3f normal = _terrain->getNormal(x, z);
					glNormal3f(normal[0], normal[1], normal[2]);

					glTexCoord2f(x/((float)_terrain->width()-1),z/((float)_terrain->length()-1));
					glVertex3f(x, (_terrain->getHeight(x, z)), z);
					normal = _terrain->getNormal(x, z + 1);
					glNormal3f(normal[0], normal[1], normal[2]);

					glTexCoord2f(x/((float)_terrain->width()-1),(z+1)/((float)_terrain->length()-1));

					glVertex3f(x,( _terrain->getHeight(x, z + 1)), z + 1);
				}
				glEnd();
			}
			/*			glBegin(GL_QUADS);
						glNormal3f(0.0, 0.0, 1.0);
						glTexCoord2f(0.0, 0.0);	
						glVertex3f(0.0, 0.0, 0.0);
						glTexCoord2f(1.0, 0.0);	
						glVertex3f(1024.0, 0.0, 0.0);
				glTexCoord2f(1.0, 1.0);	
				glVertex3f(1024.0, 1024.0, 0.0);
				glTexCoord2f(0.0,1.0);	
				glVertex3f(0.0, 1024.0, 0.0);

			glEnd();*/
//			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
		
		void cleanup() {
			delete _terrain;
		}

		int width() {
			return _terrain->width();
		}
		
		int length() {
			return _terrain->length();
		}

		float getHeight(int x, int z) {
			return _terrain->getHeight(x, z);
		}
};

class Water {
	private:
		Terrain *_terrain;
		GLuint _textureId;
		float tempChange;
		float changeHeight;

	public:
		Water(const char *filename, float height) {
			_terrain = loadTerrain(filename, height);
			changeHeight = -1.0f;
			tempChange = 0.0005f;
		}

		void setTexture(char *filename) {
                       Image *image = loadBMP(filename);
                        _textureId = loadTexture(image);
                        delete image;
		}

		Terrain* loadTerrain(const char* filename, float height) {
			Image* image = loadBMP(filename);
			Terrain* t = new Terrain(image->width, image->height);
			for(int y = 0; y < image->height; y++) {
				for(int x = 0; x < image->width; x++) {
					unsigned char color =
						(unsigned char)image->pixels[3 * (y * image->width + x)];
					float h = height * ((color / 255.0f) - 0.5f);
					t->setHeight(x, y, h);
				}
			}

			delete image;
			t->computeNormals();
			return t;
		}

		void draw() {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureId);

			//Bottom
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			glRotatef(45, 1.0, 0.0, 0.0);
			//glRotatef(angle, 0.0, 1.0, 0.0);
			makeTerrain();
/*			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-10, 0, -10);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(10, 0, -10);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(10, 0, 10);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-10, 0, 10);
			glEnd();*/
			glDisable(GL_TEXTURE_2D);
			glEnd();

		}

		int max(int a, int b) {
			return (a > b)? a:b;
		}
	
		void makeTerrain() {
			update();
			glPushMatrix();
			float scale = 5.0f / max(_terrain->width() - 1, _terrain->length() - 1);
			glScalef(scale, scale, scale);
			glTranslatef(-(float)(_terrain->width() - 1) / 2,
					0.0f,
					-(float)(_terrain->length() - 1) / 2);
			glColor3f(1.0, 1.0, 1.0);
/*			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);*/
			for(int z = 0; z < _terrain->length() - 1; z++) {
				//Makes OpenGL draw a triangle at every three consecutive vertices
				glBegin(GL_TRIANGLE_STRIP);
				for(int x = 0; x < _terrain->width(); x++) {
					Vec3f normal = _terrain->getNormal(x, z);
					glNormal3f(normal[0], normal[1], normal[2]);

					glTexCoord2f(x/((float)_terrain->width()-1),z/((float)_terrain->length()-1));
					glVertex3f(x, (_terrain->getHeight(x, z)) * changeHeight, z);
					normal = _terrain->getNormal(x, z + 1);
					glNormal3f(normal[0], normal[1], normal[2]);

					glTexCoord2f(x/((float)_terrain->width()-1),(z+1)/((float)_terrain->length()-1));

					glVertex3f(x,( _terrain->getHeight(x, z + 1)) * changeHeight, z + 1);
				}
				glEnd();
			}
			/*			glBegin(GL_QUADS);
						glNormal3f(0.0, 0.0, 1.0);
						glTexCoord2f(0.0, 0.0);	
						glVertex3f(0.0, 0.0, 0.0);
						glTexCoord2f(1.0, 0.0);	
						glVertex3f(1024.0, 0.0, 0.0);
				glTexCoord2f(1.0, 1.0);	
				glVertex3f(1024.0, 1024.0, 0.0);
				glTexCoord2f(0.0,1.0);	
				glVertex3f(0.0, 1024.0, 0.0);

			glEnd();*/
//			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
		
		void update() {
			changeHeight += tempChange;
			if(changeHeight > 1.0f || changeHeight < -1.0f)
			{
				tempChange *= -1;
			}
		}

		void cleanup() {
			delete _terrain;
		}

		int width() {
			return _terrain->width();
		}
		
		int length() {
			return _terrain->length();
		}

		float getHeight(int x, int z) {
			return _terrain->getHeight(x, z);
		}
};

/*
WorldTerrain *wt = new WorldTerrain("heightmap.bmp", 100);
Water *wt = new Water("waterheightmap.bmp", 10);
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(0);
	}
	return;
}


void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	wt->setTexture("water.bmp");
//                       Image *image = loadBMP("texture.bmp");
  //                      _textureId = loadTexture(image);
    //                    delete image;
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	gluLookAt(ex, ey, ez, ex + 10, ey , ez + 10, 0, 1, 0);
	glTranslatef(0.0, 0.0, -10.0);
	glRotatef(45, 1, 0, 0);
//	glScalef(100.0, 1.0, 100.0);
	wt->draw();
	glutSwapBuffers();
	angle += 0.5f;
	if (angle > 360) {
		angle -= 360;
	}
}

void special(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		ez -= 1.0;
	}
	else if (key == GLUT_KEY_DOWN) {
		ez += 1.0;
	}
	else if (key == GLUT_KEY_LEFT) {
		ex -= 1.0;
	}
	else if (key == GLUT_KEY_RIGHT) {
		ex += 1.0;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);

	glutCreateWindow("Textures - videotutorialsrock.com");
	initRendering();

	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(special);
	glutReshapeFunc(handleResize);

	glutMainLoop();
	return 0;
}*/
