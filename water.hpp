/**
* \file water.hpp
*
* A vizet reprezentáló osztály forráskódja itt található.
*
*/

#ifndef WATER
#define WATER

#include "defs.h"
#include "dds.hpp"

typedef void(*rNrDraw)();	

class Water {
	float Width;		
	float Height;		
	float Time;			

	Vector WaveDirX;	
	Vector WaveDirY;	

	Vector SpaceFreq;	
	Vector Amplitudes;	
	Vector TimeFreq;	

	GLuint cubemap;		//cube map identifier
	GLuint normalmap;	//normal map identifier
	GLuint reflection;	//reflection identifier
	GLuint refraction;	//refraction identifier
	GLuint depth;		//depth identifier

	cgObject *cgVertexObject;	//vertex shader
	cgObject *cgFragmentObject;	//fragment shader
 
public:

	Water(float Width, float Height) :
	Width(Width), Height(Height),
	WaveDirX(-0.5, 0.1, 0.0, 0.0),
	WaveDirY(0.7, 0.5, 0.0, 0.0),
	SpaceFreq(0.5, 0.45, 0.0, 0.0),
	Amplitudes(0.6, 0.7, 0.0, 0.0),
	TimeFreq(0.125, 0.25, 0.0, 0.0)
	{
		Time = 0.01;
    }

