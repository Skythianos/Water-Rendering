#include <stdlib.h>
#include <windows.h>

#include <iostream>
using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "defs.h"
#include "ImageFile.hpp"
#include "dds.hpp"
#include "shader.hpp"
#include "vector.hpp"
#include "camera.hpp"
#include "water.hpp"
#include "skybox.hpp"
#include "box.hpp"

Light light = {		
	{ 25, 25, -10, 1  },
	{ 0.95, 0.95, 0.95 }
};

class objLight {

private:
	Light light;

public:

	objLight(Light light) {
		this->light = light;
	}

	void Draw() {
		glPushMatrix();

			glTranslatef(light.position[0], light.position[1], light.position[2]);

			glutSolidSphere(0.2, 12, 12);

		glPopMatrix();
	}

};

objLight objlight(light);

Water water(400, 400);
SkyBox skybox;
Camera camera(600, 600);
Box box;

void onInitialization( ) {

	for (int i = 0; i < 256; i++) {
		keys[i] = 0;
	}

    glEnable(GL_DEPTH_TEST);
	
	camera.init();

	water.init();
	skybox.init();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
    //lighting();
    //glEnable(GL_LIGHTING);
 
    //glShadeModel(GL_SMOOTH);

}

void rnrDraw() {

	box.Draw();
}

int frame = 0;

void onDisplay() {

	frame++;

	//camera.init();
    camera.setCamera();

	water.RenderReflection(&rnrDraw);
	water.RenderRefractionAndDepth(&rnrDraw);

	glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
	camera.setViewport();

	skybox.Draw(camera.position.x, camera.position.y, camera.position.z);
 
	glClear(GL_DEPTH_BUFFER_BIT);

	//int m_width = 256, m_height = 256;
	//ImageFile test("sky.bmp", m_width, m_height);
	//glDrawPixels(m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, test.Load());
	//cout << m_width << endl;

	//objlight.Draw();

	box.Draw();

	water.Draw(camera.position.x, camera.position.y, camera.position.z);
 
    glutSwapBuffers();
}


double teszt = 0.1;

void onKeyboard(unsigned char key, int x, int y) {

	keys[key] = 1;
}

void onKeyboardUp(unsigned char key, int x, int y) {
	keys[key] = 0;
}

void onMotionMouse(int x, int y) {
	camera.angx -= (x - mouse_position[0]) * 0.005;
	camera.angy -= (mouse_position[1] - y) * 0.005;

	mouse_position[0] = x;
	mouse_position[1] = y;
}

void onPassiveMotionMouse(int x, int y) {
	mouse_position[0] = x;
	mouse_position[1] = y;
}

int elapsed = 0, now = 0;

int fpstime, timebase=0;
char s[50];

void onIdle() {

	fpstime = glutGet(GLUT_ELAPSED_TIME);

	now = glutGet(GLUT_ELAPSED_TIME);

	if (fpstime - timebase > 1000) {
		sprintf(s, "Water shading @FPS:%4.2f", frame * 1000.0/(fpstime-timebase));
	 	timebase = fpstime;		
		frame = 0;
	}
	//HWND
	//LOGFONT lf;
	//HWND hwnd = FindWindow(NULL, "Water shading @FPS:%4.2f");
	//HDC hdc = GetDC(hwnd);
	//static HFONT hf;

	//lf.lfOrientation = 0;
	//lf.lfEscapement = 0;
	//lf.lfHeight = 30;
	//lf.lfWidth = 10;
	//lf.lfCharSet = ANSI_CHARSET;
	//strcpy(lf.lfFaceName, "Arial");
	//hf = CreateFontIndirect(&lf);
	//
	//SelectObject(hdc, hf);
	//TextOut(hdc, 10, 10, "LOOOL0", 10);
	//ReleaseDC(hwnd, hdc);

	//sprintf(s, "%d", now);
	glutSetIconTitle(s);

	//now = 0;
	
	water.WaterIdle(glutGet(GLUT_ELAPSED_TIME));

	box.setZ(camera.moveBox);
	box.Collision(water);
	box.AnimIt(camera.animIt);
	box.Idle(glutGet(GLUT_ELAPSED_TIME));
	camera.setKeyboard();

	glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv); 
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Water Shading");

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    onInitialization();

    glutDisplayFunc(onDisplay);

    glutMotionFunc(onMotionMouse);
	glutPassiveMotionFunc(onPassiveMotionMouse);

    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);

    glutMainLoop();
    
    return 0;
}