#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <OPENGL/gl.h>
#endif

#include <iostream>

#include "FrameSaver.h"

using namespace std;

FrameSaver::FrameSaver()
{
	SetRecording(0) ;
	m_pixels = NULL ;
	m_frameCount = 0 ;
}


void FrameSaver::Toggle(void)
{
	if( GetRecording() == 0 ) 
		StartRecord() ;
	else 
		StopRecord() ;
}

void FrameSaver::StopRecord(void)
{
		SetRecording(0) ;
}

void FrameSaver::StartRecord(void)
{
	SetRecording(1) ; 
	m_frameCount = 0 ;
}


int FrameSaver::DumpPPM(int width, int height)
{
	if( m_pixels ) delete [] m_pixels ;
	m_pixels = new unsigned char [3*width];
    if(	m_pixels == NULL )
    {
		cerr << "Cannot allocate enough memory" << endl;
		return  -1;
    }

	char fname[132] ;
	char baseName[132] ;
	if( GetRecording() == 0 ) // one time save
	{
		strcpy(fname, "scene.ppm") ;
		strcpy(baseName,"scene") ;
	}
	else				// animation recording
	{
		sprintf(fname,"frame%d.ppm", m_frameCount) ;
		sprintf(baseName,"frame%d", m_frameCount) ;
		m_frameCount++ ;
	}
	FILE *fp = fopen(fname,"wb") ;
	if( fp == NULL ) 
	{
		cerr << "Cannot open file " << fname << endl;

		return -1 ;
	}
	DumpPPM(fp,width,height) ;
	fclose(fp) ;
	ConvertToPng(baseName) ;
	return 1 ;
}
void FrameSaver::ConvertToPng(char *baseName)
{
	char com[256] ;
	sprintf(com,"convert %s.ppm %s.png\n", baseName, baseName) ;
	system(com) ;
	sprintf(com,"/bin/rm -f %s.ppm\n",  baseName) ;
	system(com) ;
}

// dumps a PPM raw (P6) file on an already allocated memory array
void FrameSaver::DumpPPM(FILE *fp, int width, int height)
{
    const int maxVal=255;
    register int y;
    
    fprintf(fp,	"P6 ");
    fprintf(fp,	"%d %d ", width, height);
    fprintf(fp,	"%d\n",	maxVal);

   	glReadBuffer(GL_FRONT) ; 
	for	( y = height-1;	y>=0; y-- ) 
	{
		glReadPixels(0,y,width,1,GL_RGB,GL_UNSIGNED_BYTE, 
			(GLvoid *) m_pixels);
		fwrite(m_pixels, 3, width, fp);
	}
}

