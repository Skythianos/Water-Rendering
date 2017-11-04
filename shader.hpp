/**
* \file box.hpp
*
* 
*
*/

#ifndef CGOBJECT
#define CGOBJECT

//#include <map>

#include <iostream>
using namespace std;

class cgObject {

private:
	CGcontext _CGcontext;
	CGprofile _CGprofile;
	CGprogram _CGprogram;

	CGGLenum profile_type;

	char* ProgramFileName;
	char* EntryFuncName;

	//map<const char*, CGparameter> cg_params;

	void init() { //Cg inicializálása

		_CGcontext = cgCreateContext();

		checkForCgError("creating context");

		cgGLSetDebugMode(CG_FALSE);		//DEBUG
		cgSetParameterSettingMode(_CGcontext, CG_DEFERRED_PARAMETER_SETTING); //PARAM
		_CGprofile = cgGLGetLatestProfile(profile_type); //PROFILE
		cgGLSetOptimalOptions(_CGprofile);


		_CGprogram = cgCreateProgramFromFile(
			_CGcontext,			/* Cg runtime context */
			CG_SOURCE,			/* Program in human-readable form */
			ProgramFileName,	/* Name of file containing program */
			_CGprofile,			/* Profile: OpenGL ARB vertex program */
			EntryFuncName,		/* Entry function name */
			NULL);				/* No extra compiler options */
		
		checkForCgError("creating program from file");

		cgGLLoadProgram(_CGprogram);

		checkForCgError("loading  program");

	}

public:

	cgObject(char* _ProgramFileName, char* _EntryFuncName, CGGLenum _profile_type) {
		ProgramFileName = _ProgramFileName;
		EntryFuncName = _EntryFuncName;
		profile_type = _profile_type;

		init();
	}

	CGparameter getCgParam(const char* name) { 
		return cgGetNamedParameter(_CGprogram, name);
	}

	void setCgParam(const char* name, float value) { 
		CGparameter _CGparameter = cgGetNamedParameter(_CGprogram, name);
		cgGLSetParameter1f(_CGparameter, value);
		cgUpdateProgramParameters(_CGprogram);
	}

	void setCgParam(const char* name, float* value) { 
		CGparameter _CGparameter = cgGetNamedParameter(_CGprogram, name);
		cgGLSetParameter3fv(_CGparameter, value);
		cgUpdateProgramParameters(_CGprogram);
	}

	void setCgParam4(const char* name, float* value) { 
		CGparameter _CGparameter = cgGetNamedParameter(_CGprogram, name);
		cgGLSetParameter4fv(_CGparameter, value);
		cgUpdateProgramParameters(_CGprogram);
	}

	void update() { 
		cgUpdateProgramParameters(_CGprogram);
	}

	void render() { 
		cgGLEnableProfile(_CGprofile);
		cgGLBindProgram(_CGprogram);
	}

	void kill() { 
		cgGLDisableProfile(_CGprofile);
	}

	void checkForCgError(const char *situation) { 
		CGerror error;
		const char *string = cgGetLastErrorString(&error);

		if (error != CG_NO_ERROR) {
			printf("%s: %s: %s\n", ProgramFileName, situation, string);
			if (error == CG_COMPILER_ERROR) {
				printf("%s\n", cgGetLastListing(_CGcontext));
			}
			//exit(1);
		}
	}

};

#endif
