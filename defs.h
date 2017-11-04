#ifndef DEFINES
#define DEFINES

#define _USE_MATH_DEFINES
#include <math.h>

char keys[256];
GLfloat mouse_position[2];

typedef struct _Material {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} Material;

typedef struct _Light {
	GLfloat position[4];
	GLfloat color[3];
} Light;

typedef struct _TextCoord2f {
    GLfloat u, v;

    _TextCoord2f(GLfloat u = 0, GLfloat v = 0) {
        this->u = u;
        this->v = v;
    }
} TextCoord2f;

#endif