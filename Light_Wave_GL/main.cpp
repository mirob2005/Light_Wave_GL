 
#define _CRT_SECURE_NO_WARNINGS

#ifndef __APPLE__
#include "GLee/GLee.h"
#endif

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
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
//#include <OpenGL/OpenGL.h>
//#include <OpenGL/glext.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include "windows.h"
#endif
#include "GL/glut.h"
#include "GL/glext.h"
#include "GL/wglext.h"
#endif

#ifndef __APPLE__
// GL_ARB_framebuffer_object
PFNGLISRENDERBUFFERPROC glIsRenderbuffer;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer;
#endif

using namespace std;

#include "PPMImage.h"
using namespace msgfx;

//Define Resolutions
int screenWidth = 1280;
int screenHeight = 720;
int shadowRatio = 3;
int shadowMapWidth = screenWidth*shadowRatio;
int shadowMapHeight = screenHeight*shadowRatio;

//Globals for keeping FPS
double gTime = 0;
int frames = 0;
int fps = 0;

FrameSaver g_frameSaver;
int g_recording = 0;

char* gTextureName = "granite.ppm";
char* gTextureName2 = "wood.ppm";
GLSLProgram *gShader[3];

//Chose which object to move... camera, objects in scene
GLuint option = 0;

/***************DEFAULTS***************/
const GLfloat defobject1Position[3] = {2.0,-3.0,0.0};
const GLfloat defobject2Position[3] = {-2.0,-2.0,-2.0};
const GLfloat defcamPosition[3] = {0.0, 0.0, 4.0};
const GLfloat defcamLookAt[3] = {0.0, 0.0, 0.0};
const GLfloat defcamUpVector[3] = {0.0, 1.0, 0.0};
const GLfloat deflightPosition[4] = { 0.0, 4.0, 0.0, 0.0 };
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
GLuint dirSMtex;
GLuint dirFBOid;
GLuint dirUniID;
GLuint dirUniID2;
GLuint dirUniID3;

GLuint lightMatrix;
GLuint lightMatrix2;
GLuint lightMatrix3;

//Must Change in vertex shader and fragment shader as well
const int numINDshadows = 5;
int randomNumber[numINDshadows];

//Constants used in shaders
const int lightsAngle = 30;
const int lightsPerRay = 5;
const int numLights = lightsPerRay*((90/lightsAngle)*(360/lightsAngle)+1);
const float pi = 3.14159265359;
float maxDistance = 4.0;

/*
	FOR VPL's
*/

// For VPL Position Texture
GLuint vpl_pos_TexID;
GLuint vpl_pos_shaderID;
GLuint vpl_pos_shaderID2;
GLuint vpl_pos_shaderID3;
GLfloat *vplDataPos;

// For VPL Normal Texture
GLuint vpl_nor_TexID;
GLuint vpl_nor_shaderID;
GLuint vpl_nor_shaderID2;
GLuint vpl_nor_shaderID3;
GLfloat *vplDataNor;

//Set to true when the light source is moved:
bool updateVPLs = true;

//Set to true when the light source or an object is moved:
bool updateShadowMaps = true;

//Show VPL's
bool showVPLs = false;

//Show boxes or teapot?
bool box = false;

//First Display?
bool initial = true;


//Textures
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte textureImage[checkImageHeight][checkImageWidth][4];
static GLubyte textureImage2[checkImageHeight][checkImageWidth][4];

PPMImage *img[2];
static GLuint texName[2];
GLuint texture1;
GLuint texture2;

void ppmimageToBytes( PPMImage* img, GLubyte buffer[64][64][4] ){
  unsigned int i, j;
  for (i = 0; i < img->getWidth( ); i++) {
     for (j = 0; j < img->getHeight( ); j++) {
       Pixel p = (*img)(i, j);
       buffer[i][j][0] = p.r;
       buffer[i][j][1] = p.g;
       buffer[i][j][2] = p.b;
       buffer[i][j][3] = p.a;
     }
   }
}


