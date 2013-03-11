/*
 * Copyright (c) 2007-2010 Michael Shafae
 * All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Id: PPMImage.h 2602 2010-11-15 09:46:11Z mshafae $
 *
 * The image class defines a trivial encoding for images known as PPM
 * format; it simply consists of an array or RGB triples, with one byte
 * per component, preceeded by a simple header giving the size of the
 * image.
 */


#ifndef _MSGFX_PPMIMAGE_H_
#define _MSGFX_PPMIMAGE_H_

#include "Pixel.h"

namespace msgfx{
  
class PPMImage {
public:
  // width == x resolution
  // height == y resolution
  PPMImage( int width, int height );
  PPMImage( const char *file_name );
  ~PPMImage( ) { delete[] pixels; }
  bool write( const char *file_name );
  // Read will overwrite whatever is currently stored within the PPMImage
  bool read( const char *file_name );
  msgfx::Pixel & operator()( int i, int j ) { return *( pixels + ( i * _width + j ) ); }
  unsigned int getWidth( ){ return( _width ); };
  unsigned int getHeight( ){ return( _height ); };
private:
  msgfx::Pixel *pixels;
  unsigned int _width;
  unsigned int _height;
};

} // end namespace msgfx

#endif
