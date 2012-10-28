#include "mypanelopengl.h"
#include <cmath>
#include "GL/glext.h"
#include "GL/glu.h"


MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QGLWidget(parent)
{
    //Select Which Object to Move
    object1Selected = true;
    object2Selected = false;
    cameraSelected = false;
    option = 0;

    //Show VPL's
    showVPLs = true;

    //Set to true when the light source is moved:
    updateVPLs = true;

    //Define Resolutions
    screenWidth = 1280;
    screenHeight = 720;
    shadowRatio = 3;
    shadowMapWidth = screenWidth*shadowRatio;
    shadowMapHeight = screenHeight*shadowRatio;

    //Globals for keeping FPS
    gTime = 0;
    frames = 0;
    fps = 0;

    g_recording = 0;

    /***************DEFAULTS***************/
    defobject1Position[0] = 2.0;
    defobject1Position[1] = -3.0;
    defobject1Position[2] = 0.0;

    defobject2Position[0] = -2.0;
    defobject2Position[1] = -3.0;
    defobject2Position[2] = -2.0;

    defcamPosition[0] = 0.0;
    defcamPosition[1] = 0.0;
    defcamPosition[2] = 4.0;

    defcamLookAt[0] = 0.0;
    defcamLookAt[1] = 0.0;
    defcamLookAt[2] = 0.0;

    defcamUpVector[0] = 0.0;
    defcamUpVector[1] = 1.0;
    defcamUpVector[2] = 0.0;

    deflightPosition[0] = 0.0;
    deflightPosition[1] = 3.9;
    deflightPosition[2] = 3.0;
    deflightPosition[3] = 0.0;

    deflightLookAt[0] = deflightPosition[0];
    deflightLookAt[1] = deflightPosition[1]-0.1;
    deflightLookAt[2] = deflightPosition[2];

    deflightUpVector[0] = 0.0;
    deflightUpVector[1] = 0.0;
    deflightUpVector[2] = -1.0;

    deflightNormalVector[0] = deflightLookAt[0] - deflightPosition[0];
    deflightNormalVector[0] = deflightLookAt[1] - deflightPosition[1];
    deflightNormalVector[0] = deflightLookAt[2] - deflightPosition[2];

    defworldRotate = 0.0;
    /**************************************/
    /***************VARIABLE***************/
    //Object 1
    object1Position[0] = defobject1Position[0];
    object1Position[1] = defobject1Position[1];
    object1Position[2] = defobject1Position[2];
    //Object 2
    object2Position[0] = defobject2Position[0];
    object2Position[1] = defobject2Position[1];
    object2Position[2] = defobject2Position[2];

    //Camera Position
    camPosition[3] = defcamPosition[0];
    camPosition[1] = defcamPosition[1];
    camPosition[2] = defcamPosition[2];

    //Camera LookAT
    camLookAt[0] = defcamLookAt[0];
    camLookAt[1] = defcamLookAt[1];
    camLookAt[2] = defcamLookAt[2];

    //Camera UpVector
    camUpVector[0] = defcamUpVector[0];
    camUpVector[1] = defcamUpVector[1];
    camUpVector[2] = defcamUpVector[2];

    //Light Position, option 0
    //Needs 4 components for glLightfv params use
    lightPosition[0] = deflightPosition[0];
    lightPosition[1] = deflightPosition[1];
    lightPosition[2] = deflightPosition[2];
    lightPosition[3] = deflightPosition[3];

    //Light LookAT
    lightLookAt[0] = deflightLookAt[0];
    lightLookAt[1] = deflightLookAt[1];
    lightLookAt[2] = deflightLookAt[2];

    //Light UpVector
    lightUpVector[0] = deflightUpVector[0];
    lightUpVector[1] = deflightUpVector[1];
    lightUpVector[2] = deflightUpVector[2];

    //Light Normal/Direction
    lightNormalVector[0] = deflightNormalVector[0];
    lightNormalVector[1] = deflightNormalVector[1];
    lightNormalVector[2] = deflightNormalVector[2];

    //Camera Rotate for now, until a better method is used
    worldRotate = defworldRotate;
    /**************************************/

    pi = 3.14159265359;
    maxDistance = 4.0;

#ifndef __APPLE__
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
}

void MyPanelOpenGL::keyPressEvent (QKeyEvent *qkeyevent) {

    switch (qkeyevent->key()){
        case Qt::Key_W:
            object1Position[1]+=0.1;
            updateGL();
        break;


    }
}

