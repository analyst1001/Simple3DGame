/*
	Abhinav Singi 201001051
	Lovepreet Singh 201001038
*/

#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include "imageloader.h"
#include "vec3f.h"
#include "terrain.cpp"
#include "car.cpp"
#include "house.cpp"
#include "factory.cpp"
#include "tree.cpp"
#include "pole.cpp"
#include "earth.cpp"

using namespace std;
bool flat = false;
float ambientRed = 0.0f, ambientGreen = 0.0f, ambientBlue = 0.0f;
bool mouseKeys[3];
GLfloat _windowHeight = 400, _windowWidth = 400;
struct timeval prevTimer;
int _cameraPosition = 0;
float ex = 0.0, ey = 10.0, ez = 0.0;
bool specialKeys[256];
bool simpleKeys[256];
WorldTerrain *wt = new WorldTerrain("heightmap.bmp", 200);
Car *car1, *car2;
House *house;
FactorySmall *factorySmall;
FactoryLarge *factoryLarge;
Banana *banana;
GroundPlant *groundPlant;
Water *water;
Pole *pole;
GLuint bananaTree, terrain, houses, industries;
bool light = false;
bool diffSpec = true;
float lightRadius = 1900.0, lightAngle = 0.0;
GLfloat _freeEyeX = 1000, _freeEyeY = 1000, _freeEyeZ = 1000, _freeCenterX = 0, _freeCenterY = 0, _freeCenterZ = 0;
GLfloat _prevMouseX = 0.0, _prevMouseY = 0.0;
GLfloat _hAngle = atan((_freeEyeZ - _freeCenterZ) / (_freeEyeX - _freeCenterX)) * 180 / 3.14, _vAngle = atan(sqrt((_freeEyeX - _freeCenterX) * (_freeEyeX - _freeCenterX) + (_freeEyeZ - _freeCenterZ) * (_freeEyeZ - _freeCenterZ)) / (_freeEyeY - _freeCenterY)) * 180 / 3.14;
GLfloat _hAngle2 = atan((_freeCenterZ - _freeEyeZ) / (_freeCenterX - _freeEyeX)) * 180 / 3.14, _vAngle2 = atan(sqrt((_freeCenterX - _freeEyeX) * (_freeCenterX - _freeEyeX) + (_freeCenterZ - _freeEyeZ) * (_freeCenterZ - _freeEyeZ)) / (_freeCenterY - _freeEyeY)) * 180 / 3.14;
Earth *earth;
WorldTerrain *mainTerrain;

float getTerrainHeight(float x, float z) {
	int newX =  (int)((x / 200.0) * (mainTerrain->max(mainTerrain->width() - 1, mainTerrain->length() - 1) / 50.0) + (float)((mainTerrain->width() - 1) / 2.0));
	int newZ =  (int)((z / 200.0) * (mainTerrain->max(mainTerrain->width() - 1, mainTerrain->length() - 1) / 50.0) + (float)((mainTerrain->length() - 1) / 2.0));
	//cout<<"new X: "<<newX<<"new Y:"<<newZ<<endl;
	//cout<<"Height: "<<mainTerrain->getHeight(newX, newZ)<<endl;
	return mainTerrain->getHeight(newX, newZ);
}

bool checkCollision(float x, float z) {
	float height = getTerrainHeight(x, z);
	if ((height < 5) && (height > -6)) {
		return false;
	}
	//cout<<"collision"<<endl;
	return true;
}

