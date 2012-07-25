 
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

using namespace std;

//Define Resolutions
int screenWidth = 1280;
int screenHeight = 960;
int shadowMapWidth = screenWidth*2;
int shadowMapHeight = screenHeight*2;


GLSLProgram *gProgram;
bool gShaderEnabled;

//Globals for keeping FPS
double gTime = 0;
int frames = 0;
double fps = 0;

//Camera Position
GLfloat camPosition[3] = {0.0, 0.0, 4.0};

//Camera LookAT
GLfloat camLookAt[3] = {0.0, 0.0, 0.0};

//Camera UpVector
GLfloat camUpVector[3] = {0.0, 1.0, 0.0};

//Light Position
//Needs 4 components for glLightfv params use
GLfloat lightPosition[4] = { 0.0, 3.9, 0.0, 0.0 };

//Light LookAT
GLfloat lightLookAt[3] = {0.0, 0.0, 0.0};

//Light UpVector
GLfloat lightUpVector[3] = {0.0, 0.0, 1.0};

//Camera Rotate for now, until a better method is used
double worldRotate = 0.0;

//Variables for using a Frame Buffer Object (FBO) (for shadow mapping)
GLuint FBOid;
GLuint shadowMapID;
GLuint shaderID;


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

  shaderID = glGetUniformLocationARB(gProgram->_object, "ShadowMap");

}

/*  Initialize material property, light source, lighting model,
 *  and depth buffer.
 */
void init( void ){

	//Try to create shadow map
	glGenTextures(1, &shadowMapID);
	glBindTexture(GL_TEXTURE_2D, shadowMapID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Create the Frame Buffer Object
	if (GL_EXT_framebuffer_object)
	{
		//GL_EXT_framebuffer_object is supported
		glGenFramebuffersEXT(1, &FBOid);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBOid);
	}
	else cout << "GL_EXT_framebuffer_object not supported!" << endl;

	//No color buffers are written to with our current FBO (shadow map)
	glDrawBuffer(GL_NONE);

	//Attach the shadow map to the current FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, shadowMapID, 0);

	//Check for errors in the FBO
	GLenum FBOstatus;
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		cout << "Some requirements or rules for GL_FRAMEBUFFER failed, CANNOT use FBO!" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_UNSUPPORTED_EXT)
		cout << "This particular implementation is not supported by the OpenGL driver!" << endl;
	
	//Clear current binding of the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

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
	glVertex3f(4, -4,-4);
	glVertex3f( 4, 4,-4);
	glVertex3f(-4,4,-4);
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

   	if(newTime-gTime>500)
	{
		fps=frames/((newTime-gTime)/CLOCKS_PER_SEC);	//update the number of frames per second
		gTime = newTime;				//set time for the start of the next count
		frames=0;					//reset fps for this second
	}
	static char fpsString[32];
	sprintf(fpsString, "%.1f", fps);

	glColor3f(1.0,1.0,1.0);

	//Print text
	glWindowPos2i((GLuint)screenWidth/2,10);
	for(unsigned int i=0; i<strlen(fpsString); ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fpsString[i]);

   glPopMatrix();
}

