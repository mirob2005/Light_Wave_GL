/*
 * Copyright (c) 2007-2010 Michael Shafae
 * All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Id: PPMImage.cpp 2602 2010-11-15 09:46:11Z mshafae $
 *
 * The image class defines a trivial encoding for images known as PPM
 * format; it simply consists of an array or RGB triples, with one byte
 * per component, preceeded by a simple header giving the size of the
 * image.
 */

#include <stdio.h>


#include "PPMImage.h"

namespace msgfx{

 PPMImage::PPMImage( int width, int height ){
   _width  = width;
   _height = height;
   pixels = new msgfx::Pixel[ _width * _height ];
   msgfx::Pixel *p = pixels;
   for( unsigned int i = 0; i < _width * _height; i++ )
     *p++ = msgfx::Pixel(0,0,0);
 }

 PPMImage::PPMImage( const char *file_name ){
   pixels = NULL;
   if( !read( file_name ) ){
     fprintf( stderr, "%s: Warning: was not able to initialize PPMImage from file %s\n", __FILE__, file_name );
   }
 }

 bool PPMImage::read( const char *filename ){
   char buffer[100];
   FILE *fp;
   int maxval;
   bool rv = false;

   // open file
   if ((fp=fopen (filename, "rb"))==NULL){
       fprintf( stderr, "%s: Unable to open file %s\n", __FILE__, filename);
       return( rv );
   }

   // read file identifier (magic number)
   fgets (buffer, 100, fp);
   if ((buffer[0] != 'P') || (buffer[1] != '6')){
       fprintf(stderr, "%s: Incorrect file type\n", __FILE__ );
       return( rv );
   }

   // read image size
   do
       fgets(buffer, 100, fp);
   while(buffer[0] == '#' || buffer[0] == ' ');

   sscanf(buffer, "%u %u", &_width, &_height);

   // read maximum pixel value (usually 255)
   do{
       fgets (buffer, 100, fp);
   }while (buffer[0] == '#');

   sscanf(buffer, "%d", &maxval);

   if( maxval != 255 ){
     fprintf( stderr, "%s: Warning: maximum pixel value %d != 255\n", __FILE__, maxval );
   }

   // allocate RGBA texture buffer
   if( pixels != NULL ){
     delete pixels;
   }
   pixels = new msgfx::Pixel[ _width * _height ];

   // read RGB data and calculate alpha value
   int c;
   for( unsigned int i = 0; i < (_width * _height); i++){
     for( int j = 0; j < 4; j++ ){
       switch(j){
         case 0:
         // red channel
         c = fgetc(fp);
         pixels[i].r = static_cast<channel>(c);
         break;
         case 1:
         // green channel
         c = fgetc(fp);
         pixels[i].g = static_cast<channel>(c);
         break;
         case 2:
         // blue channel
         c = fgetc(fp);
         pixels[i].b = static_cast<channel>(c);
         break;
         case 3:
         // alpha channel - there is none in a PPM
         pixels[i].a = channel(255);
         break;
         default:
         fprintf( stderr, "%s: We should not reach the default case\n", __FILE__ );
         break;
       }      
     }
   }
   // close input file
   fclose(fp);
   rv = true;
   return( rv );
 }

 bool PPMImage::write( const char *file_name ){
   bool rv = false;
   msgfx::Pixel *p = pixels;
   FILE  *fp = fopen( file_name, "w+b" );
   if( fp == NULL ){
     return( rv );
   }
   fprintf( fp, "P6\n%d %d\n255\n", _width, _height );
   for( unsigned int i = 0; i < _width * _height; i++ ){
     fprintf( fp, "%c%c%c", p->r, p->g, p->b );
     p++;
   }
   fclose( fp );
   rv = true;
   return( rv );
 }
 
} // end namespace msgfx
 