void shaderInit(){

	VertexShader vertexShader( "vertexShader.vs");
	VertexShader vertexShader2( "vertexShader2.vs" );
	//VertexShader vertexShader3( "vertexShader3.vs" );
	FragmentShader fragmentShader( "fragShader.fs" );
	FragmentShader fragmentShader2( "fragShader2.fs" );
	//FragmentShader fragmentShader3( "fragShader3.fs" );

	for(int i=0; i<3; i++){  
		gShader[i] = new GLSLProgram( );
	}

	if( !(gShader[0]->attach( vertexShader )) ){
		fprintf( stderr, "Couldn't attach the vertex shader to the program\n" );
	}
	if( !(gShader[0]->attach( fragmentShader )) ){
		fprintf( stderr, "Couldn't attach the fragment shader to the program\n" );
	}
	if( !(gShader[1]->attach( vertexShader2 )) ){
		fprintf( stderr, "Couldn't attach the 2nd vertex shader to the program\n" );
	}
	if( !(gShader[1]->attach( fragmentShader2 )) ){
		fprintf( stderr, "Couldn't attach the 2nd fragment shader to the program\n" );
	}
	if( !(gShader[2]->attach( vertexShader2 )) ){
		fprintf( stderr, "Couldn't attach the 3rd vertex shader to the program\n" );
	}
	if( !(gShader[2]->attach( fragmentShader2 )) ){
		fprintf( stderr, "Couldn't attach the 3rd fragment shader to the program\n" );
	}
	for(int i=0; i<3; i++){
		if( !(gShader[i]->link( )) ){
			fprintf( stderr, "Couldn't link the shader.\n" );
		}

		if( !(gShader[i]->activate( )) ){
			fprintf( stderr, "Unable to activate the shader.\n" );
		}

		gShader[i]->isHardwareAccelerated( );
	}
	dirUniID = glGetUniformLocation(gShader[0]->_object, "ShadowMap");
	vpl_pos_shaderID = glGetUniformLocation(gShader[0]->_object, "vplPosTex");
	vpl_nor_shaderID = glGetUniformLocation(gShader[0]->_object, "vplNorTex");
	lightMatrix = glGetUniformLocation(gShader[0]->_object, "LightTexture");

	dirUniID2 = glGetUniformLocation(gShader[1]->_object, "ShadowMap");
	vpl_pos_shaderID2 = glGetUniformLocation(gShader[1]->_object, "vplPosTex");
	vpl_nor_shaderID2 = glGetUniformLocation(gShader[1]->_object, "vplNorTex");
	lightMatrix2 = glGetUniformLocation(gShader[1]->_object, "LightTexture");
	
	texture1 = glGetUniformLocation(gShader[1]->_object, "tex");

	dirUniID3 = glGetUniformLocation(gShader[2]->_object, "ShadowMap");
	vpl_pos_shaderID3 = glGetUniformLocation(gShader[2]->_object, "vplPosTex");
	vpl_nor_shaderID3 = glGetUniformLocation(gShader[2]->_object, "vplNorTex");
	lightMatrix3 = glGetUniformLocation(gShader[2]->_object, "LightTexture");

	texture2 = glGetUniformLocation(gShader[2]->_object, "tex");
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

	if(initial){
		vplDataPos = new GLfloat[3*numLights];
		vplDataNor = new GLfloat[4*numLights];
		initial = false;
	}

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
	randomNumber[0] = 0;
	int randomNumberCounter = 1;

	int i = lightsPerRay;

	for(int angleY = 0; angleY < 360; angleY = angleY+lightsAngle)
	{
		for(int angleZ = lightsAngle; angleZ <= 90; angleZ = angleZ+lightsAngle)
		{
			if((angleY%90 ==0)&& (angleZ==30))
			{
				randomNumber[randomNumberCounter] = i;
				randomNumberCounter++;
			}
			
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

	for(int i=0; i<3*numLights; i++)
	{
		if(vplDataPos[i]<-3.9)
			vplDataPos[i]=-3.9;
		else if(vplDataPos[i] >3.9)
			vplDataPos[i]=3.9;
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

	cout << "Chosen VPLS for Indirect Shadows:" <<endl;
	for(int x = 0; x < numINDshadows; x++)
	{
		cout << randomNumber[x] << endl;
	}

	/*
		Shadow Map Generation
	*/
	glGenTextures(1, &dirSMtex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, dirSMtex);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	//Adding 1 to layer to account for the first one being direct shadow map
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32, shadowMapWidth, shadowMapHeight, numINDshadows+1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	glGenFramebuffers(1, &dirFBOid);
	glBindFramebuffer(GL_FRAMEBUFFER, dirFBOid);
	glDrawBuffers(0, NULL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Check for errors in the FBO
	GLenum FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	cout << "glCheckFramebufferStatus returned: ";
	if(FBOstatus == GL_FRAMEBUFFER_COMPLETE)
		cout << "GL_FRAMEBUFFER_COMPLETE" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_UNSUPPORTED)
		cout << "GL_FRAMEBUFFER_UNSUPPORTED" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << endl;
	if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
		cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << endl;


	img[0] = new PPMImage( gTextureName );
	ppmimageToBytes( img[0], textureImage );

	img[1] = new PPMImage( gTextureName2 );
	ppmimageToBytes( img[1], textureImage2 );

	glGenTextures(2, texName);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
				 GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				 GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
			  checkImageHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE,
			  textureImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
				 GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				 GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
			  checkImageHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE,
			  textureImage2);

	glBindTexture(GL_TEXTURE_2D, 0);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	
}



void displayFPS()
{
   /* FPS SHOWN ON WINDOW TITLE */
   double newTime = clock();

   	if(newTime-gTime>1000)
	{
		fps=frames;
		gTime = newTime;			//set time for the start of the next count
		frames=0;					//reset frame count for this second
	}
	else
	{
		frames++;
	}
	static char fpsString[64];
	sprintf(fpsString, "Light_Wave_GL  -  FPS:%i",fps);

	//Print text
	glutSetWindowTitle(fpsString);
}

void display(void){

	//Update VPLs if updateVPLs = true (light is moved)
	if(updateVPLs)
		generateVPLs();

	
	//Update image if scene is modified
	if(updateShadowMaps){

		GLfloat modelViewMatrix[16];
		GLfloat projectionMatrix[16];
		GLfloat textureMatrix[numINDshadows][16];

		//Matrix to map [-1, 1] to [0, 1] for each of X, Y and Z coordinates
		const GLfloat biasMatrix[16] = {	0.5, 0.0, 0.0, 0.0, 
											0.0, 0.5, 0.0, 0.0,
											0.0, 0.0, 0.5, 0.0,
											0.5, 0.5, 0.5, 1.0};		

		/*
			Begin Shadow Map Creation
		*/
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f,0.0f,0.0f,1.0f);

		glEnable(GL_CULL_FACE);
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

		//Use fixed function pipline to render shadow map
		glUseProgramObjectARB(0);

		glViewport(0,0,shadowMapWidth,shadowMapHeight);

		//i =0 Direct Shadow Map, i=1+ Indirect Lights
		for(int i = 0; i <numINDshadows+1; i++) 
		{

			//Render scene with camera at lightPosition and store depth into the FBO
			glBindFramebuffer(GL_FRAMEBUFFER,dirFBOid);	

			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dirSMtex, 0, i);

			glClear(GL_DEPTH_BUFFER_BIT);

			//Disable color writing to the frame buffer 
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);


			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(125,(GLfloat) screenWidth/(GLfloat) screenHeight,0.1,20.0);

			if(i==0)
			{
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				//Using Light's position, lookAt, up vector
				gluLookAt(lightPosition[0],lightPosition[1],lightPosition[2],lightLookAt[0],lightLookAt[1],lightLookAt[2],
								lightUpVector[0],lightUpVector[1],lightUpVector[2]);
			}
			else
			{
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

				float VPLpositionX,VPLpositionY,VPLpositionZ;
				float VPLnormalX,VPLnormalY,VPLnormalZ;

				VPLpositionX = (vplDataPos[3*randomNumber[i-1]]-0.5)*maxDistance*4;
				VPLpositionY = (vplDataPos[3*randomNumber[i-1]+1]-0.5)*maxDistance*4;
				VPLpositionZ = (vplDataPos[3*randomNumber[i-1]+2]-0.5)*maxDistance*4;

				VPLnormalX = (vplDataNor[4*randomNumber[i-1]]-0.5)*maxDistance*4;
				VPLnormalY = (vplDataNor[4*randomNumber[i-1]+1]-0.5)*maxDistance*4;
				VPLnormalZ = (vplDataNor[4*randomNumber[i-1]+2]-0.5)*maxDistance*4;

				float yAxis[3] = {0.0,1.0,0.0};
				float normal[3] = {VPLnormalX,VPLnormalY,VPLnormalZ};
				float rightVector[3];
				float upVector[3];
				crossProduct(normal,yAxis,rightVector);
				crossProduct(rightVector,normal,upVector);


				//Using Light's position, lookAt, up vector
				gluLookAt(VPLpositionX,VPLpositionY,VPLpositionZ,upVector[0],upVector[1],upVector[2],
								lightUpVector[0],lightUpVector[1],lightUpVector[2]);
			}

			//Avoid self-shadow
			glCullFace(GL_FRONT);

			//Draw the scene
			drawScene(object1Position, object2Position, box);

			//Store modelview and projection matrices for shadows
			glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
			glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

			glMatrixMode(GL_TEXTURE);
			if(i==0)
				glActiveTexture(GL_TEXTURE7);
			else
				glActiveTexture(GL_TEXTURE6);

			//Multiply all 3 matrices into texture7
			glLoadIdentity();	
			glLoadMatrixf(biasMatrix);
			glMultMatrixf(projectionMatrix);
			glMultMatrixf(modelViewMatrix);

			if(i!=0)
				glGetFloatv(GL_TEXTURE_MATRIX, textureMatrix[i-1]);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}
	
		/*
		Intializing Texture 5 to be used to pass in primary light properties
		*/

		//Light Position, Light Normal, light_wave properties, Camera Position
		const GLfloat light_Matrix[16] = {	lightPosition[0], lightNormalVector[0], lightsAngle, camPosition[0], 
											lightPosition[1], lightNormalVector[1], lightsPerRay, camPosition[1],
											lightPosition[2], lightNormalVector[2], numLights, camPosition[2],
											lightPosition[3], 0.0, 0.0, 0.0};

		//Use texture5 matrix
		glMatrixMode(GL_TEXTURE);	
		glActiveTexture(GL_TEXTURE5);

		//Multiply light_matrix into texture5
		glLoadIdentity();	
		glLoadMatrixf(light_Matrix);
		

		glViewport(0,0,screenWidth,screenHeight);

		//Enable color writing to the frame buffer
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		//Clear frame buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(!showVPLs)
		{
			#ifdef __APPLE__
				glUseProgramObjectARB((void*)gShader[0]->_object);
			#else
				glUseProgramObjectARB((GLhandleARB)gShader[0]->_object);		
			#endif
		}

		//Using our shaders and shadow map

		glUniformMatrix4fv(lightMatrix, numINDshadows, GL_FALSE, (GLfloat*)textureMatrix);

		glUniform1i(vpl_pos_shaderID,1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D,vpl_pos_TexID);

		glUniform1i(vpl_nor_shaderID,2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D,vpl_nor_TexID);

		glUniform1i(dirUniID,7);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D_ARRAY,dirSMtex);

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
		
		
		drawRoom(object1Position, object2Position, box);
		if(!showVPLs)
		{
			glUseProgramObjectARB((GLhandleARB)gShader[1]->_object);
		}
		glUniformMatrix4fv(lightMatrix2, numINDshadows, GL_FALSE, (GLfloat*)textureMatrix);

		glUniform1i(vpl_pos_shaderID2,1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D,vpl_pos_TexID);

		glUniform1i(vpl_nor_shaderID2,2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D,vpl_nor_TexID);

		glUniform1i(dirUniID2,7);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D_ARRAY,dirSMtex);

		glUniform1i(texture1,0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texName[0]);		
		drawFloor();


		if(!showVPLs)
		{
			glUseProgramObjectARB((GLhandleARB)gShader[2]->_object);
		}
		glUniformMatrix4fv(lightMatrix3, numINDshadows, GL_FALSE, (GLfloat*)textureMatrix);

		glUniform1i(vpl_pos_shaderID3,1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D,vpl_pos_TexID);

		glUniform1i(vpl_nor_shaderID3,2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D,vpl_nor_TexID);

		glUniform1i(dirUniID3,7);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D_ARRAY,dirSMtex);

		glUniform1i(texture2,3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D,texName[1]);		
		drawTableAndChairs();

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
					glColor3f(0.0f,0.0f,1.0f);					
					glTranslatef(vplDataPos[i*3+0],vplDataPos[i*3+1],vplDataPos[i*3+2]);
					glDrawArrays(GL_QUADS,0,24);			
				glPopMatrix();
				glPushMatrix();
					glColor3f(0.0f,0.0f,0.0f);
					glTranslatef(vplDataNor[i*4+0],vplDataNor[i*4+1],vplDataNor[i*4+2]);					
					glDrawArrays(GL_QUADS,0,24);			
				glPopMatrix();
			}
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);


		updateShadowMaps = true;
		glFlush ();
		glutSwapBuffers();
	}

	if( g_recording == 1) g_frameSaver.DumpPPM(screenWidth,screenHeight);
	displayFPS();	
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   screenWidth = w;
   screenHeight = h;
   glLoadIdentity();
   gluPerspective(125, (GLfloat) w/(GLfloat) h, 0.1, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   updateShadowMaps = true;
}

