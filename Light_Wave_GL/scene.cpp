#include "GLee/GLee.h"
#include "teapot.h"

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

void drawScene(GLfloat *object1Position, GLfloat *object2Position, bool box)
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

		if(box){
			glPushMatrix();
				glTranslatef(object1Position[0],object1Position[1],object1Position[2]);
				glMatrixMode(GL_TEXTURE);
				glActiveTexture(GL_TEXTURE7);
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
				glActiveTexture(GL_TEXTURE7);
				glPushMatrix();
					glTranslatef(object2Position[0],object2Position[1],object2Position[2]);
					glNormalPointer(GL_FLOAT, 0, box2_normals);
					glColorPointer(3, GL_FLOAT, 0, box2_colors);
					glVertexPointer(3, GL_FLOAT, 0, box2_vertices);
					glDrawArrays(GL_QUADS,0,24);
				glPopMatrix();
				glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		
		if(!box){
			glPushMatrix();
				glTranslatef(object1Position[0]-2.0,object1Position[1]-1.0,object1Position[2]);
				glMatrixMode(GL_TEXTURE);
				glActiveTexture(GL_TEXTURE7);
				glPushMatrix();
					glTranslatef(object1Position[0]-2.0,object1Position[1],object1Position[2]);
					drawTeapot();
				glPopMatrix();
				glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}				
}