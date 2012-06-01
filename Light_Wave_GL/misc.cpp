
#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include "misc.h"
#include "GLSLShader.h"

bool _msglError( FILE *out, const char *filename, int line ){
  bool ret = false;
#ifndef NDEBUG
  GLenum err = glGetError( );
  while( err != GL_NO_ERROR ) {
    ret = true;
    fprintf( out, "GL ERROR:%s:%d: %s\n", filename, line, (char*)gluErrorString( err ) );
      err = glGetError( );
  }
#endif
  return( ret );
}

bool supportsOpenGLVersion( int atLeastMajor, int atLeastMinor ){
  const char *version;
  int major, minor;
  bool ret = false;
  version = (const char *) glGetString(GL_VERSION);
  if (sscanf(version, "%d.%d", &major, &minor) == 2){
    //fprintf( stderr, "GL Version: major: %d, minor: %d\n", major, minor );
    if(( major < atLeastMajor ) ||  
       ( major == atLeastMajor && minor >= atLeastMinor ) )
     {
       ret = true;
     }
  }
  return( ret );
}

char* file2strings( const char *filename ){
  long fileLength, numberOfCharsRead;
  char *strings;
  FILE *fhandle = fopen( filename, "r" );
  if( !fhandle ){
    fprintf( stderr, "%s: Can't open file \"%s\" for reading\n", __FUNCTION__, filename );
    return NULL;
  }
  fseek( fhandle, 0, SEEK_END );
  fileLength = ftell( fhandle );

  fseek( fhandle, 0, SEEK_SET );

  if( !(strings = (char*)calloc( (fileLength+1), sizeof(char) )) ){
    fprintf( stderr, "%s: Can't malloc space for string buffer %s\n", __FUNCTION__, filename );
    return NULL;
  }

  numberOfCharsRead = (long)fread( strings, sizeof(char), fileLength, fhandle );

  strings[numberOfCharsRead] = '\0';

  fclose( fhandle );

  return( strings );
}

// c = a - b
void vecDifference( float a[3], float b[3], float c[3] ){
  for( int i = 0; i < 3; i++ ){
    c[i] = a[i] - b[i];
  }
}

// c = a X b
void crossProduct( float a[3], float b[3], float c[3] ){
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];  
}

void vecNormalize( float n[3] ){
  float norm;
  norm = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
  if( norm != 0 ){
    n[0] = n[0] / norm;
    n[1] = n[1] / norm;
    n[2] = n[2] / norm;
  }
}