void mouse(int button, int state, int x, int y){
	//No Implementation
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
							updateShadowMaps = true;
						}
					break;
					case 1:
						//Move Object 1 up
						if(object1Position[1] < 4.0)
						{
							object1Position[1]+=0.1;
							updateShadowMaps = true;
						}
					break;
					case 2:
						//Move Object 2 up
						if(object2Position[1] < 4.0)
						{
							object2Position[1]+=0.1;
							updateShadowMaps = true;
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
							updateShadowMaps = true;
						}
					break;
					case 1:
						//Move Object 1 down
						if(object1Position[1] > -4.0)
						{
							object1Position[1]+=-0.1;
							updateShadowMaps = true;
						}
					break;
					case 2:
						//Move Object 2 up
						if(object2Position[1] > -4.0)
						{
							object2Position[1]+=-0.1;
							updateShadowMaps = true;
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
				cout << "R/r - Reset camera defaults" << endl;
				cout << "T/t - Reset Object position defaults" << endl;
				cout << "G/g - Turn off shaders, show VPL's" << endl;
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
			   if(showVPLs) 
			   {
				   showVPLs = false;
				   updateVPLs = true;
				   updateShadowMaps = true;

			   }
			   else 
			   {
				   showVPLs = true;
				   updateVPLs = true;
				   updateShadowMaps = true;
			   }
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
							updateShadowMaps = true;
						}
					break;
					case 1:
						//Move Object 1 forward
						if(object1Position[2] >-4.0)
						{
							object1Position[2]+=-0.1;
							updateShadowMaps = true;
						}
					break;
					case 2:
						//Move Object 2 forward
						if(object2Position[2] >-4.0)
						{
							object2Position[2]+=-0.1;
							updateShadowMaps = true;
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
							updateShadowMaps = true;
						}
					break;
					case 1:
						//Move Object 1 backward
						if(object1Position[2] < 4.0)
						{
							object1Position[2]+=0.1;
							updateShadowMaps = true;
						}
					break;
					case 2:
						//Move Object 2 backward
						if(object2Position[2] < 4.0)
						{
							object2Position[2]+=0.1;
							updateShadowMaps = true;
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
					   updateShadowMaps = true;
					break;
					case 1:
						//Move Object 1 left
						if(object1Position[0] >-4.0)
						{
							object1Position[0]+=-0.1;
							updateShadowMaps = true;
						}
					break;
					case 2:
						//Move Object 2 left
						if(object2Position[0] >-4.0)
						{
							object2Position[0]+=-0.1;
							updateShadowMaps = true;
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
						updateShadowMaps = true;
					break;
					case 1:
						//Move Object 1 right
						if(object1Position[0] < 4.0)
						{
							object1Position[0]+=0.1;
							updateShadowMaps = true;
						}
					break;
					case 2:
						//Move Object 2 right
						if(object2Position[0] < 4.0)
						{
							object2Position[0]+=0.1;
							updateShadowMaps = true;
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
			   updateShadowMaps = true;
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
			   updateShadowMaps = true;
		   break;
		   case 'P':
		   case 'p':
			   if(box) box = false;
			   else box = true;
			   updateShadowMaps = true;
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
		   break;
		   case 'I':
		   case 'i':
				g_frameSaver.DumpPPM(screenWidth,screenHeight) ;
			break;
	   }
   }
   glutPostRedisplay();
}

