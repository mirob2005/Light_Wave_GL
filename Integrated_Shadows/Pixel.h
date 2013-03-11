/*
 * Copyright (c) 2010 Michael Shafae
 * All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Id: Pixel.h 2602 2010-11-15 09:46:11Z mshafae $
 *
 * Simple pixel class using unsigned chars for each channel
 * Not very useful with OpenGL but handy for reading & writing images
 *
 */

#ifndef _MSGFX_PIXEL_H_
#define _MSGFX_PIXEL_H_

#include <ostream>

typedef unsigned char channel;  

namespace msgfx{
  
  class Pixel {
  public:
    Pixel( ) { r = 0; g = 0; b = 0; };
    Pixel( channel _r, channel _g, channel _b ) { r = _r; g = _g; b = _b; };
    void write( std::ostream &out ) const{
      out << "[" << r << ", " << g << ", " << b <<", " << "]";
    };
    channel r;
    channel g;
    channel b;
    channel a;
  };

  std::ostream& operator <<( std::ostream &out, const Pixel &p );

} // end namespace msgfx

#endif