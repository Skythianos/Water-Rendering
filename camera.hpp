/**
* \file box.hpp
*
* A kamerát reprezentáló osztály forráskódja itt található.
*
*/

#ifndef CAMERA
#define CAMERA

#include "defs.h"
#include <iostream>
using namespace std;

#define STEP 1

struct Camera {
	int winWidth, winHeight;
	Vector position;
	Vector view;
	Vector up;
	GLfloat angx, angy;
	GLfloat direction;

	GLfloat moveBox;	
	GLboolean animIt;	
	GLboolean once;
 
	Camera(int winWidth, int winHeight) : winWidth(winWidth), winHeight(winWidth) {

		moveBox = 0;
		animIt = GL_FALSE;
		once = GL_FALSE;

		position.x = 0;
		position.y = 0;
		position.z = -10;
		position.w = 1;

		view.x = 1;
		view.y = 1;
		view.z = 1;

		up.x = 0;
		up.y = 0;
		up.z = -1;

		angx = 0;
		angy = 0;
	
	}

	void init() {

		glViewport(0, 0, winWidth, winHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, (GLfloat)winWidth / (GLfloat)winHeight, 10, 1010);

	}

	void setViewport() {
		glViewport(0, 0, winWidth, winHeight);
	}

    void setCamera() { 
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		view.x = cos(angx) * cos(angy);
		view.y = -sin(angx) * cos(angy);
		view.z = sin(angy);

		gluLookAt(position.x, position.y, position.z, 
			      position.x + view.x, position.y + view.y, position.z + view.z, 
				  up.x, up.y, up.z);
	}

	void setKeyboard() { 

		if (keys['s']) {
			direction = -1;
			position = position + view * (direction * STEP);
		}

		if (keys['w']) {
			direction = 1;
			position = position + view * (direction * STEP);
		}

		if (keys['a'] == 1) {
			direction = -1;
			position = position + (view % up) * (direction * STEP);
		}

		if (keys['d'] == 1) {
			direction = 1;
			position = position + (view % up) * (direction * STEP);
		}

		if (keys['r'] == 1) {
			angx -= 0.05;
		}

		if (keys['f'] == 1) {
			angx += 0.05;
		}

		if (keys['t'] == 1) {
			angy -= 0.05;
		}

		if (keys['g'] == 1) {
			angy += 0.05;
		}

		if (keys['o'] == 1) {
			moveBox -= 0.5;
		}

		if (keys['p'] == 1) {
			moveBox += 0.5;
		}

		if (keys['i'] == 1 && !once) {
			once = GL_TRUE;
			if (animIt)
				animIt = 0;
			else
				animIt = 1;
		} 
		
		if (keys['i'] == 0) {
			once = GL_FALSE;
		}



		//if (keys['z'] == 1) {
		//	angz -= STEP;
		//}

		//if (keys['h'] == 1) {
		//	angz += STEP;
		//}



		//if (keys[' '] == 1) {
		//}
	}

	void setMouse() {
	}
 
};

#endif