	void init() {

		GLint m_width = 512;
		GLint m_height = 512;

		//reflection texture
		glGenTextures(1, &reflection);
		glBindTexture(GL_TEXTURE_2D, reflection);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//refraction texture
		glGenTextures(1, &refraction);
		glBindTexture(GL_TEXTURE_2D, refraction); 
		glTexImage2D(GL_TEXTURE_2D, 0, 3, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//depth texture
		glGenTextures(1, &depth);
		glBindTexture(GL_TEXTURE_2D, depth); 
		glTexImage2D(GL_TEXTURE_2D, 0, 3, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//cube map
		glGenTextures(1, &cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		loadCubeMapFromDDS("WaterCubemap.dds");
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//normal map
		glGenTextures(1, &normalmap);
		glBindTexture(GL_TEXTURE_2D, normalmap);
		loadNormalFromDDS("WaterNormalmap.dds");
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	}

	void WaterIdle(float dt) {
		Time = dt * 0.015;
	}
 
	void Draw(float ex, float ey, float ez) {

		Material water_material = {
		
			{ 0.0, 0.0, 0.9, 1.0 },
			{ 0.0, 0.0, 0.9, 1.0 },
			{ 0.6, 0.6, 1.0, 1.0 },
			32.0
		};

		//cout << camera->px;
		//GLfloat global_ambient[] = { 1, 1, 1, 1 };

		//float Time = 1.0;
		//float SpaceFreq[4] = {0.5f, 0.5f, 0.0f, 0.0f};
		//float Amplitudes[4] = {0.4f, 0.4f, 0.0f, 0.0f};
		//float TimeFreq[4] = {0.125f, 0.25f, 0.0f, 0.0f};
		//float WaveDirX[4] = {-0.5f, 0.1f, 0.0f, 0.0f};
		//float WaveDirY[4] = {0.7f, 0.5f, 0.0f, 0.0f};

		float SpaceFreq[4] = {0.5, 0.45, 0.0, 0.0};
		float Amplitudes[4] = {0.6, 0.7, 0.0, 0.0};
		float TimeFreq[4] = {0.125, 0.25, 0.0, 0.0};
		float WaveDirX[4] = {-0.5, 0.1, 0.0, 0.0};
		float WaveDirY[4] = {0.7, 0.5, 0.0, 0.0};

		if (cgVertexObject == NULL)
			cgVertexObject = new cgObject("waves.cg", "main", CG_GL_VERTEX);

		if (cgFragmentObject == NULL)
			cgFragmentObject = new cgObject("waterShader.cg", "main", CG_GL_FRAGMENT);

		cgFragmentObject->setCgParam("Time", Time);
		
		CGparameter water_reflection = cgFragmentObject->getCgParam("water_reflection");
		CGparameter water_refraction = cgFragmentObject->getCgParam("water_refraction");
		CGparameter water_cubemap = cgFragmentObject->getCgParam("water_cubemap");
		CGparameter water_normalmap = cgFragmentObject->getCgParam("water_normalmap");
		CGparameter water_depthmap = cgFragmentObject->getCgParam("water_depthmap");

		float EyePos[4] = {ex, ey, ez, 1};
		//float ModelToWorld[16];
		//glGetFloatv(GL_MODEL_MATRIX, ModelToWorld);


		//CGparameter MVIT = cgVertexObject->getCgParam("MVIT");
		//cgGLSetStateMatrixParameter(MVIT, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE_TRANSPOSE);

		//CGparameter MVP = cgVertexObject->getCgParam("MVP");
		//cgGLSetStateMatrixParameter(MVP, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

		//CGparameter MV = cgVertexObject->getCgParam("MV");
		//cgGLSetStateMatrixParameter(MV, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);

		cgVertexObject->setCgParam4("EyePos", EyePos);
		cgVertexObject->setCgParam4("SpaceFreq", SpaceFreq);
		cgVertexObject->setCgParam4("Amplitudes", Amplitudes);
		cgVertexObject->setCgParam4("TimeFreq", TimeFreq);
		cgVertexObject->setCgParam("Time", Time);
		cgVertexObject->setCgParam4("WaveDirX", WaveDirX);
		cgVertexObject->setCgParam4("WaveDirY", WaveDirY);

		cgGLSetTextureParameter(water_cubemap, cubemap);
		cgGLEnableTextureParameter(water_cubemap);

		cgGLSetTextureParameter(water_normalmap, normalmap);
		cgGLEnableTextureParameter(water_normalmap);

		cgGLSetTextureParameter(water_reflection, reflection);
		cgGLEnableTextureParameter(water_reflection);

		cgGLSetTextureParameter(water_depthmap, depth);
		cgGLEnableTextureParameter(water_depthmap);

		cgGLSetTextureParameter(water_refraction, refraction);
		cgGLEnableTextureParameter(water_refraction);

		cgFragmentObject->update();

		cgVertexObject->render();
		cgFragmentObject->render();

		glPushMatrix();

		//glScalef(0.5, 0.5, 0.5);

		for(int i = 0; i < 200; i++) {

			glBegin(GL_TRIANGLE_STRIP);

		
			for(int j = 0; j < 200; j++) {

					glVertex3f(100-i, 100-j, 0);
					glVertex3f(100-i+1, 100-j, 0);
			}

			glEnd();

		}

        glPopMatrix();

		cgVertexObject->kill();
		cgFragmentObject->kill();
    }

	
	float GetHeightAndNormal(float x, float y, Vector& normal) {
		Vector wp = WaveDirX * x + WaveDirY * y;
		Vector Phase = Vector(wp.x * SpaceFreq.x, wp.y * SpaceFreq.y, wp.z * SpaceFreq.z, wp.w * SpaceFreq.w) + TimeFreq * Time;
		
		Vector Sin(sin(Phase.x), sin(Phase.y), sin(Phase.z), sin(Phase.w));
		Vector Cos(cos(Phase.x), cos(Phase.y), cos(Phase.z), cos(Phase.w));
		
		Vector CosWaveHeight(Amplitudes.x * SpaceFreq.x * Cos.x,
		Amplitudes.y * SpaceFreq.y * Cos.y, Amplitudes.z * SpaceFreq.z * Cos.z,
		Amplitudes.w * SpaceFreq.w * Cos.w);

		Vector tangent = Vector(1, 0, CosWaveHeight * WaveDirX);
		Vector binormal = Vector(0, 1, CosWaveHeight * WaveDirY);
		normal = tangent % binormal;
		return (Sin * Amplitudes);
	}

	
	void RenderReflection(rNrDraw rnrDraw) {

		GLint m_width = 512;
		GLint m_height = 512;

		glViewport(0, 0, m_width, m_height);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//gluLookAt(ex, 0, 0, 0, 0, -1, 0, 0, -1);

		glPushMatrix();
		glScalef(1, 1, -1);
		//glTranslatef(0.0f, 0.0f, 0.0f);
		//glScalef(1.0, -1.0, 1.0);
		double plane[4] = {0.0, 0.0, -1.0, 0.0};
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, plane); 

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		rnrDraw();
		glDisable(GL_CULL_FACE);


		glDisable(GL_CLIP_PLANE0);
		glPopMatrix();

		//render reflection to texture
		glBindTexture(GL_TEXTURE_2D, reflection);

		//glCopyTexSubImage2D copies the frame buffer
		//to the bound texture
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_width, m_height);

	}

	
	void RenderRefractionAndDepth(rNrDraw rnrDraw) {

		GLint m_width = 512;
		GLint m_height = 512;

		glViewport(0, 0, m_width, m_height);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//gluLookAt(ex, ey, ez, 0, 0, 0, 0, 0, -1);

		glPushMatrix();
		//glTranslatef(0.0f, 0.0f, 0.0f);
		//normal pointing along negative y
		double plane[4] = {0.0, 0.0, 1.0, 0.0};
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, plane); 

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		rnrDraw();
		glDisable(GL_CULL_FACE);

		glDisable(GL_CLIP_PLANE0);
		glPopMatrix();

		//render color buffer to texture
		glBindTexture(GL_TEXTURE_2D, refraction);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_width, m_height);

		//render depth to texture
		glBindTexture(GL_TEXTURE_2D, depth);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, m_width, m_height, 0);


	}
};

#endif
