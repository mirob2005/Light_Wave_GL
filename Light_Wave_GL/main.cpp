 
#define _CRT_SECURE_NO_WARNINGS

#include "GLee/GLee.h"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>
#include <time.h>

#include "GLSLShader.h"
#include "scene.h"
#include "FrameSaver.h"

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
int shadowRatio = 4;
int shadowMapWidth = screenWidth*shadowRatio;
int shadowMapHeight = screenHeight*shadowRatio;

//Globals for keeping FPS
double gTime = 0;
int frames = 0;
double fps = 0;

FrameSaver g_frameSaver;
int g_recording = 0;

/*  Scene Changer
---------------------------
	Scene #0 = Cornell Box 
	Scene #1 = Chess Scene 
*/
int gScene = 0;

GLSLProgram *gProgram;

//Chose which object to move... camera, objects in scene
GLuint option = 0;

/***************DEFAULTS***************/
const GLfloat defobject1Position[3] = {2.0,-3.0,0.0};
const GLfloat defobject2Position[3] = {-2,-3,-2};
const GLfloat defcamPosition[3] = {0.0, 0.0, 4.0};
const GLfloat defcamLookAt[3] = {0.0, 0.0, 0.0};
const GLfloat defcamUpVector[3] = {0.0, 1.0, 0.0};
const GLfloat deflightPosition[4] = { 0.0, 3.9, 0.0, 0.0 };
const GLfloat deflightLookAt[3] = {deflightPosition[0], deflightPosition[1]-0.1, deflightPosition[2]};
const GLfloat deflightUpVector[3] = {0.0, 0.0, -1.0};
const GLfloat deflightNormalVector[3] = {deflightLookAt[0] - deflightPosition[0], 
										deflightLookAt[1] - deflightPosition[1], 
										deflightLookAt[2] - deflightPosition[2]};
const double  defworldRotate = 0.0;
/**************************************/
/***************VARIABLE***************/
//Object 1
GLfloat object1Position[3] = {defobject1Position[0],defobject1Position[1],defobject1Position[2]};
//Object 2
GLfloat object2Position[3] = {defobject2Position[0],defobject2Position[1],defobject2Position[2]};

//Camera Position
GLfloat camPosition[3] = {defcamPosition[0],defcamPosition[1],defcamPosition[2]};

//Camera LookAT
GLfloat camLookAt[3] = {defcamLookAt[0],defcamLookAt[1],defcamLookAt[2]};

//Camera UpVector
GLfloat camUpVector[3] = {defcamUpVector[0],defcamUpVector[1],defcamUpVector[2]};

//Light Position, option 0
//Needs 4 components for glLightfv params use
GLfloat lightPosition[4] = { deflightPosition[0], deflightPosition[1], deflightPosition[2], deflightPosition[3]};

//Light LookAT
GLfloat lightLookAt[3] = {deflightLookAt[0],deflightLookAt[1],deflightLookAt[2]};

//Light UpVector
GLfloat lightUpVector[3] = {deflightUpVector[0],deflightUpVector[1],deflightUpVector[2]};

//Light Normal/Direction
GLfloat lightNormalVector[3] = {deflightNormalVector[0],deflightNormalVector[1],deflightNormalVector[2]};

//Camera Rotate for now, until a better method is used
double worldRotate = defworldRotate;
/**************************************/

/*
	FOR SHADOW MAPPING
*/
GLuint FBOid;
GLuint shadowMapID;
GLuint shadow_shaderID;

/*
	FOR INDIRECT SHADOW MAPPING
*/
GLuint INDFBOid;
GLuint INDshadowMapID;
GLuint INDshadow_shaderID;

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
const float pi = 3.14159265359;
float maxDistance = 4.0;

//Set to true when the light source is moved:
bool updateVPLs = true;

//Show VPL's
bool showVPLs = false;

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


  shadow_shaderID = glGetUniformLocation(gProgram->_object, "ShadowMap");
  INDshadow_shaderID = glGetUniformLocation(gProgram->_object, "INDShadowMap");
  vpl_pos_shaderID = glGetUniformLocation(gProgram->_object, "vplPosTex");
  vpl_nor_shaderID = glGetUniformLocation(gProgram->_object, "vplNorTex");

}



