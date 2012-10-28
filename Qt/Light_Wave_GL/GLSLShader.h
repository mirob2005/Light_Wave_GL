/*
 * Copyright (c) 2005-2010 Michael Shafae.
 * All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Id: GLSLShader.h 2517 2010-09-28 20:40:57Z mshafae $
 *
 * Utility functions for loading a shader
 *
 */

#include "GLee/GLee.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "misc.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include "windows.h"
#endif
#include "GL/glut.h"
#endif


class Shader{
public:
  GLuint _object;
  
  Shader( ){ };

  ~Shader( ){
    glDeleteShader( _object );
    msglError( );
  }

  GLint compileShader( const GLchar *src ){
    GLint compiled_ok;
    char *msg;
    GLint length = (GLint)strlen(src);
    glShaderSource( _object, 1, &src, &length );
    glCompileShader( _object );
    msglError( );
    glGetShaderiv( _object, GL_COMPILE_STATUS, &compiled_ok );
    msglError( );
    if( !compiled_ok ){
      fprintf( stderr, "Compilation failed for vertex shader\n" );
      msg = getInfoLog( );
      fprintf( stderr, "Info Log:\n%s\n", msg );
      free( msg );
    }
    return( compiled_ok );
  }
  
  char* getInfoLog( ){
    GLint info_log_length;
    glGetShaderiv( _object, GL_INFO_LOG_LENGTH, &info_log_length );
    char *info_log = (char*)malloc( info_log_length );
    glGetShaderInfoLog( _object, info_log_length, NULL, info_log);
    return( info_log );
  }
};

class VertexShader : public Shader{

public:
  VertexShader( const char *srcFileName ){
    char *src;
    if( (src = file2strings( srcFileName )) != NULL ){
      if( (Shader::_object = glCreateShader( GL_VERTEX_SHADER )) == 0 ){
        fprintf( stderr, "Can't generate vertex shader name\n" );
      }
      msglError( );
      compileShader( src );
      msglError( );
      free( src );
    }else{
      fprintf( stderr, "There was a problem opening the vertex shader file. Exiting.\n");
      exit(1);
    }
  }

  GLuint object( ){
    return Shader::_object;
  }

};


class FragmentShader : public Shader{

public:
  FragmentShader( const char *srcFileName ){
    char *src;
    if( (src = file2strings( srcFileName )) != NULL ){
      if( (Shader::_object = glCreateShader( GL_FRAGMENT_SHADER )) == 0 ){
        fprintf( stderr, "Can't generate fragment shader name\n" );
        exit(1);
      }
        compileShader( src );
        free( src );
      }else{
        fprintf( stderr, "There was a problem opening the fragment shader file. Exiting.\n");
        exit(1);
      
      }
    }

    GLuint object( ){
      return Shader::_object;
    }
};


class GLSLProgram{

public:
  GLuint _object;

  GLSLProgram( ){
    if( (_object = glCreateProgram( )) == 0 ){
      fprintf( stderr, "Can't generate program name\n" );
      exit(1);
    }
  }

  ~GLSLProgram( ){
    detachAll( );
    glDeleteProgram( _object );
  }

  bool attach( FragmentShader &fs ){
    glAttachShader( _object, fs.object( ) );
    return( !msglError( ) );
  }

  bool attach( VertexShader &vs ){
    glAttachShader( _object, vs.object( ) );
    return( !msglError( ) );
  }

  bool detachAll( ){
    bool ret = false;
    GLsizei const maxCount = 32;
    GLsizei count;
    GLuint shaders[32];
    glGetAttachedShaders( _object, maxCount, &count, shaders );
    for(int i = 0; i < count; i++ ){
      if( !(detach( shaders[i] )) ){
        ret = true;
      }
    }
    return( ret );
  }

  bool detach( GLuint shaderName ){
    glDetachShader( _object, shaderName );
    return( !msglError( ) );
  }
  bool detach( FragmentShader &fs ){
    glDetachShader( _object, fs.object( ) );
    return( !msglError( ) );
  }

  bool detach( VertexShader &vs ){
    glDetachShader( _object, vs.object( ) );
    return( !msglError( ) );
  }

  bool link( ){
    GLint linked_ok;
    char *msg;
    bool ret = true;
  
    glLinkProgram( _object );

    glGetProgramiv( _object, GL_LINK_STATUS, &linked_ok );
    if( !linked_ok ){
      ret = false;
      fprintf( stderr, "Linking failed.\n" );
      msg = getInfoLog( );
      fprintf( stderr, "%s\n", msg );
      free(msg );
    }
    return ret;
  }
  
  char* getInfoLog( ){
    GLint info_log_length;
    glGetProgramiv( _object, GL_INFO_LOG_LENGTH, &info_log_length );
    char *info_log = (char*)malloc( info_log_length );
    glGetProgramInfoLog( _object, info_log_length, NULL, info_log);
    return( info_log );
  }
  
  bool activate( ){
    glUseProgram( _object );
    return( !msglError( ) );
  }
  
  bool deactivate( ){
    glUseProgram( NULL );
    return( !msglError( ) );
  }
  
  bool isHardwareAccelerated( ){
    bool ret = true;
     #if defined(__APPLE__)
     /*
      * This checks to make sure that the shaders are going to be hardware accelerated.
      * If they aren't, a message is printed.
      *
      * Something similar can be done on Unix as well as MS Windows but it is left up to
      * the student to look up the neccessary API calls.
      */
    GLint hardwareAccelerated;

    CGLGetParameter( CGLGetCurrentContext( ), kCGLCPGPUVertexProcessing, &hardwareAccelerated );
    if( !hardwareAccelerated ){
      ret = false;
      fprintf( stderr,
        "Warning: Vertex shader is NOT being hardware-accelerated\n" );
    }
    CGLGetParameter( CGLGetCurrentContext( ), kCGLCPGPUFragmentProcessing, &hardwareAccelerated );
    if( !hardwareAccelerated ){
      ret = false;
      fprintf( stderr,
        "Warning: Fragment shader is NOT being hardware-accelerated\n" );
    }
     #endif
    return ret;
  }
};
