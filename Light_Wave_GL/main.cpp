 
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

//Globals for keeping FPS
double gTime = 0;
int frames = 0;
double fps = 0;

/*  Scene Changer
---------------------------
	Scene #0 = Cornell Box 
	Scene #1 = Chess Scene 
*/
int gScene = 0;

GLSLProgram *gProgram;

//Chose which object to move... camera, objects in scene
GLuint option = 0;

/***************VARIABLE***************/
//Object 1
GLfloat object1Position[3] = {-2.5,-2.5,-2.5};
//Object 2
GLfloat object2Position[3] = {2.0,-3.0,0.0};

//Camera Position
GLfloat camPosition[3] = {0.0, 0.0, 4.0};

//Camera LookAT
GLfloat camLookAt[3] = {0.0, 0.0, 0.0};

//Camera UpVector
GLfloat camUpVector[3] = {0.0, 1.0, 0.0};

//Light Position, option 0
//Needs 4 components for glLightfv params use
GLfloat lightPosition[4] = { 0.0, 3.9, 0.0, 0.0 };

//Light LookAT
GLfloat lightLookAt[3] = {0.0, 0.0, 0.0};

//Light UpVector
GLfloat lightUpVector[3] = {0.0, 0.0, 1.0};

//Light Normal/Direction
GLfloat lightNormalVector[3] = {lightLookAt[0] - lightPosition[0], 
								lightLookAt[1] - lightPosition[1], 
								lightLookAt[2] - lightPosition[2]};

//Camera Rotate for now, until a better method is used
double worldRotate = 0.0;
/**************************************/
/***************DEFAULTS***************/
const GLfloat defobject1Position[3] = {-2.5,-2.5,-2.5};
const GLfloat defobject2Position[3] = {2.0,-3.0,0.0};
const GLfloat defcamPosition[3] = {0.0, 0.0, 4.0};
const GLfloat defcamLookAt[3] = {0.0, 0.0, 0.0};
const GLfloat defcamUpVector[3] = {0.0, 1.0, 0.0};
const GLfloat deflightPosition[4] = { 0.0, 3.9, 0.0, 0.0 };
const GLfloat deflightLookAt[3] = {0.0, 0.0, 0.0};
const GLfloat deflightUpVector[3] = {0.0, 0.0, 1.0};
const double  defworldRotate = 0.0;
/**************************************/

/*
	FOR SHADOW MAPPING
*/
GLuint FBOid;
GLuint shadowMapID;
GLuint shadow_shaderID;

/*
	FOR VPL's
*/

// For VPL Position Texture
GLuint vpl_pos_TexID;
GLuint vpl_pos_shaderID;
GLfloat *vplDataPos;

// For VPL Normal Texture
GLuint vpl_nor_TexID;
GLuint vpl_nor_shaderID;
GLfloat *vplDataNor;

//Constants used in shaders
const int lightsAngle = 5;
const int lightsPerRay = 5;
const int numLights = lightsPerRay*((90/lightsAngle)*(360/lightsAngle)+1);

//Set to true when the light source is moved:
bool updateVPLs = true;

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

  shadow_shaderID = glGetUniformLocationARB(gProgram->_object, "ShadowMap");
  vpl_pos_shaderID = glGetUniformLocationARB(gProgram->_object, "vplPosTex");
  vpl_nor_shaderID = glGetUniformLocationARB(gProgram->_object, "vplNorTex");

}



