#ifndef MYPANELOPENGL_H
#define MYPANELOPENGL_H

#ifndef __APPLE__
#include "GLee/GLee.h"
#endif

#include <QGLWidget>
#include <QKeyEvent>

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



using namespace std;

class MyPanelOpenGL : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyPanelOpenGL(QWidget *parent = 0);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void keyPressEvent (QKeyEvent *qkeyevent);

    void shaderInit(const char *vsFile, const char *fsFile);
    void generateVPLs();
    void displayFPS();

private:
    bool object1Selected;
    bool object2Selected;
    bool cameraSelected;

    //Show VPL's
    bool showVPLs;

    //Set to true when the light source is moved:
    bool updateVPLs;

    //Define Resolutions
    int screenWidth;
    int screenHeight;
    int shadowRatio;
    int shadowMapWidth;
    int shadowMapHeight;

    //Globals for keeping FPS
    double gTime;
    int frames;
    int fps;

    FrameSaver g_frameSaver;
    int g_recording;

    GLSLProgram *gProgram;

    //Chose which object to move... camera, objects in scene
    GLuint option;

    /***************DEFAULTS***************/
    GLfloat defobject1Position[3];
    GLfloat defobject2Position[3];
    GLfloat defcamPosition[3];
    GLfloat defcamLookAt[3];
    GLfloat defcamUpVector[3];
    GLfloat deflightPosition[4];
    GLfloat deflightLookAt[3];
    GLfloat deflightUpVector[3];
    GLfloat deflightNormalVector[3];
    double  defworldRotate;
    /**************************************/
    /***************VARIABLE***************/
    //Object 1
    GLfloat object1Position[3];
    //Object 2
    GLfloat object2Position[3];
    //Camera Position
    GLfloat camPosition[3];
    //Camera LookAT
    GLfloat camLookAt[3];
    //Camera UpVector
    GLfloat camUpVector[3];
    //Light Position, option 0
    //Needs 4 components for glLightfv params use
    GLfloat lightPosition[4];
    //Light LookAT
    GLfloat lightLookAt[3];
    //Light UpVector
    GLfloat lightUpVector[3];
    //Light Normal/Direction
    GLfloat lightNormalVector[3];
    //Camera Rotate for now, until a better method is used
    double worldRotate;
    /**************************************/

    /*
        FOR SHADOW MAPPING
    */
    GLuint dirSMtex;
    GLuint dirFBOid;
    GLuint dirUniID;

    //QGLFramebufferObject *fbo;

    GLuint lightMatrix;


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
    static const int lightsAngle = 5;
    static const int lightsPerRay = 5;
    static const int numLights = lightsPerRay*((90/lightsAngle)*(360/lightsAngle)+1);

    static const int numINDshadows = 20;
    int randomNumber[numINDshadows];

    float pi;
    float maxDistance;

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


    
signals:
    
public slots:
    void selectObject1();
    void selectObject2();
    void selectCamera();
    void changeShader();
    void resetLight();
    void resetObjects();
    
};

#endif // MYPANELOPENGL_H