void handleMousePress(int button, int state, int x, int y) {
//	cout<<"Mouse Press"<<button<<endl;
	if (state == GLUT_DOWN) {
		_prevMouseX = x;
		_prevMouseY = y;
		mouseKeys[button] = true;
	}
	else if (state == GLUT_UP) {
		mouseKeys[button] = false;
	}
	if (button == 3) {
	GLfloat radius = sqrt((_freeCenterX - _freeEyeX) * (_freeCenterX - _freeEyeX) + (_freeCenterY - _freeEyeY) * (_freeCenterY - _freeEyeY) + (_freeCenterZ - _freeEyeZ) * (_freeCenterZ - _freeEyeZ));
//		if (radius > 0.1) {
		_freeEyeX = _freeCenterX + ((radius - 20) * cos(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
		_freeEyeZ = _freeCenterZ + ((radius - 20) * sin(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
		_freeEyeY = _freeCenterY + ((radius - 20) * cos(_vAngle * 3.14 / 180));
//		}
	}
	else if (button == 4) {
	GLfloat radius = sqrt((_freeCenterX - _freeEyeX) * (_freeCenterX - _freeEyeX) + (_freeCenterY - _freeEyeY) * (_freeCenterY - _freeEyeY) + (_freeCenterZ - _freeEyeZ) * (_freeCenterZ - _freeEyeZ));
//		if (radius < 2) {
		_freeEyeX = _freeCenterX + ((radius + 20) * cos(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
		_freeEyeZ = _freeCenterZ + ((radius + 20) * sin(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
		_freeEyeY = _freeCenterY + ((radius + 20) * cos(_vAngle * 3.14 / 180));
//		}
	}
}

void handleMouseActiveMove(int x, int y) {
	GLfloat radius = sqrt((_freeCenterX - _freeEyeX) * (_freeCenterX - _freeEyeX) + (_freeCenterY - _freeEyeY) * (_freeCenterY - _freeEyeY) + (_freeCenterZ - _freeEyeZ) * (_freeCenterZ - _freeEyeZ));
	if (mouseKeys[GLUT_LEFT_BUTTON]) {
	GLfloat currMouseX, currMouseY;
	currMouseX = x;
	currMouseY = y;
	_vAngle += atan((currMouseY - _prevMouseY) / _windowHeight) * 180 / 3.14;
	_hAngle += atan((currMouseX - _prevMouseX) / _windowWidth) * 180 / 3.14;
	_freeEyeX = _freeCenterX + (radius * cos(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
	_freeEyeZ = _freeCenterZ + (radius * sin(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
	_freeEyeY = _freeCenterY + (radius * cos(_vAngle * 3.14 / 180));
	//cout<<_hAngle<<" "<<_vAngle<<" "<<_freeCenterX<<" "<<_freeCenterY<<" "<<_freeCenterZ<<endl;
	_prevMouseX = x;
	_prevMouseY = y;
/*	GLfloat radius = sqrt((_freeCenterX - _freeEyeX) * (_freeCenterX - _freeEyeX) + (_freeCenterY - _freeEyeY) * (_freeCenterY - _freeEyeY) + (_freeCenterZ - _freeEyeZ) * (_freeCenterZ - _freeEyeZ));
	if (currMouseX != _prevMouseX) {
		_freeCenterX = _freeEyeX - (radius * sin(_hAngle * 3.14 / 180));
		_freeCenterZ = _freeEyeZ + (radius * cos(_hAngle * 3.14 / 180));
	}
//	else if (currMouseX < _prevMouseX) {
//		_freeCenterX = _freeEyeX + (radius * sin(_hAngle * 3.14 / 180));
//		_freeCenterZ = _freeEyeZ - (radius * cos(_hAngle * 3.14 / 180));
//	}
	if (currMouseY != _prevMouseY) {
		_freeCenterY = _freeEyeY - (radius * sin(_vAngle * 3.14 / 180));
	}
	_prevMouseX = x;
	_prevMouseY = y;
	cout << _freeCenterX << " " << _freeCenterY<<" "<<_freeCenterZ<<" "<<_hAngle<<" "<<_vAngle<<endl;
	cout << _freeEyeX << " |" << _freeEyeY<<"| "<<_freeEyeZ<<"|"<<radius<<endl;*/
	}
	else if (mouseKeys[GLUT_RIGHT_BUTTON]) {
/*	GLfloat currMouseX, currMouseY, vAngle, hAngle;
	currMouseX = x;
	currMouseY = y;
	vAngle = atan((currMouseY - _prevMouseY) / _windowHeight);
	hAngle = atan((currMouseX - _prevMouseX) / _windowWidth);
	GLfloat radius = sqrt((_freeCenterX - _freeEyeX) * (_freeCenterX - _freeEyeX) + (_freeCenterY - _freeEyeY) * (_freeCenterY - _freeEyeY) + (_freeCenterZ - _freeEyeZ) * (_freeCenterZ - _freeEyeZ));
	if (currMouseX > _prevMouseX) {
		_freeEyeX = _freeCenterX + (radius * sin(180 - (_hAngle * 3.14 / 180)) / 100);
		_freeEyeZ = _freeCenterZ + (radius * cos(180 - (_hAngle * 3.14 / 180)) / 100);
	}
	else if (currMouseX < _prevMouseX) {
		_freeEyeX = _freeCenterX + (radius * sin(hAngle) / 100);
		_freeEyeZ = _freeCenterZ - (radius * cos(hAngle) / 100);
	}
//	_freeEyeY = _freeCenterY - (radius * sin(vAngle));
	_prevMouseX = x;
	_prevMouseY = y;
	cout << currMouseX << " " << currMouseY<<endl;*/
	GLfloat currMouseX, currMouseY;
	currMouseX = x;
	currMouseY = y;
	_vAngle += atan((currMouseY - _prevMouseY) / _windowHeight) * 180 / 3.14;
	_hAngle += atan((currMouseX - _prevMouseX) / _windowWidth) * 180 / 3.14;
	_freeCenterX = _freeEyeX - (radius * cos(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
	_freeCenterZ = _freeEyeZ - (radius * sin(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
	_freeCenterY = _freeEyeY - (radius * cos(_vAngle * 3.14 / 180));
	//cout<<_hAngle2<<" "<<_vAngle2<<" "<<_freeCenterX<<" "<<_freeCenterY<<" "<<_freeCenterZ<<endl;
	_prevMouseX = x;
	_prevMouseY = y;
	}
}

void handleKeyRelease(unsigned char key, int x, int y) {
	simpleKeys[key] = false;
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(0);
		case 'l':
			if (!light) {
				light = true;
				glEnable(GL_LIGHT0);
			}
			else {
				light = false;
				glDisable(GL_LIGHT0);
			}
			break;
		case 'm':
			if (diffSpec) {
				diffSpec = false;
			}
			else {
				diffSpec = true;
			}
			break;
		case 'c':
			_cameraPosition = (_cameraPosition + 1) % 3;
			break;
		case 'e':
			lightAngle += 0.5f;
			break;
		case 'q':
			lightAngle -= 0.5f;
			break;
		case '+':
			if (ambientRed < 1.0) {
				ambientRed += 0.1;
				ambientGreen += 0.1;
				ambientBlue += 0.1;
			}
			break;
		case '-':
			if (ambientRed > 0.0) {
				ambientRed -= 0.1;
				ambientGreen -= 0.1;
				ambientBlue -= 0.1;
			}
			break;
		case 'f':
			if (flat) {
				flat = false;
			}
			else {
				flat = true;
			}

	}
/*	if (key == 'f') {
		cout<<"f"<<endl;
		GLfloat radius = sqrt((_freeCenterX - _freeEyeX) * (_freeCenterX - _freeEyeX) + (_freeCenterY - _freeEyeY) * (_freeCenterY - _freeEyeY) + (_freeCenterZ - _freeEyeZ) * (_freeCenterZ - _freeEyeZ));
		_freeCenterX = _freeEyeX - ((radius + 50) * cos(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
		_freeCenterZ = _freeEyeZ - ((radius + 50) * sin(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
		_freeCenterY = _freeEyeY - ((radius + 50) * cos(_vAngle * 3.14 / 180));
	}
	if (key == 'b') {
		cout<<"b"<<endl;
		GLfloat radius = sqrt((_freeCenterX - _freeEyeX) * (_freeCenterX - _freeEyeX) + (_freeCenterY - _freeEyeY) * (_freeCenterY - _freeEyeY) + (_freeCenterZ - _freeEyeZ) * (_freeCenterZ - _freeEyeZ));
		_freeCenterX = _freeEyeX - ((radius - 50) * cos(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
		_freeCenterZ = _freeEyeZ - ((radius - 50) * sin(_hAngle * 3.14 / 180) * sin(_vAngle * 3.14 / 180));
		_freeCenterY = _freeEyeY - ((radius - 50) * cos(_vAngle * 3.14 / 180));
	}*/
	simpleKeys[key] = true;
	return;
}


void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	wt->setTexture("terrainTexture.bmp");
	car1 = new Car("car.md2");
	car2 = new Car("car.md2");
	house = new House();
	house->setTexture("hfront.bmp", "hback.bmp", "hside.bmp", "htop.bmp");
	factorySmall = new FactorySmall(); 
	factorySmall->setTexture("ffront.bmp", "fback.bmp", "fside.bmp", "ftop.bmp");
	factoryLarge = new FactoryLarge(); 
	factoryLarge->setTexture("flfront.bmp", "flback.bmp", "flside.bmp", "fltop.bmp");
	banana = new Banana("banana.md2");
	groundPlant = new GroundPlant("plant3.md2");
	water = new Water("waterheightmap.bmp", 10);
	water->setTexture("water.bmp");
//	car->carX = -4940;
//	car->carZ = -4890;
	pole = new Pole();
	bananaTree = glGenLists(1);
	earth = new Earth();
	earth->setTexture("clouds.bmp");
	mainTerrain = new WorldTerrain("terrainCheck.bmp", 20);
	//earth->CreateSphere(70, 0, 0, 0);
	glNewList(bananaTree, GL_COMPILE);
	for (int i = -1650; i <= 630; i += 50) {
		for (int j = 2050; j <= 3840; j += 50) {
			glPushMatrix();
			glTranslated(j, 0.0, i);
			glScalef(5, 10, 5);
			banana->draw();
			glPopMatrix();
		}
	}
	for (int i = -1650; i <= 630; i += 75) {
		for (int j = 2050; j <= 3840; j += 75) {
			glPushMatrix();
			glTranslated(j + 5, 0.0, i);
			glScalef(5, 10, 5);
			groundPlant->draw();
			glPopMatrix();
		}
	}
	glEndList();
	terrain = glGenLists(1);
	glNewList(terrain, GL_COMPILE);
		glScalef(200, 1, 200);
		wt->draw();
	glEndList();
	houses = glGenLists(1);
	glNewList(houses, GL_COMPILE);
	glPushMatrix();
	glTranslatef(100, 100, 3705);
	glRotatef(-180, 0, 1, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();

	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(100, 100, 3150);
	glRotatef(-180, 0, 1, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();

	glPopMatrix();


	glPushMatrix();
	glTranslatef(-250, 100, 3150);
	glRotatef(-180, 0, 1, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-250, 100, 3705);
	glRotatef(-180, 0, 1, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glTranslatef(200, 0, 0);
	glPushMatrix();
	glScalef(10, 10, 10);
	house->draw();
	glPopMatrix();
	glPopMatrix();
	glEndList();
	industries = glGenLists(1);
	glNewList(industries, GL_COMPILE);
	glPushMatrix();	
	glTranslatef(-2200, 25, -1220);
	glPushMatrix();
	glScalef(10, 5, 10);
	factorySmall->draw();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-300, 0, 0);
	glScalef(10, 5, 10);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(-300, 0, 0);
	glPushMatrix();
	glScalef(10, 5, 10);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(-300, 0, 0);
	glPushMatrix();
	glScalef(10, 5, 10);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(-300, 0, 0);
	glPushMatrix();
	glScalef(10, 5, 10);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(-300, 0, 0);
	glPushMatrix();
	glScalef(10, 5, 10);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(-300, 0, 0);
	glPushMatrix();
	glScalef(10, 5, 10);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(-300, 0, 200);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(45, 0, 1, 0);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(-200, 0, 500);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(270, 0, 1, 0);
	factoryLarge->draw();
	glPopMatrix();
	glTranslatef(0, 0, 500);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(270, 0, 1, 0);
	factoryLarge->draw();
	glPopMatrix();
	glTranslatef(150, 0, 200);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(180, 0, 1, 0);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(300, 0, 0);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(180, 0, 1, 0);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(350, 0, 300);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(270, 0, 1, 0);
	factorySmall->draw();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 180);
	glScalef(10, 5, 10);
	glRotatef(180, 0, 1, 0);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(350, 0, 150);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(180, 0, 1, 0);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(350, 0, 0);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(180, 0, 1, 0);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(400, 0, 0);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(270, 0, 1, 0);
	factoryLarge->draw();
	glPopMatrix();
	glTranslatef(150, 0, -500);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(270, 0, 1, 0);
	factoryLarge->draw();
	glPopMatrix();
	glTranslatef(0, 0, -500);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(270, 0, 1, 0);
	factorySmall->draw();
	glPopMatrix();
	glTranslatef(0, 0, -500);
	glPushMatrix();
	glScalef(10, 5, 10);
	glRotatef(270, 0, 1, 0);
	factoryLarge->draw();
	glPopMatrix();
	glPopMatrix();
	glEndList();


//                       Image *image = loadBMP("texture.bmp");
  //                      _textureId = loadTexture(image);
    //                    delete image;
	car1->carX = -50;
	car1->carZ = 275;
	car2->carX = -50;
	car2->carZ = 300;
}

void handleResize(int w, int h) {
	_windowWidth = w;
	_windowHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1, 15000.0);
}

void keyOperations() {
	float _newCar1X, _newCar1Z, _newCar2Z, _newCar2X;;
	float timeElapsed = 1;//getElapsedTime();
	if (specialKeys[GLUT_KEY_UP]) {
		car1->accelerate();
	}
	else if (specialKeys[GLUT_KEY_DOWN]) {
		car1->deccelerate();
	}
	else {
		car1->normalAccelerate();
	}
	if (simpleKeys['w']) {
		car2->accelerate();
	}
	else if (simpleKeys['s']) {
		car2->deccelerate();
	}
	else {
		car2->normalAccelerate();
	}

	car1->changeVelocity(timeElapsed);
	car2->changeVelocity(timeElapsed);
//	cout<<"Car velocity: "<<car->carVelocity<<endl;
	float distance1 = car1->carVelocity * timeElapsed;
	float distance2 = car2->carVelocity * timeElapsed;
//	cout<<"carangle:"<<_carAngle<<endl;
	_newCar1X = car1->carX - (distance1 * sin(car1->carAngle * 3.14/180));	
	_newCar1Z = car1->carZ - (distance1 * cos(car1->carAngle * 3.14/180));	
	
	_newCar2X = car2->carX - (distance2 * sin(car2->carAngle * 3.14/180));	
	_newCar2Z = car2->carZ - (distance2 * cos(car2->carAngle * 3.14/180));	

	if ((car1->carVelocity > 0) && (specialKeys[GLUT_KEY_LEFT])) {
		car1->moveForwardLeft(timeElapsed, distance1);
	}
	else if ((car1->carVelocity > 0) && (specialKeys[GLUT_KEY_RIGHT])) {
		car1->moveForwardRight(timeElapsed, distance1);
	}
	else if ((car1->carVelocity < 0) && specialKeys[GLUT_KEY_LEFT]) {
		car1->moveBackLeft(timeElapsed, distance1);
	}
	else if ((car1->carVelocity < 0) && specialKeys[GLUT_KEY_RIGHT]) {
		car1->moveBackRight(timeElapsed, distance1);
	}
	else {
		car1->moveStraight(timeElapsed, distance1);
	}
	
	if ((car2->carVelocity > 0) && (simpleKeys['a'])) {
		car2->moveForwardLeft(timeElapsed, distance2);
	}
	else if ((car2->carVelocity > 0) && (simpleKeys['d'])) {
		car2->moveForwardRight(timeElapsed, distance2);
	}
	else if ((car2->carVelocity < 0) && simpleKeys['a']) {
		car2->moveBackLeft(timeElapsed, distance2);
	}
	else if ((car2->carVelocity < 0) && simpleKeys['d']) {
		car2->moveBackRight(timeElapsed, distance2);
	}
	else {
		car2->moveStraight(timeElapsed, distance2);
	}
//	carHeight = raceTrack->getHeight(tempCarX, tempCarZ);
//	if (!checkCollision()) {
	if (!checkCollision(_newCar1X, _newCar1Z)) {
		car1->carX = _newCar1X;
		car1->carZ = _newCar1Z;
	}
	if (!checkCollision(_newCar2X, _newCar2Z)) {
		car2->carX = _newCar2X;
		car2->carZ = _newCar2Z;
	}
//	}
//	else {
//		_carVelocity = 0;
//	}
	//cout<<car1->carX<<","<<car1->carY<<","<<car1->carZ<<endl;
}

void drawScene() {
	if (flat) {
		glShadeModel(GL_FLAT);
	}
	else {
		glShadeModel(GL_SMOOTH);
	}
	glClearColor(1.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat ambientColor[] = {ambientRed, ambientGreen, ambientBlue, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	if (light){
		GLfloat diffuseColor[] = {1.0f, 1.0f, 0.5f, 1.0f};
//		GLfloat specularColor[] = {0.0f, 0.0f, 1.0f, 1.0f};
		GLfloat lightPos[] = {lightRadius * sin(lightAngle * 3.14 / 180), lightRadius * cos(lightAngle * 3.14 / 180), 0, 0.0};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//		glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
		lightAngle += 0.5;
		//cout<<"Light angle: "<<lightAngle<<endl;
		if (lightAngle > 360) {
			lightAngle -= 360;
		}
		if (((lightAngle > 120) && (lightAngle < 240))) {
			glDisable(GL_LIGHT0);
		}
		else {
			glEnable(GL_LIGHT0);
		}
	}
	if (_cameraPosition == 0) {
		float distance = 10;
		float offset = -40;
		gluLookAt(car1->carX - offset * sin((car1->carAngle ) * 3.14 / 180), car1->carY + 8, car1->carZ - offset * cos(car1->carAngle * 3.14 / 180), car1->carX + distance * sin(car1->carAngle * 3.14 / 180), car1->carY + 5, car1->carZ + distance * cos(car1->carAngle * 3.14 / 180), 0, 1, 0);
	}
	else if (_cameraPosition == 1) {
		float distance = 10;
		float offset = -40;
		gluLookAt(car2->carX - offset * sin((car2->carAngle ) * 3.14 / 180), car2->carY + 8, car2->carZ - offset * cos(car2->carAngle * 3.14 / 180), car2->carX + distance * sin(car2->carAngle * 3.14 / 180), car2->carY + 5, car2->carZ + distance * cos(car2->carAngle * 3.14 / 180), 0, 1, 0);
	}
	else if (_cameraPosition == 2) {
		gluLookAt(_freeEyeX, _freeEyeY, _freeEyeZ, _freeCenterX, _freeCenterY, _freeCenterZ, 0.0, 1.0, 0.0);
	}
//	gluLookAt(ex, ey, ez, ex, 8, ez + 10, 0, 1, 0);
//	cout<<ex<<","<<ey<<","<<ez<<endl;
//	gluLookAt(0.001 * (car->carX - offset * sin((car->carAngle ) * 3.14 / 180)), (0.0001 + car->carY - 0.05) * 0.001, 0.001 * (car->carZ - offset * cos(car->carAngle * 3.14 / 180)) - 0.1, 0.001 * (car->carX - distance * sin(car->carAngle * 3.14 / 180)),  0.001 * car->carY -0.05, 0.001 * (car->carZ - distance * cos(car->carAngle * 3.14 / 180)) -0.1, 0, 1, 0);
//	gluLookAt(car->carX, car->carY + 0.5 ,car->carZ, 0.001 * car->carX, 0.001 * car->carY, 0.001 * car->carZ, 0, 0, -1);
//	gluLookAt(ex + 200,1000, ez + 3705, ex + 200, 0.0, ez + 3705, 0.0, 0.0, -1.0);
	glPushMatrix();
	if (diffSpec) {
		GLfloat diffuseMaterial[] = {0.5, 0.5, 0.5, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
		GLfloat specularMaterial[] = {0.0, 0.0, 0.0, 0.0};
		GLfloat shininess[] = {0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	}
	else {
		GLfloat specularMaterial[] = {0.2, 0.2, 0.2, 1.0};
		GLfloat shininess[] = {64};
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		GLfloat diffuseMaterial[] = {0.0, 0.0, 0.0, 0.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
	}
	glPushMatrix();
	glScalef(3000, 3000, 3000);
	earth->draw();
	glPopMatrix();
	glTranslatef(-55.0, 10.0, -410.0);
	pole->draw();
	glPopMatrix();
	glPushMatrix();
	glCallList(terrain);
//	glTranslatef(0.0, 0.0, -5.0);
	glPopMatrix();
	glPushMatrix();
	glCallList(bananaTree);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -10, 0);
//	glTranslatef(0.0, ey, ez);
//	glScalef(0.001, 0.001, 0.001);
//	glPushMatrix();
//	earth->draw();
//	glPopMatrix();
	glCallList(houses);
	glCallList(industries);


	glPushMatrix();
	for (int i = -4940; i < 4880; i += 256) {
		for (int j = -4890; j < -4530; j += 256) {
			glPushMatrix();
			glTranslated(i, 1.0, j);
			glScalef(55, 10, 55);
			water->draw();
			glPopMatrix();
		}
	}
	glPopMatrix();
	glPopMatrix();
	glTranslatef(0, -10, 0);
	glPushMatrix();
/*	float offset = 7.0;
	float lightX = car1->carX - offset * sin((car1->carAngle ) * 3.14 / 180);
	float lightZ = car1->carZ - offset * cos((car1->carAngle ) * 3.14 / 180);
	float pointX = car1->carX - (offset + 10) * sin((car1->carAngle ) * 3.14 / 180);
	float pointZ = car1->carZ - (offset + 10) * cos((car1->carAngle ) * 3.14 / 180);
	glEnable(GL_LIGHT1);
	GLfloat color[] = {1, 1, 0, 1.0};
	GLfloat position[] = {lightX, 2.0, lightZ, 1.0};
	GLfloat direction[] = {pointX, 1.0, pointZ, 1.0};
	GLfloat cutoffAngle[] = {10};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, cutoffAngle);
	glLightfv(GL_LIGHT1, GL_POSITION, position);
//	GLfloat attenuationFactor[] = {0.5};
//	glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, attenuationFactor);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	//glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, 128);*/
	car1->draw();
	glPopMatrix();
	glPushMatrix();
/*	offset = 7.0;
	lightX = car2->carX - offset * sin((car2->carAngle ) * 3.14 / 180);
	lightZ = car2->carZ - offset * cos((car2->carAngle ) * 3.14 / 180);
	pointX = car2->carX - (offset + 10) * sin((car2->carAngle ) * 3.14 / 180);
	pointZ = car2->carZ - (offset + 10) * cos((car2->carAngle ) * 3.14 / 180);
	glEnable(GL_LIGHT2);
	GLfloat direction2[] = {pointX, 1.0, pointZ, 1.0};
	GLfloat color2[] = {1, 1, 0, 1.0};
	GLfloat position2[] = {lightX, 2.0, lightZ, 1.0};
	GLfloat cutoffAngle2[] = {10};
	glLightfv(GL_LIGHT2, GL_DIFFUSE, color2);
	glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, cutoffAngle2);
	glLightfv(GL_LIGHT2, GL_POSITION, position2);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction2);
//	glLightfv(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, attenuationFactor);*/
	car2->draw();
	glPopMatrix();
	glutSwapBuffers();
	keyOperations();
}

void keySpecial(int key, int x, int y) {
	//cout<<"Key pressed: "<<key<<endl;
	specialKeys[key] = true;
}

void keySpecialUp(int key, int x, int y) {
	//cout<<"Key left: "<<key<<endl;
	specialKeys[key] = false;
}

void special(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		ez += 10.0;
	}
	else if (key == GLUT_KEY_DOWN) {
		ez -= 10.0;
	}
	else if (key == GLUT_KEY_LEFT) {
		ex += 10.0;
	}
	else if (key == GLUT_KEY_RIGHT) {
		ex -= 10.0;
	}
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	_windowWidth = 800;
	_windowHeight = 600;
	glutInitWindowSize(_windowWidth, _windowHeight);

	glutCreateWindow("3D World");
	initRendering();

	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleKeyRelease);
	glutSpecialFunc(keySpecial);
	glutSpecialUpFunc(keySpecialUp);
	glutReshapeFunc(handleResize);
	glutMouseFunc(handleMousePress);
	glutMotionFunc(handleMouseActiveMove);

	glutMainLoop();
	return 0;
}
