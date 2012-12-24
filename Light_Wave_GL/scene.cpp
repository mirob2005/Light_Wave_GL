#include "GLee/GLee.h"
#include "teapot.h"
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

void drawBox(GLfloat *scale, GLfloat *translate, GLfloat *color);
void drawRoom(GLfloat *object1Position, GLfloat *object2Position, bool box);
void drawTable();
void drawChairR();
void drawChairL();
void drawLight();
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

	GLfloat floor_vertices[] = {-6,-4,-4,-6,-4,4,6,-4,4,6,-4,-4};
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
void drawScene(GLfloat *object1Position, GLfloat *object2Position, bool box)
{
		drawRoom(object1Position, object2Position, box);
		drawFloor();
		drawTableAndChairs();

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
void drawRoom(GLfloat *object1Position, GLfloat *object2Position, bool box) {
	drawTeapot(object1Position, object2Position, box);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	GLfloat room_vertices[] = {-6,4,-4,6,4,-4,6,4,4,-6,4,4,
						  -6,-4,-4,6,-4,-4,6,4,-4,-6,4,-4,
						  -6,-4,4,-6,4,4,6,4,4,6,-4,4,
						  -6,-4,-4,-6,4,-4,-6,4,4,-6,-4,4,
						  6,-4,-4,6,-4,4,6,4,4,6,4,-4};
	GLfloat room_colors[] = {1,1,1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,1,1,
						1,1,1,1,1,1,1,1,1,1,1,1,
						1,0,0,1,0,0,1,0,0,1,0,0,
						0,1,0,0,1,0,0,1,0,0,1,0};
	//GLfloat room_colors[] = {0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,
	//					0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,
	//					0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,
	//					0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,
	//					0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098,0.6078,0.5804,0.5098};
	GLfloat room_normals[] = {0,-1,0,0,-1,0,0,-1,0,0,-1,0,
						0,0,1,0,0,1,0,0,1,0,0,1,
						0,0,-1,0,0,-1,0,0,-1,0,0,-1,
						1,0,0,1,0,0,1,0,0,1,0,0,
						-1,0,0,-1,0,0,-1,0,0,-1,0,0};


	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE7);
		glPushMatrix();			
			glNormalPointer(GL_FLOAT, 0, room_normals);
			glColorPointer(3, GL_FLOAT, 0, room_colors);
			glVertexPointer(3, GL_FLOAT, 0, room_vertices);
			glDrawArrays(GL_QUADS,0,20);
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
	{
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