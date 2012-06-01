 
#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>
#include <time.h>

#include "getopt.h"

#include "GLSLShader.h"

#ifdef __APPLE__
/* Apple's weird location of their OpenGL & GLUT implementation */
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


bool gIsVerbose;
GLSLProgram *gProgram;
double gSpin = 0;
bool gShaderEnabled;
double gTime = 0;
double angle = 0;

using namespace std;

void shaderInit( const char *vsFile, const char *fsFile ){

  VertexShader vertexShader( vsFile );
  FragmentShader fragmentShader( fsFile );

  gProgram = new GLSLProgram( );
  
  if( !(gProgram->attach( vertexShader )) ){
    fprintf( stderr, "Couldn't attach the vertex shader to the program\n" );
  }
  if( !(gProgram->attach( fragmentShader )) ){
    fprintf( stderr, "Couldn't attach the fragment shader to the program\n" );
  }
  
  if( !(gProgram->link( )) ){
    fprintf( stderr, "Couldn't link the shader.\n" );
  }

  if( !(gProgram->activate( )) ){
    fprintf( stderr, "Unable to activate the shader.\n" );
  }

  gProgram->isHardwareAccelerated( );

}

/*  Initialize material property, light source, lighting model,
 *  and depth buffer.
 */
void init( void ){
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

   gShaderEnabled = true;
}
void DrawScene(float angle)
{
	////Display lists for objects
	static GLuint wallList=0;

	//Build Walls if necessary
	if(!wallList)
	{
		wallList=glGenLists(1);
		glNewList(wallList, GL_COMPILE);
		{
			glPushMatrix();

				glScalef(1.0f, 0.05f, 1.0f);
				//Left Wall
				glPushMatrix();
					glColor3f(0.65f, 0.5f, 0.29f);
					glutSolidCube(3.0f);
				glPopMatrix();
				////Right Wall
				//glPushMatrix();
				//	glColor3f(0.0f, 0.0f, 1.0f);
				//	glutSolidCube(3.0f);
				//glPopMatrix();
				////Top Wall
				//glPushMatrix();
				//	glColor3f(0.0f, 0.0f, 1.0f);
				//	glutSolidCube(3.0f);
				//glPopMatrix();
				////Bottom Wall
				//glPushMatrix();
				//	glColor3f(0.0f, 0.0f, 1.0f);
				//	glutSolidCube(3.0f);
				//glPopMatrix();
				////Far Wall
				//glPushMatrix();
				//	glColor3f(0.0f, 0.0f, 1.0f);
				//	glutSolidCube(3.0f);
				//glPopMatrix();

			glPopMatrix();
		}
		glEndList();
	}

	//Draw the Scene
	glCallList(wallList);

}


/*  Here is where the light position is reset after the modeling
 *  transformation (glRotated) is called.  This places the
 *  light at a new position in world coordinates.  The cube
 *  represents the position of the light.
 */
void display(void){
   GLfloat position[] = { 0.0, 4.0, 1.0, 1.0 };

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
   //gluLookAt (0.0, 0.0, -15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   	gluLookAt(-2.5f, 3.5f,-2.5f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

	angle += (clock() - gTime)/10;
	gTime = clock();

   glPushMatrix ();
   glRotated( gSpin, 0.0, 1.0, 0.0 );
   glLightfv (GL_LIGHT0, GL_POSITION, position);

   glTranslated (0.0, 0.0, 1.5);
   glDisable (GL_LIGHTING);
   glPushMatrix();
	   //glColor3f (0.0, 1.0, 1.0);
	   glutWireCube (0.1);
   glPopMatrix();
   glEnable (GL_LIGHTING);
   glPopMatrix ();

   DrawScene(angle);

   glPopMatrix ();
   glFlush ();
   glutSwapBuffers();
   glutPostRedisplay();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void mouse(int button, int state, int x, int y){
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
           gSpin += 30.0;
           gSpin = fmod(gSpin, 360.0);
           glutPostRedisplay();
         }
         break;
      default:
         break;
   }
}

void keyboard(unsigned char key, int x, int y){
   switch (key) {
      case 27:
         exit(0);
         break;
       break;
       case 'G':
       case 'g':
       gShaderEnabled = !gShaderEnabled;
        if( gShaderEnabled ){
          fprintf( stderr, "Shader enabled\n" );
          gProgram->activate( );
        }else{
          fprintf( stderr, "Shader disabled\n" );
          gProgram->deactivate( );
        }
       break;
   }
   glutPostRedisplay();
}

void usage( string msg = "" ){
  cerr << msg.c_str( );
  cerr << "usage: shader [-hv] -x vertexshadersrc.vs -f fragmentshadersrc.fs" << endl;
}

int main(int argc, char** argv){
  int ch;
  string vertexShaderSrc, fragmentShaderSrc;

  gTime = clock();
  
  static struct option longopts[] = {
    { "fragmentshader", required_argument, NULL, 'f' },
    { "vertexshader", required_argument, NULL, 'x' },
    { "combo", required_argument, NULL, 'c' },
    { "verbose", no_argument, NULL, 'v' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0 }
  };
  
  gIsVerbose = false;
  
  if( argc < 3 ){
    usage( string( "You must specify the correct number of parameters.\n" ) );
    exit(1);
  }
  
   glutInit(&argc, argv);
   while( (ch = getopt_long(argc, argv, "c:x:f:vh", longopts, NULL)) != -1 ){
     switch( ch ){
       case 'c':
         /* basename for vertex and fragment shader file */
         vertexShaderSrc = string(optarg) + string(".vs");
         fragmentShaderSrc = string(optarg) + string(".fs");
         break;
       case 'x':
         /* input vertex shader file */
        vertexShaderSrc = string( optarg );
       break;
       case 'f':
         /* input fragment shader file */
        fragmentShaderSrc = string( optarg );
       break;
       case 'v':
         /* turn on verbose logging */
         gIsVerbose = true;
       break;
       case 'h':
         usage( );
         return(0);
       break;
       default:
         /* do nothing */
         fprintf( stderr, "Ignoring unknown option: %c\n", ch );
       break;
     } 
   }
   
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 800); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("GLSL Shader");
#ifdef __APPLE__
   if( supportsOpenGLVersion( 2, 0 ) ){
     fprintf( stderr, "Congrats! OpenGL Shading Language is supported.\n" );
   }else{
     fprintf( stderr, "OpenGL Shading Language not supported. Sorry.\n" );
     exit(1);
   }
   if( gProgram->isHardwareAccelerated( ) ){
     fprintf( stderr, "Oh and it's hardware accelerated!\n" );
   }
#else
   GLenum err = glewInit();
   if( GLEW_OK != err ){
     /* Problem: glewInit failed, something is seriously wrong. */
     fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
     exit(1);
   }
   if( GLEW_VERSION_2_0 ){
     fprintf( stderr, "Congrats! OpenGL Shading Language is supported.\n" );
   }else{
     fprintf( stderr, "OpenGL Shading Language not supported. Sorry.\n" );
     exit(1);
   }
#endif
   init( );

   shaderInit( vertexShaderSrc.c_str( ), fragmentShaderSrc.c_str( ) );

   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop( );
   return( 0 );
}
