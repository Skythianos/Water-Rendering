/**
* \file skybox.hpp
*
* 
*
*/

#ifndef SKYBOX
#define SKYBOX

#include "defs.h"

class SkyBox {
	GLuint sky;					
	GLUquadricObj * quadric;

public:

	
	void init() {

		int m_width, m_height;

		ImageFile img_sky("sky.bmp", m_width, m_height);

		quadric = gluNewQuadric();
		gluQuadricTexture(quadric, GL_TRUE);

		glGenTextures(1, &sky);
		glBindTexture(GL_TEXTURE_2D, sky);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_sky.Load());

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	}

	void Draw(float ex, float ey, float ez) {

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//gluQuadricOrientation(quadric, GLU_OUTSIDE);
		//gluQuadricDrawStyle(quadric, GLU_FILL);
		//gluQuadricNormals(quadric, GLU_SMOOTH);
		//gluQuadricTexture(quadric, GL_TRUE);


		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		glBindTexture(GL_TEXTURE_2D, sky);

		glPushMatrix();
		glTranslatef(ex, ey, ez);
		gluSphere(quadric, 50, 32, 32);
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
};

#endif
