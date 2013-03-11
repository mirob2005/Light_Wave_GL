/* Michael Robertson
 * mirob2005@gmail.com
 * Simple/Naive .obj parser
 * .Obj Specification used: http://www.martinreddy.net/gfx/3d/OBJ.spec
 */

#ifndef READOBJDATA_H
#define READOBJDATA_H

#include "ObjData.h"

ObjData* readObjData( const char* filename );

#endif // READOBJDATA_H
