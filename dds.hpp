/**
* \file box.hpp
*
* 
*
*/

#ifndef DDS
#define DDS

#include <GL/glext.h>
#include <assert.h>
#include <iostream>
using namespace std;

void loadCubeMapFromDDS(const char *filename);
void loadDecalFromDDS(const char *filename);

#define DDSD_CAPS        0x00000001
#define DDSD_HEIGHT      0x00000002
#define DDSD_WIDTH       0x00000004
#define DDSD_PITCH       0x00000008
#define DDSD_PIXELFORMAT 0x00001000
#define DDSD_MIPMAPCOUNT 0x00020000
#define DDSD_LINEARSIZE  0x00080000
#define DDSD_DEPTH       0x00800000

#define DDSCAPS2_CUBEMAP		   0x00000200 
#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400 
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800 
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000 
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000 
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000 
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000 
#define DDSCAPS2_VOLUME            0x00200000 

struct DDCOLORKEY {
	DWORD dwColorSpaceLowValue;
    DWORD dwColorSpaceHighValue;
};

struct DDSCAPS2 {
    DWORD dwCaps;
    DWORD dwCaps2;
    DWORD dwCaps3;
    union {
        DWORD dwCaps4;
        DWORD dwVolumeDepth;
    };
};

struct DDPIXELFORMAT {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwFourCC;

    union {
        DWORD dwRGBBitCount;
        DWORD dwYUVBitCount;
        DWORD dwZBufferBitDepth;
        DWORD dwAlphaBitDepth;
        DWORD dwLuminanceBitCount;
        DWORD dwBumpBitCount;
        DWORD dwPrivateFormatBitCount;
    };
    union {
        DWORD dwRBitMask;
        DWORD dwYBitMask;
        DWORD dwStencilBitDepth;
        DWORD dwLuminanceBitMask;
        DWORD dwBumpDuBitMask;
        DWORD dwOperations;
    };
    union {
        DWORD dwGBitMask;
        DWORD dwUBitMask;
        DWORD dwZBitMask;
        DWORD dwBumpDvBitMask;
        struct {
            WORD    wFlipMSTypes;
            WORD    wBltMSTypes;
        } MultiSampleCaps;
    };
    union {
        DWORD dwBBitMask;
        DWORD dwVBitMask;
        DWORD dwStencilBitMask;
        DWORD dwBumpLuminanceBitMask;
    };
    union {
        DWORD dwRGBAlphaBitMask;
        DWORD dwYUVAlphaBitMask;
        DWORD dwLuminanceAlphaBitMask;
        DWORD dwRGBZBitMask;
        DWORD dwYUVZBitMask;
    };
};

struct DDSURFACEDESC2 {

	DWORD dwMagic;
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwHeight;
    DWORD dwWidth;

    union { 
        LONG  lPitch;
        DWORD dwLinearSize;
    };

    DWORD dwBackBufferCount;

    union { 
        DWORD dwMipMapCount;
        DWORD dwRefreshRate;
        DWORD dwSrcVBHandle;
    };

    DWORD  dwAlphaBitDepth;
    DWORD  dwReserved;
    LPVOID lpSurface;

    union {
        DDCOLORKEY ddckCKDestOverlay;
        DWORD	   dwEmptyFaceColor;
    };

    DDCOLORKEY ddckCKDestBlt;
    DDCOLORKEY ddckCKSrcOverlay;
    DDCOLORKEY ddckCKSrcBlt;

    union {
        DDPIXELFORMAT ddpfPixelFormat;
        DWORD         dwFVF;
    };
    DDSCAPS2 ddsCaps;
    DWORD    dwTextureStage;
};

