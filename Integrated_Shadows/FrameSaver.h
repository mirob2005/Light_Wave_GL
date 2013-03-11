#ifndef FrameSaver_h 
#define FrameSaver_h
/*
	FrameSaver code from UCLA CS174A Assignment#1 Spring 2009

*/
/*

	This class reads the OpenGL frame buffer and dumps it a sequence of PPM 
	files. This is useful for recording movies of your animations.

	NOTE: this class makes extensive use of old-school c-style functions, 
	such as sprintf and fprint. A proper C++ implementation should use 
	C++ strings, file
	streams for file I/O, and the Boost lexical_cast for formatted strings.
	You can find more info about lexical_cast here: 
	
	http://www.boost.org/doc/libs/1_36_0/libs/conversion/lexical_cast.htm

*/

class FrameSaver {
public:
	FrameSaver() ;
		
	unsigned char *m_pixels;
	int m_record ;
	int m_frameCount ;
	int DumpPPM(int w, int h) ;
	void DumpPPM(FILE *fp,int w, int h) ;
	void StartRecord(void) ;
	void StopRecord(void) ;
	void Toggle(void) ;
	int GetRecording(void) { return m_record ;}
	void SetRecording(int r) { m_record = r ;}
	void ConvertToPng(char *baseName) ;
} ;


#endif