void MyPanelOpenGL::initializeGL(){
    #ifndef __APPLE__
    GLboolean err = GLeeInit();
    if( err == false){
     /* Problem: GLeeInit failed...*/
     fprintf(stderr, "Error: %s\n",GLeeGetErrorString());
     exit(1);
    }
    #endif

    //Initialize VPLs
    generateVPLs();

    for(int x = 0; x < numINDshadows; x++)
    {
        randomNumber[x] = rand() % numLights;
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

//    glGenFramebuffers(1, &dirFBOid);
//    glBindFramebuffer(GL_FRAMEBUFFER, dirFBOid);
//    glDrawBuffers(0, NULL);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);


//    //Check for errors in the FBO
//    GLenum FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//    cout << "glCheckFramebufferStatus returned: ";
//    if(FBOstatus == GL_FRAMEBUFFER_COMPLETE)
//        cout << "GL_FRAMEBUFFER_COMPLETE" << endl;
//    if(FBOstatus == GL_FRAMEBUFFER_UNSUPPORTED)
//        cout << "GL_FRAMEBUFFER_UNSUPPORTED" << endl;
//    if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
//        cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << endl;
//    if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
//        cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << endl;
//    if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
//        cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << endl;
//    if(FBOstatus == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
//        cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << endl;

    if(!showVPLs)
        shaderInit( "D:\\_Programming\\Qt\\Light_Wave_GL\\vertexShader.vs", "D:\\_Programming\\Qt\\Light_Wave_GL\\fragShader.fs" );
}
void MyPanelOpenGL::resizeGL(int w, int h){
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
void MyPanelOpenGL::paintGL(){

    //Update VPLs if updateVPLs = true (light is moved)
    if(updateVPLs)
        generateVPLs();

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

//    //i =0 Direct Shadow Map, i=1+ Indirect Lights
//    for(int i = 0; i <numINDshadows+1; i++)
//    {

//        //Render scene with camera at lightPosition and store depth into the FBO
//        glBindFramebuffer(GL_FRAMEBUFFER,dirFBOid);

//        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dirSMtex, 0, i);

//        glClear(GL_DEPTH_BUFFER_BIT);

//        //Disable color writing to the frame buffer
//        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);


//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
//        gluPerspective(125,(GLfloat) screenWidth/(GLfloat) screenHeight,0.1,20.0);

//        if(i==0)
//        {
//            glMatrixMode(GL_MODELVIEW);
//            glLoadIdentity();
//            //Using Light's position, lookAt, up vector
//            gluLookAt(lightPosition[0],lightPosition[1],lightPosition[2],lightLookAt[0],lightLookAt[1],lightLookAt[2],
//                            lightUpVector[0],lightUpVector[1],lightUpVector[2]);
//        }
//        else
//        {
//            glMatrixMode(GL_MODELVIEW);
//            glLoadIdentity();

//            float VPLpositionX,VPLpositionY,VPLpositionZ;

//            VPLpositionX = (vplDataPos[3*randomNumber[i-1]]-0.5)*maxDistance*4;
//            VPLpositionY = (vplDataPos[3*randomNumber[i-1]+1]-0.5)*maxDistance*4;
//            VPLpositionZ = (vplDataPos[3*randomNumber[i-1]+2]-0.5)*maxDistance*4;

//            //Using Light's position, lookAt, up vector
//            gluLookAt(VPLpositionX,VPLpositionY,VPLpositionZ,VPLpositionX,VPLpositionY-0.1,VPLpositionZ,
//                            lightUpVector[0],lightUpVector[1],lightUpVector[2]);
//        }

//        //Avoid self-shadow
//        glCullFace(GL_FRONT);

//        //Draw the scene
//        drawScene(object1Position, object2Position);

//        //Store modelview and projection matrices for shadows
//        glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
//        glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

//        glMatrixMode(GL_TEXTURE);
//        if(i==0)
//            glActiveTexture(GL_TEXTURE7);
//        else
//            glActiveTexture(GL_TEXTURE6);

//        //Multiply all 3 matrices into texture7
//        glLoadIdentity();
//        glLoadMatrixf(biasMatrix);
//        glMultMatrixf(projectionMatrix);
//        glMultMatrixf(modelViewMatrix);

//        if(i!=0)
//            glGetFloatv(GL_TEXTURE_MATRIX, textureMatrix[i-1]);

//        glBindFramebuffer(GL_FRAMEBUFFER, 0);

//    }
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

//    if(!showVPLs)
//    {
//        #ifdef __APPLE__
//            glUseProgramObjectARB((void*)gProgram->_object);
//        #else
//            glUseProgramObjectARB((GLhandleARB)gProgram->_object);
//        #endif
//    }

//    //Using our shaders and shadow map

//    glUniformMatrix4fv(lightMatrix, numINDshadows, GL_FALSE, (GLfloat*)textureMatrix);

//    glUniform1i(vpl_pos_shaderID,1);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_1D,vpl_pos_TexID);

//    glUniform1i(vpl_nor_shaderID,2);
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_1D,vpl_nor_TexID);

//    glUniform1i(dirUniID,7);
//    glActiveTexture(GL_TEXTURE7);
//    glBindTexture(GL_TEXTURE_2D_ARRAY,dirSMtex);

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


    drawScene(object1Position, object2Position);

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

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    displayFPS();

    if( g_recording == 1) g_frameSaver.DumpPPM(screenWidth,screenHeight);



}
void MyPanelOpenGL::selectObject1(){
    object1Selected = true;
    object2Selected = false;
    cameraSelected = false;
}
void MyPanelOpenGL::selectObject2(){
    object1Selected = false;
    object2Selected = true;
    cameraSelected = false;
}
void MyPanelOpenGL::selectCamera(){
    object1Selected = false;
    object2Selected = false;
    cameraSelected = true;
}
void MyPanelOpenGL::changeShader(){
    if(showVPLs) showVPLs = false;
    else showVPLs = true;

    updateGL();
}
void MyPanelOpenGL::resetLight(){
    //...
}
void MyPanelOpenGL::resetObjects(){
    //...
}






void MyPanelOpenGL::shaderInit( const char *vsFile, const char *fsFile ){

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


  dirUniID = glGetUniformLocation(gProgram->_object, "ShadowMap");
  vpl_pos_shaderID = glGetUniformLocation(gProgram->_object, "vplPosTex");
  vpl_nor_shaderID = glGetUniformLocation(gProgram->_object, "vplNorTex");
  lightMatrix = glGetUniformLocation(gProgram->_object, "LightTexture");
}

void MyPanelOpenGL::generateVPLs( void )
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


void MyPanelOpenGL::displayFPS()
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
    setWindowTitle((QString)fpsString);
}