void generateVPLs( void )
{
	float maxDistance = 8.0;

	lightNormalVector[0] = lightLookAt[0] - lightPosition[0];
	lightNormalVector[1] = lightLookAt[1] - lightPosition[1];
	lightNormalVector[2] = lightLookAt[2] - lightPosition[2];

	vecNormalize(lightNormalVector);

	/*
		VPL DATA GENERATION
	*/

	/**************************************************************************************
	* Creating VPL's with center(3: x,y,z), normal(3: x,y,z), //attenuation(1) (not yet)
	* 6485 Lights, 1297rays *5per ray
	* 72 horizontal (360 degrees/5) * 18 vertical (90/5) + 1 (<0,-1,0> ray) = 1297 Directional Lights
	* NOTE: DOES NOT INCLUDE PRIMARY DIRECTIONAL LIGHT (INDIRECT ONES ONLY)
	**************************************************************************************/

	vplDataPos = new GLfloat[3*numLights];
	vplDataNor = new GLfloat[3*numLights];

	// First Ray (<0,-1,0> ray
	for(int i = 0; i <lightsPerRay; i++) {
		// VPL Position   = primary light pos+ primary light normal  *maxDistance * 1/lightsPerRay      
		vplDataPos[i*3+0] = lightPosition[0] + (lightNormalVector[0])*maxDistance * ((i+0.001)/lightsPerRay);
		vplDataPos[i*3+1] = lightPosition[1] + (lightNormalVector[1])*maxDistance * ((i+0.001)/lightsPerRay);
		vplDataPos[i*3+2] = lightPosition[2] + (lightNormalVector[2])*maxDistance * ((i+0.001)/lightsPerRay);
		
		// VPL Normal
		vplDataNor[i*3+0] = lightNormalVector[0];
		vplDataNor[i*3+1] = lightNormalVector[1];
		vplDataNor[i*3+2] = lightNormalVector[2];
		
		// VPL - Attenuating 5%, 10%, 20%, 40%, 80%
		//Change vplDataNor to be RGBA?
	}
	
	int i = lightsPerRay;
	int interpolate = 1;

	for(int angleXZ = 0; angleXZ <360; angleXZ = angleXZ +lightsAngle) {
		for(int angleXY = 275; angleXY <=360; angleXY = angleXY +lightsAngle){
			for(int counter = 0; counter <lightsPerRay; counter++) {
				//Direction of the computed ray
				float normal[3] = {cos(angleXZ*3.1416/180)*cos(angleXY*3.1416/180),sin(angleXY*3.1416/180),-sin(angleXZ*3.1416/180)};
				
				// VPL Position   = primary light pos+ normal    * maxDistance * 1/lightsPerRay   
				vplDataPos[i*3+0] = lightPosition[0] + normal[0] * (maxDistance-(4.0/interpolate))*(((counter%lightsPerRay)+0.001)/lightsPerRay);
				vplDataPos[i*3+1] = lightPosition[1] + normal[1] * (maxDistance-(4.0/interpolate))*(((counter%lightsPerRay)+0.001)/lightsPerRay);
				vplDataPos[i*3+2] = lightPosition[2] + normal[2] * (maxDistance-(4.0/interpolate))*(((counter%lightsPerRay)+0.001)/lightsPerRay);
				
				// VPL Normal
				vecNormalize(normal);
				vplDataNor[i*3+0] = normal[0];
				vplDataNor[i*3+1] = normal[1];
				vplDataNor[i*3+2] = normal[2];
				
				// VPL - Attenuating 5%, 10%, 20%, 40%, 80%
				//Change vplDataNor to be RGBA?
				i++;
			}
			interpolate++;
		}
		interpolate = 1;
	}

	for(int i=0; i<3*numLights; i++)
	{
		vplDataPos[i] = (vplDataPos[i]/maxDistance)+0.5;
	}

	for(int i=0; i<3*numLights; i++)
	{
		vplDataNor[i] = (vplDataNor[i]/maxDistance)+0.5;
	}


	/*
		VPL Texture Generation
	*/

	//Create VPL Position Texture
	glGenTextures(1, &vpl_pos_TexID);
	glBindTexture(GL_TEXTURE_1D, vpl_pos_TexID);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB16, numLights, 0, GL_RGB, GL_FLOAT, vplDataPos);
	glBindTexture(GL_TEXTURE_1D, 0);

	//Create VPL Normal Texture
	glGenTextures(1, &vpl_nor_TexID);
	glBindTexture(GL_TEXTURE_1D, vpl_nor_TexID);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB16, numLights, 0, GL_RGB, GL_FLOAT, vplDataNor);
	glBindTexture(GL_TEXTURE_1D, 0);

	updateVPLs = false;

}

