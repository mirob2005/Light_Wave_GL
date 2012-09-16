#include "GLee/GLee.h"

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

void drawScene(int scene, GLfloat *object1Position, GLfloat *object2Position)
{
	if(scene==0)
	{
		GLfloat room_vertices[] = {-4,-4,-4,-4,-4,4,4,-4,4,4,-4,-4,
							  -4,4,-4,4,4,-4,4,4,4,-4,4,4,
							  -4,-4,-4,4,-4,-4,4,4,-4,-4,4,-4,
							  -4,-4,4,-4,4,4,4,4,4,4,-4,4,
							  -4,-4,-4,-4,4,-4,-4,4,4,-4,-4,4,
							  4,-4,-4,4,-4,4,4,4,4,4,4,-4};
		GLfloat room_colors[] = {1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,1,1,1,1,1,1,1,1,1,1,1,
							1,0,0,1,0,0,1,0,0,1,0,0,
							0,1,0,0,1,0,0,1,0,0,1,0};
		GLfloat room_normals[] = {0,1,0,0,1,0,0,1,0,0,1,0,
							0,-1,0,0,-1,0,0,-1,0,0,-1,0,
							0,0,1,0,0,1,0,0,1,0,0,1,
							0,0,-1,0,0,-1,0,0,-1,0,0,-1,
							1,0,0,1,0,0,1,0,0,1,0,0,
							-1,0,0,-1,0,0,-1,0,0,-1,0,0};

		GLfloat box1_vertices[] = {	-1,1,-1,-1,1,1,1,1,1,1,1,-1,
									-1,-1,-1,1,-1,-1,1,-1,1,-1,-1,1,
									1,1,1,-1,1,1,-1,-1,1,1,-1,1,
									1,1,-1,1,-1,-1,-1,-1,-1,-1,1,-1,
									-1,-1,1,-1,1,1,-1,1,-1,-1,-1,-1,
									1,1,-1,1,1,1,1,-1,1,1,-1,-1};
		GLfloat box1_colors[] = {1,1,1,1,1,1,1,1,1,1,1,1,	
								1,1,1,1,1,1,1,1,1,1,1,1,
								1,1,1,1,1,1,1,1,1,1,1,1,
								1,1,1,1,1,1,1,1,1,1,1,1,
								1,1,1,1,1,1,1,1,1,1,1,1,
								1,1,1,1,1,1,1,1,1,1,1,1};
		GLfloat box1_normals[] = {	0,1,0,0,1,0,0,1,0,0,1,0,
									0,-1,0,0,-1,0,0,-1,0,0,-1,0,
									0,0,1,0,0,1,0,0,1,0,0,1,
									0,0,-1,0,0,-1,0,0,-1,0,0,-1,
									-1,0,0,-1,0,0,-1,0,0,-1,0,0,
									1,0,0,1,0,0,1,0,0,1,0,0};

		GLfloat box2_vertices[] = {	-1,3,-1,-1,3,1,1,3,1,1,3,-1,
									-1,-1,-1,1,-1,-1,1,-1,1,-1,-1,1,
									1,3,1,-1,3,1,-1,-1,1,1,-1,1,
									1,3,-1,1,-1,-1,-1,-1,-1,-1,3,-1,
									-1,-1,1,-1,3,1,-1,3,-1,-1,-1,-1,
									1,3,-1,1,3,1,1,-1,1,1,-1,-1};
		GLfloat box2_colors[] = {1,1,1,1,1,1,1,1,1,1,1,1,	
								1,1,1,1,1,1,1,1,1,1,1,1,
								1,1,1,1,1,1,1,1,1,1,1,1,
								1,1,1,1,1,1,1,1,1,1,1,1,
								1,1,1,1,1,1,1,1,1,1,1,1,
								1,1,1,1,1,1,1,1,1,1,1,1};
		GLfloat box2_normals[] = {	0,1,0,0,1,0,0,1,0,0,1,0,
									0,-1,0,0,-1,0,0,-1,0,0,-1,0,
									0,0,1,0,0,1,0,0,1,0,0,1,
									0,0,-1,0,0,-1,0,0,-1,0,0,-1,
									-1,0,0,-1,0,0,-1,0,0,-1,0,0,
									1,0,0,1,0,0,1,0,0,1,0,0};




		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glNormalPointer(GL_FLOAT, 0, room_normals);
		glColorPointer(3, GL_FLOAT, 0, room_colors);
		glVertexPointer(3, GL_FLOAT, 0, room_vertices);
		glDrawArrays(GL_QUADS,0,24);

		glPushMatrix();
			glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
			glMatrixMode(GL_TEXTURE);
			#ifdef __APPLE__
			glActiveTexture(GL_TEXTURE7);
			#else
			glActiveTextureARB(GL_TEXTURE7);
			#endif
			glPushMatrix();
				glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
				glNormalPointer(GL_FLOAT, 0, box1_normals);
				glColorPointer(3, GL_FLOAT, 0, box1_colors);
				glVertexPointer(3, GL_FLOAT, 0, box1_vertices);
				glDrawArrays(GL_QUADS,0,24);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(object2Position[0],object2Position[1],object2Position[2]);
			glMatrixMode(GL_TEXTURE);
			#ifdef __APPLE__
			glActiveTexture(GL_TEXTURE7);
			#else
			glActiveTextureARB(GL_TEXTURE7);
			#endif
			glPushMatrix();
				glTranslatef(object2Position[0],object2Position[1],object2Position[2]);
				glNormalPointer(GL_FLOAT, 0, box2_normals);
				glColorPointer(3, GL_FLOAT, 0, box2_colors);
				glVertexPointer(3, GL_FLOAT, 0, box2_vertices);
				glDrawArrays(GL_QUADS,0,24);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	else
	{
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
			#ifdef __APPLE__
			glActiveTexture(GL_TEXTURE7);
			#else
			glActiveTextureARB(GL_TEXTURE7);
			#endif
			
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
			#ifdef __APPLE__
			glActiveTexture(GL_TEXTURE7);
			#else
			glActiveTextureARB(GL_TEXTURE7);
			#endif
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
			#ifdef __APPLE__
			glActiveTexture(GL_TEXTURE7);
			#else
			glActiveTextureARB(GL_TEXTURE7);
			#endif
			
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
			#ifdef __APPLE__
			glActiveTexture(GL_TEXTURE7);
			#else
			glActiveTextureARB(GL_TEXTURE7);
			#endif
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
}