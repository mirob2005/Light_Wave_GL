 
#define _CRT_SECURE_NO_WARNINGS

#include "GLee/GLee.h"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>
#include <time.h>

#include "GLSLShader.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include "windows.h"
#endif

#include "GL/glut.h"
#endif


bool gIsVerbose;
GLSLProgram *gProgram;
bool gShaderEnabled;

//Globals for keeping FPS
double gTime = 0;
int frames = 0;
double fps = 0;

//Camera Position
GLfloat camPosition[] = {0.0, 0.0, 4.0};

//Camera LookAT
GLfloat camLookAt[] = {0.0, 0.0, 0.0};

//Camera UpVector
GLfloat camUpVector[] = {0.0, 1.0, 0.0};

//Light Position
//Needs 4 components for glLightfv params use
GLfloat lightPosition[] = { 0.0, 4.0, 0.0, 0.0 };

//Light LookAT
GLfloat lightLookAt[] = {0.0, 0.0, 0.0};

//Light UpVector
GLfloat lightUpVector[] = {0.0, 1.0, 0.0};


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



void drawObjects()
{
	glPushMatrix();
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glTranslatef(-2.5,-2.5,-2.5);
		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glPushMatrix();
			glTranslatef(-2.5,-2.5,-2.5);
			glutSolidSphere(1.5f,25,25);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPushMatrix();
		glColor4f(0.0f, 0.0f, 1.0f,1.0f);
		glTranslatef(2.0,-3,0);	
		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glPushMatrix();
			glTranslatef(2.0,-3,0);	
			glutSolidSphere(1.0f,25,25);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawRoom()
{
	// Ground Wall
	glColor3f(0.5f, 0.29f, 0.65f);
	glBegin(GL_QUADS);
	glVertex3f(-4,-4,-4);
	glVertex3f(-4,-4, 4);
	glVertex3f( 4,-4, 4);
	glVertex3f( 4,-4,-4);
	glEnd();

	// Top Wall
	glColor3f(0.5f, 0.65f, 0.29f);
	glBegin(GL_QUADS);
	glVertex3f(-4,4,-4);
	glVertex3f( 4,4,-4);
	glVertex3f( 4,4, 4);
	glVertex3f(-4,4, 4);
	glEnd();

	// Far Wall
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(-4,-4,-4);
	glVertex3f(-4, 4,-4);
	glVertex3f( 4, 4,-4);
	glVertex3f( 4,-4,-4);
	glEnd();

	// Near Wall
	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(-4,-4,4);
	glVertex3f(-4, 4,4);
	glVertex3f( 4, 4,4);
	glVertex3f( 4,-4,4);
	glEnd();

	// Left Wall
	glColor3f(0.65f, 0.5f, 0.29f);
	glBegin(GL_QUADS);
	glVertex3f(-4,-4,-4);
	glVertex3f(-4,4, -4);
	glVertex3f(-4, 4, 4);
	glVertex3f(-4, -4,4);
	glEnd();

	// Right Wall
	glColor3f(0.29f, 0.65f, 0.5f);
	glBegin(GL_QUADS);
	glVertex3f(4,-4,-4);
	glVertex3f(4,-4, 4);
	glVertex3f(4, 4, 4);
	glVertex3f(4, 4,-4);
	glEnd();
}

void displayFPS()
{
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
}

void display(void){

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	displayFPS();

	glPushMatrix();

	// Set up Camera
	gluLookAt (camPosition[0], camPosition[1], camPosition[2], 
			   camLookAt[0], camLookAt[1], camLookAt[2],
			   camUpVector[0], camUpVector[1], camUpVector[2]);

	glRotatef(worldRotate,0,1,0);


	glPushMatrix ();

	GLfloat lightDirection[] = {lightLookAt[0]-lightPosition[0],lightLookAt[1]-lightPosition[1],lightLookAt[2]-lightPosition[2]};
	glLightfv (GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);

	//glTranslated (0.0, 0.0, 1.5);
	glDisable (GL_LIGHTING);
	glPushMatrix();
		glColor3f (1.0, 0.0, 0.0);
		glTranslatef(lightPosition[0],lightPosition[1],lightPosition[2]);

		glutSolidCube (0.1);
	glPopMatrix();
	glEnable (GL_LIGHTING);
	glPopMatrix ();

	drawObjects();
	drawRoom();

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
			camPosition[2] = 4.0;
			camLookAt[2] = 0.0;
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
			camPosition[2] += -0.1;	  
			camLookAt[2] += -0.1;	
	   break;	
	   case GLUT_KEY_DOWN:
			camPosition[2] += 0.1;
			camLookAt[2] += 0.1;
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
	glutCreateWindow("Light_Wave_GL");
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
	GLboolean err = GLeeInit();
	if( err == false){
	 /* Problem: GLeeInit failed...*/
	 fprintf(stderr, "Error: %s\n",GLeeGetErrorString());
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