void special( int key, int px, int py ){
	switch (key) {
	   case GLUT_KEY_UP:
			if(lightPosition[2] >-2.9)
			{
				lightPosition[2] += -0.1;	  
				lightLookAt[2] += -0.1;	 
				updateVPLs = true;
				updateShadowMaps = true;
			}
	   break;	
	   case GLUT_KEY_DOWN:
			if(lightPosition[2] <2.9)
			{
				lightPosition[2] += 0.1;	
				lightLookAt[2] += 0.1;	
				updateVPLs = true;
				updateShadowMaps = true;
			}
	   break;	
	   case GLUT_KEY_LEFT:
			if(lightPosition[0] >-2.9)
			{
				lightPosition[0] += -0.1;		  
				lightLookAt[0] += -0.1;	
				updateVPLs = true;
				updateShadowMaps = true;
			}
	   break;
	   case GLUT_KEY_RIGHT:
			if(lightPosition[0] <2.9)
			{
				lightPosition[0] += 0.1;	  
				lightLookAt[0] += 0.1;	
				updateVPLs = true;
				updateShadowMaps = true;
			}
	   break;
	}
  glutPostRedisplay( );
}

int main(int argc, char** argv){

	glutInit(&argc, argv);

	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight); 
	glutInitWindowPosition(400, 10);
	glutCreateWindow("Light_Wave_GL");

	#ifndef __APPLE__
		GLboolean err = GLeeInit();
		if( err == false){
		 /* Problem: GLeeInit failed...*/
		 fprintf(stderr, "Error: %s\n",GLeeGetErrorString());
		 exit(1);
		}

		glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbuffer");
		glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
		glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
		glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameteriv");
		glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)wglGetProcAddress("glIsFramebuffer");
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
		glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
		glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)wglGetProcAddress("glFramebufferTexture1D");
		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
		glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)wglGetProcAddress("glFramebufferTexture3D");
		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
		glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
		glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
		glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)wglGetProcAddress("glBlitFramebuffer");
		glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)wglGetProcAddress("glRenderbufferStorageMultisample");
		glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)wglGetProcAddress("glFramebufferTextureLayer");
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
		shaderInit();

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutSpecialFunc( special );
	glutKeyboardFunc(keyboard);
	//Comment Out to preserve CPU while not updating the screen
	glutIdleFunc(display);
	glutMainLoop( );
	return( 0 );
}
