 
#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>
#include <time.h>

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
//double gSpin = 0;
bool gShaderEnabled;

double gTime = 0;
int frames = 0;
double fps = 0;
double cameraPosZ = 4.0;
double worldRotate = 0.0;

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
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);


   gShaderEnabled = true;
}
void DrawScene()
{
	//Display lists for objects
	static GLuint wallList=0, objectList=0;

	// Draw Objects
	if(!objectList)
	{
		objectList=glGenLists(1);
		glNewList(objectList, GL_COMPILE);
		{
			glPushMatrix();
				glColor3f(1.0f, 1.0f, 1.0f);
				glTranslatef(-2.5,-2.5,-2.5);	
				glutSolidSphere(1.5f,25,25);
			glPopMatrix();
			glPushMatrix();
				glColor3f(1.0f, 1.0f, 1.0f);
				glTranslatef(2.0,-3,0);	
				glutSolidSphere(1.0f,25,25);
			glPopMatrix();
		}
		glEndList();
	}

	//Build Walls if necessary
	if(!wallList)
	{
		wallList=glGenLists(1);
		glNewList(wallList, GL_COMPILE);
		{
			glPushMatrix();
				
				
				//Left Wall
				glPushMatrix();
					glColor3f(0.65f, 0.5f, 0.29f);
					glTranslatef(-4.0,0,0);
					glScalef(0.0125f,1.0f,1.0f);
					glRotatef(90,0,0,1);		
					glutSolidCube(8.0f);
				glPopMatrix();
				//Right Wall
				glPushMatrix();
					glColor3f(0.29f, 0.65f, 0.5f);
					glTranslatef(4.0,0,0);
					glScalef(0.0125f,1.0f,1.0f);
					glRotatef(90,0,0,1);		
					glutSolidCube(8.0f);
				glPopMatrix();
				//Top Wall
				glPushMatrix();
					glColor3f(1.0f, 1.0f, 1.0f);
					glTranslatef(0,4.0,0);
					glScalef(1.0f,0.0125f,1.0f);
					glutSolidCube(8.0f);
				glPopMatrix();
				//Bottom Wall
				glPushMatrix();
					glColor3f(0.5f, 0.29f, 0.65f);
					glTranslatef(0,-4.0,0);
					glScalef(1.0f,0.0125f,1.0f);
					glutSolidCube(8.0f);
				glPopMatrix();
				//Far Wall
				glPushMatrix();
					glColor3f(1.0f, 1.0f, 1.0f);
					glTranslatef(0,0,-4.0);
					glScalef(1.0f,1.0f,0.0125f);
					glutSolidCube(8.0f);
				glPopMatrix();
				//Near Wall
				glPushMatrix();
					glColor3f(1.0f, 1.0f, 1.0f);
					glTranslatef(0,0,4.0);
					glScalef(1.0f,1.0f,0.0125f);
					glutSolidCube(8.0f);
				glPopMatrix();

			glPopMatrix();
		}
		glEndList();
	}

	//Draw the Scene
	glCallList(wallList);
	glCallList(objectList);

}


/*  Here is where the light position is reset after the modeling
 *  transformation (glRotated) is called.  This places the
 *  light at a new position in world coordinates.  The cube
 *  represents the position of the light.
 */
void display(void){
   GLfloat position[] = { 0.0, 4.0, 0.0, 0.0 };

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   /* FPS TIMER ON BOTTOM OF SCREEN */
   glPushMatrix ();
   gluLookAt (0.0, 0.0, 4.0, 
			  0.0, 0.0, 0.0, 
			  0.0, 1.0, 0.0);

   double newTime = clock();
   
   frames++;

   	if(newTime-gTime>1.0f)
	{
		fps=frames/((newTime-gTime)/CLOCKS_PER_SEC);	//update the number of frames per second
		gTime = newTime;				//set time for the start of the next count
		frames=0;					//reset fps for this second
	}
	static char fpsString[32];
	sprintf(fpsString, "%.1f", fps);

	glColor3f(1.0,1.0,1.0);
	//Print text
	glRasterPos3f(0.0f, -1.0f,3.0f);
	for(unsigned int i=0; i<strlen(fpsString); ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fpsString[i]);

   glPopMatrix();
   /********************************************************************/


   glPushMatrix();

   //(cameraPosZ-4.0) ensures that our lookAt spot moves as we move
   gluLookAt (0.0, 0.0, cameraPosZ, 
			  0.0, 0.0, (cameraPosZ-4.0), 
			  0.0, 1.0, 0.0);

	glRotatef(worldRotate,0,1,0);


   glPushMatrix ();
   
   glLightfv (GL_LIGHT0, GL_POSITION, position);

   //glTranslated (0.0, 0.0, 1.5);
   glDisable (GL_LIGHTING);
   glPushMatrix();
	   glColor3f (1.0, 0.0, 0.0);
	   //glRotated( gSpin, 0.0, 1.0, 0.0 );
	   glTranslatef(position[0],position[1],position[2]);
	   
	   glutSolidCube (0.1);
   glPopMatrix();
   glEnable (GL_LIGHTING);
   glPopMatrix ();

   DrawScene();

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
   gluPerspective(100.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void mouse(int button, int state, int x, int y){
	// POSSIBLE TO HAVE CLICK TO OUTPUT COLOR OF PIXEL CLICKED ON?


   //switch (button) {
   //   case GLUT_LEFT_BUTTON:
   //      if (state == GLUT_DOWN) {
   //        gSpin += 30.0;
   //        gSpin = fmod(gSpin, 360.0);
   //        glutPostRedisplay();
   //      }
   //      break;
   //   default:
   //      break;
   //}
}

void keyboard(unsigned char key, int x, int y){
   switch (key) {
       case 27:
	   case 'Q':
	   case 'q':
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
	   case 'R':
       case 'r':
			cameraPosZ = 4.0;	  
			worldRotate = 0.0;
	   break;
   }
   glutPostRedisplay();
}

void special( int key, int px, int py ){
  // If you need to save what key was last pressed
  // uncomment the line below
  //static int sLastKey = key;
	switch (key) {
	   case GLUT_KEY_UP:
			cameraPosZ += -0.1;	  
	   break;	
	   case GLUT_KEY_DOWN:
			cameraPosZ += 0.1;	  
	   break;	
	   case GLUT_KEY_LEFT:
			worldRotate += -2.0;	  
	   break;
	   case GLUT_KEY_RIGHT:
			worldRotate += 2.0;	  
	   break;
	}


  glutPostRedisplay( );
}

int main(int argc, char** argv){

  
  gIsVerbose = false;
  
   glutInit(&argc, argv);

   
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 800); 
   glutInitWindowPosition(800, 100);
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

   shaderInit( "diffuse.vs", "diffuse.fs" );

   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutSpecialFunc( special );
   glutKeyboardFunc(keyboard);
   glutMainLoop( );
   return( 0 );
}