/*  Initialize material property, light source, lighting model,
 *  and depth buffer.
 */
void init( void ){

	//Initialize VPLs
	generateVPLs();

	/*
		Shadow Map Generation
	*/

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

}

void drawChessScene()
{
	
	GLUquadricObj *qobj;
	qobj = gluNewQuadric();
	//Draw Objects
	//Replace with Chess pieces
	//Object 1, option 1
	
	glPushMatrix();
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
		glTranslatef(0,-0.5,0);
		glScalef(1,2,1);
		glRotatef(90,1,0,0);
		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glPushMatrix();
			glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
			glTranslatef(0,-0.5,0);
			glScalef(1,2,1);
			glRotatef(90,1,0,0);
			glutSolidCube(0.5f);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPushMatrix();
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
		glTranslatef(0,-1.25,0);
		glScalef(2,1,2);
		glRotatef(90,1,0,0);
		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glPushMatrix();
			glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
			glTranslatef(0,-1.25,0);
			glScalef(2,1,2);
			glRotatef(90,1,0,0);
			glutSolidCube(0.5f);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPushMatrix();
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
		glTranslatef(0,0.25,0);
		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glPushMatrix();
			glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
			glTranslatef(0,0.25,0);
			glutSolidSphere(0.5,25,25);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	//Object 2, option 2
	glPushMatrix();
		glColor4f(0.0f, 0.0f, 1.0f,1.0f);
		glTranslatef(object2Position[0],object2Position[1],object2Position[2]);
		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glPushMatrix();
			glTranslatef(object2Position[0],object2Position[1],object2Position[2]);
			glutSolidSphere(0.5f,25,25);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	glPushMatrix();
	//Chess Board
	bool black = true;
	for(int row=0; row < 8; row++)
	{
		if(black)
		{
			glColor3f(0.420f, 0.180f, 0.098f);
			black = false;
		}
		else
		{
			glColor3f(0.757f,0.592f,0.443f);
			black = true;
		}
		for(int column=0; column <8; column++)
		{
			if(black)
			{
				glColor3f(0.420f, 0.180f, 0.098f);
				black = false;
			}
			else
			{
				glColor3f(0.757f,0.592f,0.443f);
				black = true;
			}
			glBegin(GL_QUADS);
			glNormal3f(0,1,0);
			glVertex3f(-4+row,-4,-4+column);
			glVertex3f(-4+row,-4,-4+column+1);
			glVertex3f(-4+row+1,-4,-4+column+1);
			glVertex3f(-4+row+1,-4,-4+column);
			glEnd();
		}
	}

	// Top Wall
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0,-1,0);
	glVertex3f(-4,4,-4);
	glVertex3f( 4,4,-4);
	glVertex3f( 4,4, 4);
	glVertex3f(-4,4, 4);
	glEnd();

	// Far Wall
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glVertex3f(-4,-4,-4);
	glVertex3f(4, -4,-4);
	glVertex3f( 4, 4,-4);
	glVertex3f(-4,4,-4);
	glEnd();

	// Near Wall
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glVertex3f(-4,-4,4);
	glVertex3f(-4, 4,4);
	glVertex3f( 4, 4,4);
	glVertex3f( 4,-4,4);
	glEnd();

	// Left Wall
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(1,0,0);
	glVertex3f(-4,-4,-4);
	glVertex3f(-4,4, -4);
	glVertex3f(-4, 4, 4);
	glVertex3f(-4, -4,4);
	glEnd();

	// Right Wall
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glVertex3f(4,-4,-4);
	glVertex3f(4,-4, 4);
	glVertex3f(4, 4, 4);
	glVertex3f(4, 4,-4);
	glEnd();

	glPopMatrix();
}