void generateVPLs( void )
{

	lightNormalVector[0] = lightLookAt[0] - lightPosition[0];
	lightNormalVector[1] = lightLookAt[1] - lightPosition[1];
	lightNormalVector[2] = lightLookAt[2] - lightPosition[2];

	vecNormalize(lightNormalVector);

	/*
		VPL DATA GENERATION
	*/

	/**************************************************************************************
	* Creating VPL's with center(3: x,y,z), normal(3: x,y,z), attenuation(1)
	* 6485 Lights, 1297rays *5per ray
	* 72 horizontal (360 degrees/5) * 18 vertical (90/5) + 1 (<0,-1,0> ray) = 1297 Directional Lights
	* NOTE: DOES NOT INCLUDE PRIMARY DIRECTIONAL LIGHT (INDIRECT ONES ONLY)
	**************************************************************************************/

	vplDataPos = new GLfloat[3*numLights];
	vplDataNor = new GLfloat[4*numLights];

	// First Ray (<0,-1,0> ray
	for(int i = 0; i <lightsPerRay; i++) {
		float normal[3] = {deflightNormalVector[0],deflightNormalVector[1],deflightNormalVector[2]};
		vecNormalize(normal);

		vplDataPos[i*3+0] = lightPosition[0] + normal[0]*(maxDistance - (maxDistance/pow(2.0,(i+1))));
		vplDataPos[i*3+1] = lightPosition[1] + normal[1]*(maxDistance - (maxDistance/pow(2.0,(i+1))));
		vplDataPos[i*3+2] = lightPosition[2] + normal[2]*(maxDistance - (maxDistance/pow(2.0,(i+1))));
		
		// VPL Normal
		vplDataNor[i*4+0] = normal[0];
		vplDataNor[i*4+1] = normal[1];
		vplDataNor[i*4+2] = normal[2];
		
		// VPL - Attenuating 5%, 10%, 20%, 40%, 80%
		vplDataNor[i*4+3] = 0.05*pow(2.0,i);
	}

	int i = lightsPerRay;

	for(int angleY = 0; angleY < 360; angleY = angleY+lightsAngle)
	{
		for(int angleZ = lightsAngle; angleZ <= 90; angleZ = angleZ+lightsAngle)
		{
			
			float normal[3] = {deflightNormalVector[0],deflightNormalVector[1],deflightNormalVector[2]};
			float temp0=normal[0];
			float temp1=normal[1];
			float temp2=normal[2];
			normal[0] = cos(angleZ*pi/180)*temp0-sin(angleZ*pi/180)*temp1;
			normal[1] = sin(angleZ*pi/180)*temp0+cos(angleZ*pi/180)*temp1;

			vecNormalize(normal);
			temp0=normal[0];
			temp1=normal[1];
			temp2=normal[2];

			normal[0] = cos(angleY*pi/180)*temp0+sin(angleY*pi/180)*temp2;
			normal[2] = -sin(angleY*pi/180)*temp0+cos(angleY*pi/180)*temp2;
			vecNormalize(normal);
			

			for(int counter = 0; counter < lightsPerRay; counter ++)
			{
				// VPL Position   = primary light pos+ primary light normal  *maxDistance * 1/lightsPerRay      
				vplDataPos[i*3+0] = lightPosition[0] + normal[0]*(maxDistance - (maxDistance/pow(2.0,(counter+1))));
				vplDataPos[i*3+1] = lightPosition[1] + normal[1]*(maxDistance - (maxDistance/pow(2.0,(counter+1))));
				vplDataPos[i*3+2] = lightPosition[2] + normal[2]*(maxDistance - (maxDistance/pow(2.0,(counter+1))));


				// VPL Normal
				vplDataNor[i*4+0] = normal[0];
				vplDataNor[i*4+1] = normal[1];
				vplDataNor[i*4+2] = normal[2];
				
				// VPL - Attenuating 5%, 10%, 20%, 40%, 80%
				vplDataNor[i*4+3] = 0.05*pow(2.0,counter);

				i++;
			}	
		}
	}
//for(int i =60; i<65; i++)
//{
//	cout << vplDataNor[4*i+0]<< endl << vplDataNor[4*i+1] << endl << vplDataNor[4*i+2] << endl << vplDataNor[4*i+3]<<endl<<endl;
//
//}

	if(!showVPLs)
	{
		for(int i=0; i<3*numLights; i++)
		{
			vplDataPos[i] = (vplDataPos[i]/(4*maxDistance))+0.5;
		}

		for(int i=0; i<4*numLights; i++)
		{
			vplDataNor[i] = (vplDataNor[i]/(4*maxDistance))+0.5;
		}
	}
	
	////Simulate Clamping process of the texture --DEBUG
	//for(int i=0; i<3*numLights; i++)
	//{
	//	if(vplDataPos[i]<0)
	//		vplDataPos[i]=0;
	//	else if(vplDataPos[i] >1)
	//		vplDataPos[i]=1;
	//}

	//for(int i=0; i<4*numLights; i++)
	//{
	//	if(vplDataNor[i]<0)
	//		vplDataNor[i]=0;
	//	else if(vplDataNor[i] >1)
	//		vplDataNor[i]=1;
	//}

	//for(int i=0; i<3*numLights; i++)
	//{
	//	vplDataPos[i] = (vplDataPos[i]-0.5)*maxDistance*4;
	//}

	//for(int i=0; i<4*numLights; i++)
	//{
	//	vplDataNor[i] = (vplDataNor[i]-0.5)*maxDistance*4;
	//}



	/*
		VPL Texture Generation
	*/

	//Create VPL Position Texture
	glGenTextures(1, &vpl_pos_TexID);
	glBindTexture(GL_TEXTURE_1D, vpl_pos_TexID);
	glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB, numLights, 0, GL_RGB, GL_FLOAT, &vplDataPos[0]);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	
	glBindTexture(GL_TEXTURE_1D, 0);

	//Create VPL Normal Texture
	glGenTextures(1, &vpl_nor_TexID);
	glBindTexture(GL_TEXTURE_1D, vpl_nor_TexID);
	glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, numLights, 0, GL_RGBA, GL_FLOAT, &vplDataNor[0]);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	
	glBindTexture(GL_TEXTURE_1D, 0);

	updateVPLs = false;

}

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
	//glTexParameterf( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);	

	
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
	glReadBuffer(GL_NONE);


	//Attach the shadow map to the current FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, shadowMapID,0);

	//Check for errors in the FBO
	GLenum FBOstatus;
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	cout << "glCheckFramebufferStatusEXT returned (DIRECT): ";
	if(FBOstatus == GL_FRAMEBUFFER_COMPLETE_EXT)
		cout << "GL_FRAMEBUFFER_COMPLETE_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_UNSUPPORTED_EXT)
		cout << "GL_FRAMEBUFFER_UNSUPPORTED_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT" << endl;

	
	//Clear current binding of the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);







	/*
		INDIRECT Shadow Map Generation
	*/

	//Try to create shadow map
	glGenTextures(1, &INDshadowMapID);
	glBindTexture(GL_TEXTURE_2D, INDshadowMapID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	//glTexParameterf( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);	

	
	glBindTexture(GL_TEXTURE_2D, 0);

	//Create the Frame Buffer Object
	if (GL_EXT_framebuffer_object)
	{
		//GL_EXT_framebuffer_object is supported
		glGenFramebuffersEXT(1, &INDFBOid);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, INDFBOid);
	}
	else cout << "GL_EXT_framebuffer_object not supported!" << endl;

	//No color buffers are written to with our current FBO (shadow map)
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);


	//Attach the shadow map to the current FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, INDshadowMapID,0);

	//Check for errors in the FBO
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	cout << "glCheckFramebufferStatusEXT returned (INDIRECT): ";
	if(FBOstatus == GL_FRAMEBUFFER_COMPLETE_EXT)
		cout << "GL_FRAMEBUFFER_COMPLETE_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_UNSUPPORTED_EXT)
		cout << "GL_FRAMEBUFFER_UNSUPPORTED_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT" << endl;

	
	//Clear current binding of the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	drawScene(gScene, object1Position, object2Position);

	static GLdouble modelViewMatrix[16];
	static GLdouble projectionMatrix[16];
	//static GLdouble textureMatrix[16];

	//Matrix to map [-1, 1] to [0, 1] for each of X, Y and Z coordinates
	const GLdouble biasMatrix[16] = {	0.5, 0.0, 0.0, 0.0, 
										0.0, 0.5, 0.0, 0.0,
										0.0, 0.0, 0.5, 0.0,
										0.5, 0.5, 0.5, 1.0};

	//Store modelview and projection matrices for shadows
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

	/*
	Intializing Texture 5 to be used to pass in primary light properties
	*/

	//Light Position, Light Normal, light_wave properties
	//Last Column available for additional parameters
	const GLdouble light_Matrix[16] = {	lightPosition[0], lightNormalVector[0], lightsAngle, camPosition[0], 
										lightPosition[1], lightNormalVector[1], lightsPerRay, camPosition[1],
										lightPosition[2], lightNormalVector[2], numLights, camPosition[2],
										lightPosition[3], 0.0, 0.0, 0.0};

	//Use texture5 matrix
	glMatrixMode(GL_TEXTURE);	
	glActiveTexture(GL_TEXTURE5);

	//Multiply light_matrix into texture5
	glLoadIdentity();	
	glLoadMatrixd(light_Matrix);

	//Use texture7 matrix
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE7);

	//Multiply all 3 matrices into texture7
	glLoadIdentity();	
	glLoadMatrixd(biasMatrix);
	glMultMatrixd (projectionMatrix);
	glMultMatrixd (modelViewMatrix);

	
	//glGetDoublev(GL_TEXTURE_MATRIX, textureMatrix);

	//cout << modelViewMatrix[0] << ", " <<modelViewMatrix[1] << ", " <<modelViewMatrix[2] << ", " <<modelViewMatrix[3] << endl;
	//cout << modelViewMatrix[4] << ", " <<modelViewMatrix[5] << ", " <<modelViewMatrix[6] << ", " <<modelViewMatrix[7] << endl;
	//cout << modelViewMatrix[8] << ", " <<modelViewMatrix[9] << ", " <<modelViewMatrix[10] << ", " <<modelViewMatrix[11] << endl;
	//cout << modelViewMatrix[12] << ", " <<modelViewMatrix[13] << ", " <<modelViewMatrix[14] << ", " <<modelViewMatrix[15] << endl;

	//cin.get();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	/*
		Begin INDIRECT Shadow Map Creation
	*/
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	glEnable(GL_CULL_FACE);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	//Render scene with camera at lightPosition and store depth into the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,INDFBOid);	

	//Use fixed function pipline to render shadow map
	glUseProgramObjectARB(0);
	

	glViewport(0,0,shadowMapWidth,shadowMapHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Disable color writing to the frame buffer 
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(125,(GLfloat) screenWidth/(GLfloat) screenHeight,0.1,20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float VPLpositionX,VPLpositionY,VPLpositionZ;

	VPLpositionX = (vplDataPos[0]-0.5)*maxDistance*4;
	VPLpositionY = (vplDataPos[1]-0.5)*maxDistance*4;
	VPLpositionZ = (vplDataPos[2]-0.5)*maxDistance*4;

	//Using Light's position, lookAt, up vector
	gluLookAt(VPLpositionX,VPLpositionY,VPLpositionZ,VPLpositionX,VPLpositionY-0.1,VPLpositionZ,
					lightUpVector[0],lightUpVector[1],lightUpVector[2]);

	//Avoid self-shadow
	glCullFace(GL_FRONT);

	//Draw the scene
	drawScene(gScene, object1Position, object2Position);








	glMatrixMode(GL_MODELVIEW);

	//Begin rendering from the camera's perspective using our shadow map
	//Clear current binding of the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glViewport(0,0,screenWidth,screenHeight);

	//Enable color writing to the frame buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	//Clear frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(!showVPLs)
	{
		#ifdef __APPLE__
			glUseProgramObjectARB((void*)gProgram->_object);
		#else
			glUseProgramObjectARB((GLhandleARB)gProgram->_object);		
		#endif
	}

	//Using our shaders and shadow map

	glUniform1i(vpl_pos_shaderID,1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D,vpl_pos_TexID);

	glUniform1i(vpl_nor_shaderID,2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D,vpl_nor_TexID);

	glUniform1i(shadow_shaderID,7);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D,shadowMapID);

	glUniform1i(INDshadow_shaderID,3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,INDshadowMapID);

	glShadeModel(GL_SMOOTH);

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
	

	drawScene(gScene, object1Position, object2Position);



	/*
		VPL Debug Section - Display cube at each VPL to test distribution
	*/

	GLfloat VPL_vertices[] = {	-0.05,0.05,-0.05,-0.05,0.05,0.05,0.05,0.05,0.05,0.05,0.05,-0.05,
								-0.05,-0.05,-0.05,0.05,-0.05,-0.05,0.05,-0.05,0.05,-0.05,-0.05,0.05,
								0.05,0.05,0.05,-0.05,0.05,0.05,-0.05,-0.05,0.05,0.05,-0.05,0.05,
								0.05,0.05,-0.05,0.05,-0.05,-0.05,-0.05,-0.05,-0.05,-0.05,0.05,-0.05,
								-0.05,-0.05,0.05,-0.05,0.05,0.05,-0.05,0.05,-0.05,-0.05,-0.05,-0.05,
								0.05,0.05,-0.05,0.05,0.05,0.05,0.05,-0.05,0.05,0.05,-0.05,-0.05};

	GLfloat VPL_normals[] = {	0,1,0,0,1,0,0,1,0,0,1,0,
								0,-1,0,0,-1,0,0,-1,0,0,-1,0,
								0,0,1,0,0,1,0,0,1,0,0,1,
								0,0,-1,0,0,-1,0,0,-1,0,0,-1,
								-1,0,0,-1,0,0,-1,0,0,-1,0,0,
								1,0,0,1,0,0,1,0,0,1,0,0};
			
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glNormalPointer(GL_FLOAT, 0, VPL_normals);
	glVertexPointer(3, GL_FLOAT, 0, VPL_vertices);
	
	if(showVPLs)
	{


		for(int i =0; i<numLights; i++)
		{
			glPushMatrix();
				glColor3f(1.0f,0.0f,0.0f);
				glTranslatef(vplDataPos[i*3+0],vplDataPos[i*3+1],vplDataPos[i*3+2]);
				glDrawArrays(GL_QUADS,0,24);			
			glPopMatrix();
		}
	}
/////////////////////////////////////////////////////////////////////////////


	//LIGHT MARKER
	glPushMatrix();
		glColor3f(1.0f,1.0f,0.0f);
		glTranslatef(lightPosition[0],lightPosition[1],lightPosition[2]);
		glScalef(2,2,2);
		glDrawArrays(GL_QUADS,0,24);
	glPopMatrix();

	//Light LookAT marker
	glPushMatrix();
		glColor3f(1.0f,1.0f,0.0f);
		glTranslatef(lightLookAt[0], lightLookAt[1], lightLookAt[2]);
		glScalef(2,2,2);
		glDrawArrays(GL_QUADS,0,24);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	displayFPS();

	if( g_recording == 1) g_frameSaver.DumpPPM(screenWidth,screenHeight);

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
   shadowMapWidth = w*shadowRatio;
   shadowMapHeight = h*shadowRatio;
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
				cout << "I/i - Capture screenshot" << endl;
				cout << "M/m - Enable Frame Recording" << endl;
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
		   case 'M':
		   case 'm':
				if( g_recording == 1 )
				{
					std::cout << "Frame recording disabled." << std::endl;
					g_recording = 0 ;
				}
				else
				{
					std::cout << "Frame recording enabled." << std::endl;
					g_recording = 1  ;
				}
				g_frameSaver.Toggle();
		   break ;
		   case 'I':
		   case 'i':
				g_frameSaver.DumpPPM(screenWidth,screenHeight) ;
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
					lightLookAt[2] += -0.1;	 
					updateVPLs = true;
				}
		   break;	
		   case GLUT_KEY_DOWN:
				if(lightPosition[2] <3.0)
				{
					lightPosition[2] += 0.1;	
					lightLookAt[2] += 0.1;	
					updateVPLs = true;
				}
		   break;	
		   case GLUT_KEY_LEFT:
				if(lightPosition[0] >-3.0)
				{
					lightPosition[0] += -0.1;		  
					lightLookAt[0] += -0.1;	
					updateVPLs = true;
				}
		   break;
		   case GLUT_KEY_RIGHT:
				if(lightPosition[0] <3.0)
				{
					lightPosition[0] += 0.1;	  
					lightLookAt[0] += 0.1;	
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

	#ifdef _WIN32
		GLboolean err = GLeeInit();
		if( err == false){
		 /* Problem: GLeeInit failed...*/
		 fprintf(stderr, "Error: %s\n",GLeeGetErrorString());
		 exit(1);
		}
	#endif


	init( );

	char *GL_version=(char *)glGetString(GL_VERSION);
	printf("You are using OpenGL version %s\n", GL_version);

	char *GLSL_version=(char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("You are using GLSL version %s\n", GLSL_version);

	char *GL_vendor=(char *)glGetString(GL_VENDOR);
	printf("Provided by this vendor %s\n", GL_vendor);

	char *GL_renderer=(char *)glGetString(GL_RENDERER);
	printf("Using this renderer %s\n\n", GL_renderer);


	cout << "Using these parameters:\nLight Angle = "<< lightsAngle << "\nLights Per Ray = " << lightsPerRay << 
			"\nTotal VPL's Used = " << numLights << endl;

	if(!showVPLs)
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
