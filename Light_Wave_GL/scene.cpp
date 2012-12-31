#include "GLee/GLee.h"
#include "teapot.h"
#include "scene.h"
#include <cmath>

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

#define PI 3.14159265

GLfloat *vertices;
GLfloat *normals;
GLfloat *colors;
unsigned int faces;

void sceneInit(ObjData *obj){
	faces = obj->faceCount();

	vertices = new GLfloat[faces*9];
	normals = new GLfloat[faces*9];
	colors = new GLfloat[faces*9];

	for(unsigned int i=0; i<faces; i++){
		vertices[i*9+0] = obj->vertices[obj->faces[i][0][0]-1][0];
		vertices[i*9+1] = obj->vertices[obj->faces[i][0][0]-1][1];
		vertices[i*9+2] = obj->vertices[obj->faces[i][0][0]-1][2];
		vertices[i*9+3] = obj->vertices[obj->faces[i][1][0]-1][0];
		vertices[i*9+4] = obj->vertices[obj->faces[i][1][0]-1][1];
		vertices[i*9+5] = obj->vertices[obj->faces[i][1][0]-1][2];
		vertices[i*9+6] = obj->vertices[obj->faces[i][2][0]-1][0];
		vertices[i*9+7] = obj->vertices[obj->faces[i][2][0]-1][1];
		vertices[i*9+8] = obj->vertices[obj->faces[i][2][0]-1][2];
	
		normals[i*9+0] = obj->normals[obj->faces[i][0][2]-1][0];
		normals[i*9+1] = obj->normals[obj->faces[i][0][2]-1][1];
		normals[i*9+2] = obj->normals[obj->faces[i][0][2]-1][2];
		normals[i*9+3] = obj->normals[obj->faces[i][1][2]-1][0];
		normals[i*9+4] = obj->normals[obj->faces[i][1][2]-1][1];
		normals[i*9+5] = obj->normals[obj->faces[i][1][2]-1][2];
		normals[i*9+6] = obj->normals[obj->faces[i][2][2]-1][0];
		normals[i*9+7] = obj->normals[obj->faces[i][2][2]-1][1];
		normals[i*9+8] = obj->normals[obj->faces[i][2][2]-1][2];

		colors[i*9+0] = 1.0;
		colors[i*9+1] = 1.0;
		colors[i*9+2] = 1.0;
		colors[i*9+3] = 1.0;
		colors[i*9+4] = 1.0;
		colors[i*9+5] = 1.0;
		colors[i*9+6] = 1.0;
		colors[i*9+7] = 1.0;
		colors[i*9+8] = 1.0;
	}

	cout << "SCENE INITIALIZED" << endl << endl;
	cout << "Faces: " << faces << endl;
}
void drawWhtSquare(){
	GLfloat vertices[] = {0,0,0.62,0.62,0,0.62,0.62,0,0,0,0,0};
	GLfloat colors[] = {1,1,1,1,1,1,1,1,1,1,1,1};
	GLfloat normals[] = {0,1,0,0,1,0,0,1,0,0,1,0};

	glNormalPointer(GL_FLOAT, 0,normals);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_QUADS,0,4);
}
void drawBlkSquare(){
	GLfloat vertices[] = {0,0,0.62,0.62,0,0.62,0.62,0,0,0,0,0};
	GLfloat colors[] = {0,0,0,0,0,0,0,0,0,0,0,0};
	GLfloat normals[] = {0,1,0,0,1,0,0,1,0,0,1,0};

	glNormalPointer(GL_FLOAT, 0,normals);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_QUADS,0,4);
}
void drawBoard(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	GLfloat vertices[] = {-3,-0.01,3.5,3,-0.01,3.5,3,-0.01,-2.5,-3,-0.01,-2.5};
	GLfloat colors[] = {0.3922,0.1765,0.0706,0.3922,0.1765,0.0706,0.3922,0.1765,0.0706,0.3922,0.1765,0.0706};
	GLfloat normals[] = {0,1,0,0,1,0,0,1,0,0,1,0};

	glNormalPointer(GL_FLOAT, 0,normals);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_QUADS,0,4);



	for(unsigned int j=0; j<8;j++){
		for(unsigned int i=0; i<8;i++){
			glPushMatrix();
				glTranslatef(-2.17+0.62*j,0,-1.66+i*0.62);
				glMatrixMode(GL_TEXTURE);
				glActiveTexture(GL_TEXTURE7);
				glPushMatrix();		
					glTranslatef(-2.17+0.62*j,0,-1.66+i*0.62);
					if(j%2==0){
						if(i%2==1)drawBlkSquare();
						else drawWhtSquare();
					}
					else{
						if(i%2==0)drawBlkSquare();
						else drawWhtSquare();
					}
				glPopMatrix();
				glMatrixMode(GL_MODELVIEW);
			glPopMatrix();	
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
void drawWhtPieces(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glPushMatrix();
		glScalef(0.05,0.05,0.05);
		glMatrixMode(GL_TEXTURE);
		glActiveTexture(GL_TEXTURE7);
		glPushMatrix();		
			glScalef(0.05,0.05,0.05);
			glNormalPointer(GL_FLOAT, 0, normals);
			glColorPointer(3, GL_FLOAT, 0, colors);
			glVertexPointer(3, GL_FLOAT, 0, vertices);
			glDrawArrays(GL_TRIANGLES,0,61560*3);		
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
void drawBlkPieces(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glPushMatrix();
		glScalef(0.05,0.05,0.05);
		glMatrixMode(GL_TEXTURE);
		glActiveTexture(GL_TEXTURE7);
		glPushMatrix();		
			glScalef(0.05,0.05,0.05);
			glNormalPointer(GL_FLOAT, 0, normals);
			glColorPointer(3, GL_FLOAT, 0, colors);
			glVertexPointer(3, GL_FLOAT, 0, vertices);
			glDrawArrays(GL_TRIANGLES,61560*3,61560*3);		
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
void drawCase(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	GLfloat room_vertices[] = {//-4,-0.01,-4,-4,-0.01,4,4,-0.01,4,4,-0.01,-4,
						  //-4,4,-4,4,4,-4,4,4,4,-4,4,4,
						  3,-0.01,-2.5,3,1.0,-2.5,-3,1.0,-2.5,-3,-0.01,-2.5,
						  -3,-0.01,3.5,-3,1.0,3.5,3,1.0,3.5,3,-0.01,3.5,
						  -3,-0.01,-2.5,-3,1.0,-2.5,-3,1.0,3.5,-3,-0.01,3.5,
						  3,-0.01,-2.5,3,-0.01,3.5,3,1.0,3.5,3,1.0,-2.5};
	GLfloat room_colors[] = {//1,1,1,1,1,1,1,1,1,1,1,1,
						//1,1,1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,1,1,
						1,0,0,1,0,0,1,0,0,1,0,0,
						0,1,0,0,1,0,0,1,0,0,1,0};
	GLfloat room_normals[] = {//0,1,0,0,1,0,0,1,0,0,1,0,
						//0,-1,0,0,-1,0,0,-1,0,0,-1,0,
						0,0,1,0,0,1,0,0,1,0,0,1,
						0,0,-1,0,0,-1,0,0,-1,0,0,-1,
						1,0,0,1,0,0,1,0,0,1,0,0,
						-1,0,0,-1,0,0,-1,0,0,-1,0,0};


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glPushMatrix();		
		glNormalPointer(GL_FLOAT, 0, room_normals);
		glColorPointer(3, GL_FLOAT, 0, room_colors);
		glVertexPointer(3, GL_FLOAT, 0, room_vertices);
		glDrawArrays(GL_QUADS,0,16);	
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);



}
void drawObj(){
	drawWhtPieces();
	drawBlkPieces();
	drawBoard();
	drawCase();
}
void drawScene(GLfloat *object1Position, GLfloat *object2Position, bool box)
{
		drawRoom();
		drawFloor();
		drawTableAndChairs();
		drawAwning();
		drawTeapot(object1Position, object2Position, box);

		////Cornell Box Scene
		//if(box){
		//	GLfloat scale[] = {1,1,1};
		//	GLfloat translate[] = {0,0,0};
		//	GLfloat color[] = {1,1,1};
		//	glPushMatrix();
		//		glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
		//		glMatrixMode(GL_TEXTURE);
		//		glActiveTexture(GL_TEXTURE7);
		//		glPushMatrix();
		//			glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
		//			drawBox(scale,translate,color);
		//		glPopMatrix();
		//		glMatrixMode(GL_MODELVIEW);
		//	glPopMatrix();

		//	glPushMatrix();
		//		
		//		glTranslatef(object2Position[0],object2Position[1],object2Position[2]);
		//		glScalef(1.0,2.0,1.0);
		//		glMatrixMode(GL_TEXTURE);
		//		glActiveTexture(GL_TEXTURE7);
		//		glPushMatrix();
		//			glTranslatef(object2Position[0],object2Position[1],object2Position[2]);
		//			glScalef(1.0,2.0,1.0);
		//			drawBox(scale,translate,color);
		//		glPopMatrix();
		//		glMatrixMode(GL_MODELVIEW);
		//	glPopMatrix();
		//}
		
		////10 Teapots
		//if(!box){
		//	for(int i=0; i<5; i++){
		//		glPushMatrix();
		//		glScalef(.2,.2,.2);			
		//			glPushMatrix();
		//				glTranslatef(object1Position[0]-15.0+(i*5),object1Position[1],object1Position[2]);
		//				glMatrixMode(GL_TEXTURE);
		//				glActiveTexture(GL_TEXTURE7);
		//				glPushMatrix();
		//					glTranslatef(object1Position[0]-15.0+(i*5),object1Position[1],object1Position[2]);
		//					drawTeapot();
		//				glPopMatrix();
		//				glMatrixMode(GL_MODELVIEW);
		//			glPopMatrix();							
		//		glPopMatrix();
		//	}
		//	for(int i=0; i<5; i++){
		//		glPushMatrix();
		//		glScalef(.2,.2,.2);			
		//			glPushMatrix();
		//				glTranslatef(object1Position[0]-15.0+(i*5),object1Position[1]-5.0,object1Position[2]+5.0);
		//				glMatrixMode(GL_TEXTURE);
		//				glActiveTexture(GL_TEXTURE7);
		//				glPushMatrix();
		//					glTranslatef(object1Position[0]-15.0+(i*5),object1Position[1]-5.0,object1Position[2]+5.0);
		//					drawTeapot();
		//				glPopMatrix();
		//				glMatrixMode(GL_MODELVIEW);
		//			glPopMatrix();						
		//		glPopMatrix();
		//	}
		//}
}
void drawTeapot(GLfloat *object1Position, GLfloat *object2Position, bool box){
	//1 Teapot
	if(!box){
		glPushMatrix();
			glTranslatef(object1Position[0]-2.0,object1Position[1]+1.1,object1Position[2]);
			glScalef(.2,.2,.2);	
			glMatrixMode(GL_TEXTURE);
			glActiveTexture(GL_TEXTURE7);
			glPushMatrix();
				glTranslatef(object1Position[0]-2.0,object1Position[1]+1.1,object1Position[2]);
				glScalef(.2,.2,.2);	
				drawTeapot();
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}
void drawRoom() {	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	GLfloat room_vertices[] = {
						  //-6,-4,-4,6,-4,-4,6,4,-4,-6,4,-4,
						  //-6,-4,4,-6,4,4,6,4,4,6,-4,4,
						  -6,-4,-8,-6,4,-8,-6,4,8,-6,-4,8,
						  //6,-4,-4,6,-4,4,6,4,4,6,4,-4
							};
	GLfloat room_colors[] = {
						//1,1,1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,1,1,
						//1,0,0,1,0,0,1,0,0,1,0,0,
						//0,1,0,0,1,0,0,1,0,0,1,0
						};
	//GLfloat room_colors[] = {0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,
	//					0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,
	//					0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,
	//					0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,
	//					0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098};
	GLfloat room_normals[] = {
						//0,0,1,0,0,1,0,0,1,0,0,1,
						//0,0,-1,0,0,-1,0,0,-1,0,0,-1,
						1,0,0,1,0,0,1,0,0,1,0,0,
						//-1,0,0,-1,0,0,-1,0,0,-1,0,0
						};


	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE7);
		glPushMatrix();			
			glNormalPointer(GL_FLOAT, 0, room_normals);
			glColorPointer(3, GL_FLOAT, 0, room_colors);
			glVertexPointer(3, GL_FLOAT, 0, room_vertices);
			glDrawArrays(GL_QUADS,0,4);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
void drawTableAndChairs(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	drawTable();
	drawChairL();
	drawChairR();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
void drawFloor()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	GLfloat floor_vertices[] = {-6,-4,-8,-6,-4,8,10,-4,8,10,-4,-8};
	GLfloat floor_colors[] = {1,1,1,1,1,1,1,1,1,1,1,1};
	GLfloat floor_normals[] = {0,1,0,0,1,0,0,1,0,0,1,0};


	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE7);
		glPushMatrix();			
			glNormalPointer(GL_FLOAT, 0, floor_normals);
			glColorPointer(3, GL_FLOAT, 0, floor_colors);
			glVertexPointer(3, GL_FLOAT, 0, floor_vertices);
			glDrawArrays(GL_QUADS,0,4);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void drawBox(GLfloat *scale, GLfloat *translate, GLfloat *color) {

	GLfloat box_vertices[] = {	-1,1,-1,-1,1,1,1,1,1,1,1,-1,
							-1,-1,-1,1,-1,-1,1,-1,1,-1,-1,1,
							1,1,1,-1,1,1,-1,-1,1,1,-1,1,
							1,1,-1,1,-1,-1,-1,-1,-1,-1,1,-1,
							-1,-1,1,-1,1,1,-1,1,-1,-1,-1,-1,
							1,1,-1,1,1,1,1,-1,1,1,-1,-1};
	GLfloat box_colors[] = {1,1,1,1,1,1,1,1,1,1,1,1,	
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1};
	GLfloat box_normals[] = {	0,1,0,0,1,0,0,1,0,0,1,0,
								0,-1,0,0,-1,0,0,-1,0,0,-1,0,
								0,0,1,0,0,1,0,0,1,0,0,1,
								0,0,-1,0,0,-1,0,0,-1,0,0,-1,
								-1,0,0,-1,0,0,-1,0,0,-1,0,0,
								1,0,0,1,0,0,1,0,0,1,0,0};

	GLfloat custom_color[] = {color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],
		color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],
		color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],
		color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],
		color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],
		color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2],color[0],color[1],color[2]};

	glPushMatrix();
	glTranslatef(translate[0],translate[1],translate[2]);
	glScalef(scale[0],scale[1],scale[2]);
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE7);
		glPushMatrix();
			glTranslatef(translate[0],translate[1],translate[2]);
			glScalef(scale[0],scale[1],scale[2]);
			glNormalPointer(GL_FLOAT, 0,box_normals);
			glColorPointer(3, GL_FLOAT, 0, custom_color);
			glVertexPointer(3, GL_FLOAT, 0, box_vertices);
			glDrawArrays(GL_QUADS,0,24);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void drawTable() {
	{
		GLfloat color[] = {0.6,0.43,0.03};
		{
		GLfloat scale[] = {2.0,0.1,1.0};
		GLfloat translate[] = {0,-2,0};
		drawBox(scale, translate, color);
		}		
		{
		GLfloat scale[] = {0.1,1.0,0.1};
		GLfloat translate[] = {-1,-3,-0.75};		
		drawBox(scale, translate, color);
		}
		{
		GLfloat scale[] = {0.1,1.0,0.1};
		GLfloat translate[] = {-1,-3,0.75};		
		drawBox(scale, translate, color);
		}
		{
		GLfloat scale[] = {0.1,1.0,0.1};
		GLfloat translate[] = {1,-3,0.75};		
		drawBox(scale, translate, color);
		}
		{
		GLfloat scale[] = {0.1,1.0,0.1};
		GLfloat translate[] = {1,-3,-0.75};		
		drawBox(scale, translate, color);
		}
	}
}
void drawChairL() {
	{
		GLfloat color[] = {0.6,0.43,0.03};
		//Seat
		{
		GLfloat scale[] = {0.75,0.1,0.75};
		GLfloat translate[] = {-2.5,-3,0};
		drawBox(scale, translate, color);
		}	
		//Back
		{
		GLfloat scale[] = {0.1,0.75,0.75};
		GLfloat translate[] = {-3.15,-2.2,0};
		drawBox(scale, translate, color);
		}		
		//Legs
		{
		GLfloat scale[] = {0.1,0.5,0.1};
		GLfloat translate[] = {-2,-3.5,-0.5};		
		drawBox(scale, translate, color);
		}
		{
		GLfloat scale[] = {0.1,0.5,0.1};
		GLfloat translate[] = {-3,-3.5,-0.5};		
		drawBox(scale, translate, color);
		}
		{
		GLfloat scale[] = {0.1,0.5,0.1};
		GLfloat translate[] = {-2,-3.5,0.5};		
		drawBox(scale, translate, color);
		}
		{
		GLfloat scale[] = {0.1,0.5,0.1};
		GLfloat translate[] = {-3,-3.5,0.5};		
		drawBox(scale, translate, color);
		}
	}
}
void drawChairR() {
	
	GLfloat color[] = {0.6,0.43,0.03};
	//Seat
	{
	GLfloat scale[] = {0.75,0.1,0.75};
	GLfloat translate[] = {2.5,-3,0};
	drawBox(scale, translate, color);
	}	
	//Back
	{
	GLfloat scale[] = {0.1,0.75,0.75};
	GLfloat translate[] = {3.15,-2.2,0};
	drawBox(scale, translate, color);
	}		
	//Legs
	{
	GLfloat scale[] = {0.1,0.5,0.1};
	GLfloat translate[] = {2,-3.5,-0.5};		
	drawBox(scale, translate, color);
	}
	{
	GLfloat scale[] = {0.1,0.5,0.1};
	GLfloat translate[] = {3,-3.5,-0.5};		
	drawBox(scale, translate, color);
	}
	{
	GLfloat scale[] = {0.1,0.5,0.1};
	GLfloat translate[] = {2,-3.5,0.5};		
	drawBox(scale, translate, color);
	}
	{
	GLfloat scale[] = {0.1,0.5,0.1};
	GLfloat translate[] = {3,-3.5,0.5};		
	drawBox(scale, translate, color);
	}		
}
void drawAwning() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);	

	GLfloat color[] = {1,1,1};
	//Z-axis
	{
	GLfloat scale[] = {0.25,0.25,5.0};
	GLfloat translate[] = {2,4,0};
	drawBox(scale, translate, color);
	}
	{
	GLfloat scale[] = {0.25,0.25,5.0};
	GLfloat translate[] = {0,4,0};
	drawBox(scale, translate, color);
	}
	{
	GLfloat scale[] = {0.25,0.25,5.0};
	GLfloat translate[] = {-2,4,0};
	drawBox(scale, translate, color);
	}
	{
	GLfloat scale[] = {0.25,0.25,5.0};
	GLfloat translate[] = {-4,4,0};
	drawBox(scale, translate, color);
	}
	//X-axis
	{
	GLfloat scale[] = {5.0,0.25,0.25};
	GLfloat translate[] = {-1,4,0};
	drawBox(scale, translate, color);
	}	
	{
	GLfloat scale[] = {5.0,0.25,0.25};
	GLfloat translate[] = {-1,4,-2};
	drawBox(scale, translate, color);
	}	
	{
	GLfloat scale[] = {5.0,0.25,0.25};
	GLfloat translate[] = {-1,4,2};
	drawBox(scale, translate, color);
	}
	{
	GLfloat scale[] = {5.0,0.25,0.25};
	GLfloat translate[] = {-1,4,-4};
	drawBox(scale, translate, color);
	}	
	{
	GLfloat scale[] = {5.0,0.25,0.25};
	GLfloat translate[] = {-1,4,4};
	drawBox(scale, translate, color);
	}
	//Y-Axis
	{
	GLfloat scale[] = {0.25,4.0,0.25};
	GLfloat translate[] = {2,0,4};
	drawBox(scale, translate, color);
	}	
	{
	GLfloat scale[] = {0.25,4.0,0.25};
	GLfloat translate[] = {2,0,-4};
	drawBox(scale, translate, color);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void drawLight() { 
	float R = 1.0; // Radius of hemisphere.
	int p = 10; // Number of longitudinal slices.
	int q = 10; // Number of latitudinal slices.

	glPushMatrix();
	glColor3f(1,1,1);
	for(int j = 0; j < q; j++)
	{
      // One latitudinal triangle strip.
      glBegin(GL_TRIANGLE_STRIP);
         for(int i = 0; i <= p; i++)
		 {
            glVertex3f( R * cos( (float)(j+1)/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        R * sin( (float)(j+1)/q * PI/2.0 ),
					    R * cos( (float)(j+1)/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );
            glVertex3f( R * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                        R * sin( (float)j/q * PI/2.0 ),
					    R * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) );         
		 }
      glEnd();
   }
   glPopMatrix();


}