void drawCornellBox()
{
	//Draw Objects
	//Object 1, option 1
	glPushMatrix();
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glPushMatrix();
			glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
			glutSolidSphere(1.5f,25,25);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//Object 2, option 2
	glPushMatrix();
		glColor4f(0.0f, 0.0f, 1.0f,1.0f);
		glTranslatef(object2Position[0],object2Position[1],object2Position[2]);
		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glPushMatrix();
			glTranslatef(object2Position[0],object2Position[1],object2Position[2]);
			glutSolidSphere(1.0f,25,25);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//Draw Room
	glPushMatrix();
	// Ground Wall
	glColor3f(0.5f, 0.29f, 0.65f);
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glVertex3f(-4,-4,-4);
	glVertex3f(-4,-4, 4);
	glVertex3f( 4,-4, 4);
	glVertex3f( 4,-4,-4);
	glEnd();

	// Top Wall
	glColor3f(0.5f, 0.65f, 0.29f);
	glBegin(GL_QUADS);
	glNormal3f(0,-1,0);
	glVertex3f(-4,4,-4);
	glVertex3f( 4,4,-4);
	glVertex3f( 4,4, 4);
	glVertex3f(-4,4, 4);
	glEnd();

	// Far Wall
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glVertex3f(-4,-4,-4);
	glVertex3f(4, -4,-4);
	glVertex3f( 4, 4,-4);
	glVertex3f(-4,4,-4);
	glEnd();

	// Near Wall
	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glVertex3f(-4,-4,4);
	glVertex3f(-4, 4,4);
	glVertex3f( 4, 4,4);
	glVertex3f( 4,-4,4);
	glEnd();

	// Left Wall
	glColor3f(0.65f, 0.5f, 0.29f);
	glBegin(GL_QUADS);
	glNormal3f(1,0,0);
	glVertex3f(-4,-4,-4);
	glVertex3f(-4,4, -4);
	glVertex3f(-4, 4, 4);
	glVertex3f(-4, -4,4);
	glEnd();

	// Right Wall
	glColor3f(0.29f, 0.65f, 0.5f);
	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glVertex3f(4,-4,-4);
	glVertex3f(4,-4, 4);
	glVertex3f(4, 4, 4);
	glVertex3f(4, 4,-4);
	glEnd();

	glPopMatrix();
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

	//Update VPLs if updateVPLs = true (light is moved)
	if(updateVPLs)
		generateVPLs();

	/*
		Begin Shadow Map Creation
	*/
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
	if(gScene==0)
		drawCornellBox();
	else
		drawChessScene();

	static GLdouble modelViewMatrix[16];
	static GLdouble projectionMatrix[16];

	//Matrix to map [-1, 1] to [0, 1] for each of X, Y and Z coordinates
	const GLdouble biasMatrix[16] = {	0.5, 0.0, 0.0, 0.0, 
										0.0, 0.5, 0.0, 0.0,
										0.0, 0.0, 0.5, 0.0,
										0.5, 0.5, 0.5, 1.0};

	//Store modelview and projection matrices for shadows
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

/*************************************************/

	//NEX CODE HERE

	////Matrix is Blue, Green, Red, Purple... matrix is row order
	//const GLdouble color_Matrix[16] = {	1.0, 0.0, 0.0, 0.5, 
	//									0.0, 1.0, 0.0, 0.0,
	//									0.0, 0.0, 1.0, 0.5,
	//									0.0, 0.0, 0.0, 0.0};
	//
	//
	////Use texture6 matrix
	//glMatrixMode(GL_TEXTURE);
	//glActiveTextureARB(GL_TEXTURE6);
	//
	////Multiply matrix into texture6
	//glLoadIdentity();	
	//glLoadMatrixd(color_Matrix);

/*
	Intializing Texture 5 to be used to pass in primary light properties
*/

	//Light Position, Light Normal, light_wave properties (may need to be const in shader, remove?)
	//can Add lihgt color or attenuation to last column
	const GLdouble light_Matrix[16] = {	lightPosition[0], lightNormalVector[0], lightsAngle, 0.0, 
										lightPosition[1], lightNormalVector[1], lightsPerRay, 0.0,
										lightPosition[2], lightNormalVector[2], numLights, 0.0,
										lightPosition[3], 0.0, 0.0, 0.0};

	//Use texture5 matrix
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE5);
	
	//Multiply light_matrix into texture6
	glLoadIdentity();	
	glLoadMatrixd(light_Matrix);


