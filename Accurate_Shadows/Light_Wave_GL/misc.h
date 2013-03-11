
#include "GLee/GLee.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include "windows.h"
#endif
#include "GL/glut.h"
#endif

#ifndef _MISC_H_
#define _MISC_H_

#define msglError( ) _msglError( stderr, __FILE__, __LINE__ )

bool _msglError( FILE *out, const char *filename, int line );

bool supportsOpenGLVersion( int atLeastMajor, int atLeastMinor );

char* file2strings( const char *filename );

void vecDifference( float a[3], float b[3], float c[3] );

void crossProduct( float a[3], float b[3], float c[3] );

void vecNormalize( float n[3] );

#endif