void display(void){

	//Begin Shadow Map Creation
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	glEnable(GL_CULL_FACE);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	//Render scene with camera at lightPosition and store depth into the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,FBOid);	

	//Use fixed function pipline to render shadow map
	glUseProgramObjectARB(0);

	glViewport(0,0,shadowMapWidth,shadowMapHeight);
	glClear( GL_DEPTH_BUFFER_BIT);

	//Disable color writing to the frame buffer 
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(125,(GLfloat) screenWidth/(GLfloat) screenHeight,0.1,20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Using Light's position, lookAt, up vector
	gluLookAt(lightPosition[0],lightPosition[1],lightPosition[2],lightLookAt[0],lightLookAt[1],lightLookAt[2],
					lightUpVector[0],lightUpVector[1],lightUpVector[2]);

	//Avoid self-shadow
	glCullFace(GL_FRONT);

	//Draw the scene
	drawObjects();
	drawRoom();

	static GLdouble modelViewMatrix[16];
	static GLdouble projectionMatrix[16];

	//Mmatrix to map [-1, 1] to [0, 1] for each of X, Y and Z coordinates
	const GLdouble biasMatrix[16] = {	0.5, 0.0, 0.0, 0.0, 
										0.0, 0.5, 0.0, 0.0,
										0.0, 0.0, 0.5, 0.0,
										0.5, 0.5, 0.5, 1.0};

	//Store modelview and projection matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

	//Use texture7 matrix
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	
	//Multiply all 3 matrices into texture7
	glLoadIdentity();	
	glLoadMatrixd(biasMatrix);
	glMultMatrixd (projectionMatrix);
	glMultMatrixd (modelViewMatrix);
	
	glMatrixMode(GL_MODELVIEW);

	//Begin rendering from the camera's perspective using our shadow map
	//Clear current binding of the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glViewport(0,0,screenWidth,screenHeight);

	//Enable color writing to the frame buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	//Clear frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Using our shaders and shadow map
	glUseProgramObjectARB(gProgram->_object);
	glUniform1iARB(shaderID,7);
	glActiveTextureARB(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D,shadowMapID);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat lightDirection[] = {lightLookAt[0]-lightPosition[0],lightLookAt[1]-lightPosition[1],lightLookAt[2]-lightPosition[2]};
	glLightfv (GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(125,(GLfloat) screenWidth/(GLfloat) screenHeight,0.1,20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Using Camera's position, lookAt, up vector
	gluLookAt (camPosition[0], camPosition[1], camPosition[2], 
			   camLookAt[0], camLookAt[1], camLookAt[2],
			   camUpVector[0], camUpVector[1], camUpVector[2]);

	glCullFace(GL_BACK);
	glRotatef(worldRotate,0,1,0);
	drawObjects();
	drawRoom();

	glDisable (GL_LIGHTING);
	float markerx,markery,markerz;
	//LIGHT MARKER
	// Math is done to ensure light marker is is 'behind' the light
	glPushMatrix();
		glColor4f(1.0,1.0f,0,0.0f);
		if(lightPosition[0] >0)
			markerx = lightPosition[0]+0.1;
		else
			markerx = lightPosition[0]-0.1;
		if(lightPosition[1] >0)
			markery = lightPosition[1];//+0.1;
		else
			markery = lightPosition[1];//-0.1;
		if(lightPosition[2] >0)
			markerz = lightPosition[2]+0.1;
		else
			markerz = lightPosition[2]-0.1;
		glTranslatef(markerx,markery,markerz);
		glRotatef(-90,1,0,0);
		glutSolidCone(0.1f,0.2f,25,25);
	glPopMatrix();

	glEnable(GL_LIGHTING);

	displayFPS();

	glFlush ();
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   screenWidth = w;
   screenHeight = h;
   shadowMapWidth = w*2;
   shadowMapHeight = h*2;
   glLoadIdentity();
   gluPerspective(125, (GLfloat) w/(GLfloat) h, 0.1, 20.0);
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
	if( glutGetModifiers( ) == GLUT_ACTIVE_SHIFT )
	{
		switch (key) 
		{
		   case 'W':
		   case 'w':
			   //Move Camera/focus pt up
				if(camPosition[1] < 3.5)
				{
					camPosition[1]+=0.1;
					camLookAt[1]+=0.1;
				}
		   break;
		   case 'S':
		   case 's':
			   //Move Camera/focus pt down
				if(camPosition[1] >-3.5)
				{
					camPosition[1]+=-0.1;
					camLookAt[1]+=-0.1;
				}
		   break;
		}
	}
	else
	{
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
		   case 'W':
		   case 'w':
			   //Move Camera/focus pt forward
				//Account for Near viewing distance
				if(camPosition[2] >-2.9)
				{
					camPosition[2]+=-0.1;
					camLookAt[2]+=-0.1;
				}
		   break;
		   case 'S':
		   case 's':
			   //Move Camera/focus pt backward
				if(camPosition[2] < 3.9)
				{
					camPosition[2]+=0.1;
					camLookAt[2]+=0.1;
				}
		   break;
		   case 'A':
		   case 'a':
				worldRotate+=-1.0;
		   break;
		   case 'D':
		   case 'd':
				worldRotate+=1.0;
		   break;
		   case 'R':
		   case 'r':
			   //Reset Rotations
			   worldRotate=0.0;
		   break;
		   case 'T':
		   case 't':
			   //Restore Camera Defaults
			   camLookAt[0] = 0;
			   camLookAt[1] = 0;
			   camLookAt[2] = 0;
			   camPosition[0] = 0;
			   camPosition[1] = 0;
			   camPosition[2] = 4;
		   break;
	   }
   }
   glutPostRedisplay();
}

void special( int key, int px, int py ){
  // If you need to save what key was last pressed
  // uncomment the line below
  //static int sLastKey = key;
	if( glutGetModifiers( ) == GLUT_ACTIVE_SHIFT )
	{
		switch (key) {
		   case GLUT_KEY_UP:
				if(lightPosition[1] <3.9)
				{
					lightPosition[1] += 0.1; 
					lightLookAt[1] += 0.1;
				}
		   break;	
		   case GLUT_KEY_DOWN:
				if(lightPosition[1] >-3.9)
				{
					lightPosition[1] -= 0.1;	
					lightLookAt[1] -= 0.1;
				}
		   break;
		}
	}
	else
	{
		switch (key) {
		   case GLUT_KEY_UP:
				if(lightPosition[2] >-3.0)
				{
					lightPosition[2] += -0.1;	  
					lightLookAt[2] += -0.1;	  
				}
		   break;	
		   case GLUT_KEY_DOWN:
				if(lightPosition[2] <3.0)
				{
					lightPosition[2] += 0.1;	
					lightLookAt[2] += 0.1;	
				}
		   break;	
		   case GLUT_KEY_LEFT:
				if(lightPosition[0] >-3.0)
				{
					lightPosition[0] += -0.1;		  
					lightLookAt[0] += -0.1;		  
				}
		   break;
		   case GLUT_KEY_RIGHT:
				if(lightPosition[0] <3.0)
				{
					lightPosition[0] += 0.1;	  
					lightLookAt[0] += 0.1;	  
				}
		   break;
		}
	}

  glutPostRedisplay( );
}

int main(int argc, char** argv){
  
	glutInit(&argc, argv);

	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight); 
	glutInitWindowPosition(400, 10);
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

	shaderInit( "vertexShader.vs", "fragShader.fs" );

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutSpecialFunc( special );
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutMainLoop( );
	return( 0 );
}