void loadCubeMapFromDDS(const char *filename) { 

	FILE *file = fopen(filename, "rb");
	long size;
	void *data;
	char *beginning, *image;
	int *words;
	size_t bytes;
	DDSURFACEDESC2 *header;
	int i, face, level;

	if (!file) {
		fprintf(stderr, "cubemap file not found");
		exit(1);
	}

	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	data = (char*) malloc(size);

	if (data == NULL) {
		fprintf(stderr, "Mem error...");
		exit(1);
	}

	bytes = fread(data, 1, size, file);
	fclose(file);

	if (bytes < sizeof(DDSURFACEDESC2)) {
		fprintf(stderr, "file smaller than header...");
		exit(1);
	}

	header = (DDSURFACEDESC2*)data;
	beginning = (char *)data;
	image = (char *)&header[1];

	GLuint levels = 1;
	GLuint width = 0;
	GLuint height = 0;

	if(header->dwFlags & DDSD_MIPMAPCOUNT) { 
		levels = header->dwMipMapCount;
	}

	if(header->dwFlags & (DDSD_HEIGHT | DDSD_WIDTH)) { 
		width = header->dwWidth;
		height = header->dwHeight;
	}

	if(header->ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP) { 
		cout << "====CubeMap====" << endl;
	}

	if (header->ddpfPixelFormat.dwRGBBitCount & 32) { 
		cout << "32bit_";
		if (header->ddpfPixelFormat.dwRBitMask & 0x00ff0000) {
			cout << "DX_B8G8R8";

		}

		if (header->ddpfPixelFormat.dwRBitMask & 0x000000ff) {
			cout << "GL_R8G8B8";
		}

		if (header->ddpfPixelFormat.dwRGBAlphaBitMask && 0xff000000) {
			cout << "A8";
		} else {
			cout << "X8";
		}
	}

	if (header->ddpfPixelFormat.dwRGBBitCount & 24) {
		cout << "24bit_";

		if (header->ddpfPixelFormat.dwRBitMask & 0xff0000) {
			cout << "DX_B8G8R8";
		}

		if (header->ddpfPixelFormat.dwRBitMask & 0x0000ff) {
			cout << "GL_R8G8B8";
		}
	}

	cout << endl;

	cout << "MipMapCount: " << levels << endl;
	cout << "Width: " << width << endl;
	cout << "Height: " << height << endl;
	cout << "==CubeMap=END==" << endl;

	
	for (face=0; face<6; face++) {

		
		width = header->dwWidth;
		height = header->dwHeight;
		const int border = 0;

		
		for (level=0; level<levels; level++) {

			GLuint skip = width * height * 4;

			GLint offsetInToRead = image + skip - beginning;

			if (offsetInToRead > bytes) {
				fprintf(stderr, "Over load");
				exit(1);
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, GL_RGBA8, width, height, border, GL_BGRA, GL_UNSIGNED_BYTE, image);

			image += skip;

			width = width >> 1;

			if (width < 1) {
				width = 1;
			}

			height = height >> 1;

			if (height < 1) {
				height = 1;
			}
		}

		if (levels > 1) {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, header->dwMipMapCount-1);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

	}
	assert(image <= beginning + bytes);
  
	free(data);
}

void loadNormalFromDDS(const char *filename) { 

	FILE *file = fopen(filename, "rb");
	
	long size;
	void *data;
	char *beginning, *image;
	int *words;
	size_t bytes;
	DDSURFACEDESC2 *header;
	int i, level;

	if (!file) {
		fprintf(stderr, "normalmap file not found");
		exit(1);
	}

	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	data = (char*) malloc(size);
	
	if (data == NULL) {
		fprintf(stderr, "Mem error...");
		exit(1);
	}
	
	bytes = fread(data, 1, size, file);
	fclose(file);

	if (bytes < sizeof(DDSURFACEDESC2)) {
		fprintf(stderr, "file smaller than header");
		exit(1);
	}

	header = (DDSURFACEDESC2*)data;
	beginning = (char *)data;
	image = (char*) &header[1];

	GLuint levels = 1;
	GLuint width = 0;
	GLuint height = 0;

	if(header->dwFlags & DDSD_MIPMAPCOUNT) {
		levels = header->dwMipMapCount;
	}

	if(header->dwFlags & (DDSD_HEIGHT | DDSD_WIDTH)) {
		width = header->dwWidth;
		height = header->dwHeight;
	}

	cout << "====NormalMap====" << endl;

	if (header->ddpfPixelFormat.dwRGBBitCount & 32) {
		cout << "32bit_";
		if (header->ddpfPixelFormat.dwRBitMask & 0x00ff0000) {
			cout << "DX_B8G8R8";

		}

		if (header->ddpfPixelFormat.dwRBitMask & 0x000000ff) {
			cout << "GL_R8G8B8";
		}

		if (header->ddpfPixelFormat.dwRGBAlphaBitMask && 0xff000000) {
			cout << "A8";
		} else {
			cout << "X8";
		}
	}

	if (header->ddpfPixelFormat.dwRGBBitCount & 24) {
		cout << "24bit_";

		if (header->ddpfPixelFormat.dwRBitMask & 0xff0000) {
			cout << "DX_B8G8R8";
		}

		if (header->ddpfPixelFormat.dwRBitMask & 0x0000ff) {
			cout << "GL_R8G8B8";
		}
	}

	cout << endl;

	cout << "MipMapCount: " << levels << endl;
	cout << "Width: " << width << endl;
	cout << "Height: " << height << endl;
	cout << "==NormalMap=END==" << endl;

    const int border = 0;

	for (level=0; level<levels; level++) {

		GLuint skip = width * height * 4;

		GLint offsetInToRead = image + skip - beginning;

		if (offsetInToRead > bytes) {
			fprintf(stderr, "Over load");
			exit(1);
		}

		glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA8, width, height, border, GL_BGRA, GL_UNSIGNED_BYTE, image);

		image += skip;

		width = width >> 1;

		if (width < 1) {
			width = 1;
		}

		height = height >> 1;

		if (height < 1) {
			height = 1;
		}
	}

	assert(image <= beginning + bytes);

	if (header->dwMipMapCount > 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, header->dwMipMapCount-1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	free(data);
}

#endif