/*************************************************/


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

	glUniform1iARB(vpl_pos_shaderID,1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D,vpl_pos_TexID);

	glUniform1iARB(vpl_nor_shaderID,2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D,vpl_nor_TexID);

	glUniform1iARB(shadow_shaderID,7);
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
	

	if(gScene==0)
		drawCornellBox();
	else
		drawChessScene();

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
		glTranslatef(lightPosition[0],lightPosition[1],lightPosition[2]);
		glutSolidSphere(0.1f,25,25);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(lightLookAt[0], lightLookAt[1], lightLookAt[2]);
		glutSolidSphere(.1f,25,25);
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
				switch(option)
				{
					case 0:
						//Move Camera/focus pt up
						if(camPosition[1] < 3.5)
						{
							camPosition[1]+=0.1;
							camLookAt[1]+=0.1;
						}
					break;
					case 1:
						//Move Object 1 up
						if(object1Position[1] < 4.0)
						{
							object1Position[1]+=0.1;
						}
					break;
					case 2:
						//Move Object 2 up
						if(object2Position[1] < 4.0)
						{
							object2Position[1]+=0.1;
						}
					break;
			   }
		   break;
		   case 'S':
		   case 's':
				switch(option)
				{
					case 0:
					   //Move Camera/focus pt down
						if(camPosition[1] >-3.5)
						{
							camPosition[1]+=-0.1;
							camLookAt[1]+=-0.1;
						}
					break;
					case 1:
						//Move Object 1 down
						if(object1Position[1] > -4.0)
						{
							object1Position[1]+=-0.1;
						}
					break;
					case 2:
						//Move Object 2 up
						if(object2Position[1] > -4.0)
						{
							object2Position[1]+=-0.1;
						}
					break;
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
			case 'H':
			case 'h':
				cout << "\nKey Bindings:" << endl;
				cout << "--------------" << endl;
				cout << "ESC, Q/q - Exit the program" << endl;
				cout << "H/h - This help menu" << endl;
				cout << "0 - Allows you to move the camera using WASD keys" << endl;
				cout << "1 - Allows you to move the first Object using WASD keys" << endl;
				cout << "2 - Allows you to move the second Object using WASD keys" << endl;
				cout << "W/w - Move the selected object forward (Default: Camera)" << endl;
				cout << "A/a - Move the selected object left (Default: Camera)" << endl;
				cout << "S/s - Move the selected object backward (Default: Camera)" << endl;
				cout << "D/d - Move the selected object right (Default: Camera)" << endl;
				cout << "shift+W/w - Move the selected object up (Default: Camera)" << endl;
				cout << "shift+W/w - Move the selected object down (Default: Camera)" << endl;
				cout << "UP arrow - Move the light forward" << endl;
				cout << "DOWN arrow - Move the light backward" << endl;
				cout << "LEFT arrow - Move the light left" << endl;
				cout << "RIGHT arrow - Move the light right" << endl;
				cout << "shift+UP arrow - Move the light up" << endl;
				cout << "shift+DOWN arrow - Move the light down" << endl;
				cout << "R/r - Reset camera defaults" << endl;
				cout << "T/t - Reset Object position defaults" << endl;
				cout << "G/g - Change Scene" << endl;
				cout << "--------------" << endl;
				cout << endl;
			break;
			case '0':
				option = 0;
				cout << "Option = " << option << ", WASD keys move the camera!" << endl;
			break;
			case '1':
				option = 1;
				cout << "Option = " << option << ", WASD keys move Object 1!" << endl;
			break;
			case '2':
			   option = 2;
			   cout << "Option = " << option << ", WASD keys move Object 2!" << endl;
		   break;
		   case 'G':
		   case 'g':
			   gScene = (gScene+1)%2;
		   break;
		   case 'W':
		   case 'w':
				switch(option)
				{
					case 0:
					   //Move Camera/focus pt forward
						//Account for Near viewing distance
						if(camPosition[2] >-2.9)
						{
							camPosition[2]+=-0.1;
							camLookAt[2]+=-0.1;
						}
					break;
					case 1:
						//Move Object 1 forward
						if(object1Position[2] >-4.0)
						{
							object1Position[2]+=-0.1;
						}
					break;
					case 2:
						//Move Object 2 forward
						if(object2Position[2] >-4.0)
						{
							object2Position[2]+=-0.1;
						}
					break;
			   }
		   break;
		   case 'S':
		   case 's':
				switch(option)
				{
					case 0:
					   //Move Camera/focus pt backward
						if(camPosition[2] < 3.9)
						{
							camPosition[2]+=0.1;
							camLookAt[2]+=0.1;
						}
					break;
					case 1:
						//Move Object 1 backward
						if(object1Position[2] < 4.0)
						{
							object1Position[2]+=0.1;
						}
					break;
					case 2:
						//Move Object 2 backward
						if(object2Position[2] < 4.0)
						{
							object2Position[2]+=0.1;
						}
					break;
			   }
		   break;
		   case 'A':
		   case 'a':
				switch(option)
				{
					case 0:
						//Rotate scene to the left
					   worldRotate+=-1.0;
					break;
					case 1:
						//Move Object 1 left
						if(object1Position[0] >-4.0)
						{
							object1Position[0]+=-0.1;
						}
					break;
					case 2:
						//Move Object 2 left
						if(object2Position[0] >-4.0)
						{
							object2Position[0]+=-0.1;
						}
					break;
			   }
		   break;
		   case 'D':
		   case 'd':
				switch(option)
				{
					case 0:
						//Rotate scene to the right
						worldRotate+=1.0;
					break;
					case 1:
						//Move Object 1 right
						if(object1Position[0] < 4.0)
						{
							object1Position[0]+=0.1;
						}
					break;
					case 2:
						//Move Object 2 right
						if(object2Position[0] < 4.0)
						{
							object2Position[0]+=0.1;
						}
					break;
			   }
		   break;
		   case 'R':
		   case 'r':
			   //Reset Rotations
			   worldRotate=defworldRotate;
			   //Restore Camera Defaults
			   camLookAt[0] = defcamLookAt[0];
			   camLookAt[1] = defcamLookAt[1];
			   camLookAt[2] = defcamLookAt[2];
			   camPosition[0] = defcamPosition[0];
			   camPosition[1] = defcamPosition[1];
			   camPosition[2] = defcamPosition[2];
		   break;
		   case 'T':
		   case 't':
				//Reset Object movements
			   object1Position[0] = defobject1Position[0];
			   object1Position[1] = defobject1Position[1];
			   object1Position[2] = defobject1Position[2];
			   object2Position[0] = defobject2Position[0];
			   object2Position[1] = defobject2Position[1];
			   object2Position[2] = defobject2Position[2];
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
					updateVPLs = true;
				}
		   break;	
		   case GLUT_KEY_DOWN:
				if(lightPosition[1] >-3.9)
				{
					lightPosition[1] -= 0.1;	
					lightLookAt[1] -= 0.1;
					updateVPLs = true;
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
					//lightLookAt[2] += -0.1;	 
					updateVPLs = true;
				}
		   break;	
		   case GLUT_KEY_DOWN:
				if(lightPosition[2] <3.0)
				{
					lightPosition[2] += 0.1;	
					//lightLookAt[2] += 0.1;	
					updateVPLs = true;
				}
		   break;	
		   case GLUT_KEY_LEFT:
				if(lightPosition[0] >-3.0)
				{
					lightPosition[0] += -0.1;		  
					//lightLookAt[0] += -0.1;	
					updateVPLs = true;
				}
		   break;
		   case GLUT_KEY_RIGHT:
				if(lightPosition[0] <3.0)
				{
					lightPosition[0] += 0.1;	  
					//lightLookAt[0] += 0.1;	
					updateVPLs = true;
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
