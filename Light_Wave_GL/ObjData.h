/* Michael Robertson
 * mirob2005@gmail.com
 * Simple/Naive .obj parser
 * .Obj Specification used: http://www.martinreddy.net/gfx/3d/OBJ.spec
 */

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;

#ifndef OBJDATA_H
#define OBJDATA_H

// Data to parse:
// v - geometric vertices
// vn - vertex normals
// vt -  texture vertices
// f - faces 3 vertices only (4 vertices not supported yet)
// Other data will be ignored for now...


class ObjData {
private:
    std::string _filename;
    unsigned int _vCount;
    unsigned int _vnCount;
    unsigned int _vtCount;
    unsigned int _fCount;

public:
    //Array for vertices: [vertex][x,y,z]
    float **vertices;
    //Array for vertex normals: [vertex][x,y,z]
    //Vertex index for normals not necessarily same as above
    float **normals;
    //Array for texture vertices: [vertex][x,y,z], z = 0.0000
    float **textureVertices;
    //Array for faces [face][vertex][v/vt/vn]
    int ***faces;

    ObjData( const char *filename, unsigned int vertexCount,
              unsigned int vertexNormalCount, unsigned int textureVertexCount,
              unsigned int faceCount ){
        _filename = filename;
        _vCount = vertexCount, _vnCount = vertexNormalCount, _vtCount = textureVertexCount;
        _fCount = faceCount;

        unsigned int i;
        if( !(vertices = (float**)calloc( _vCount, sizeof(float*))) ){
            std::cerr << "Didn't allocated memory for vertex array." << std::endl;
        }
        for( i = 0; i < _vCount; i++ ){
            if( !(vertices[i] = (float*)calloc( 3, sizeof(float) )) ){
                std::cerr << "Couldn't allocate memory for the vertices at location " << i << std::endl;
            }
        }

        if( !(normals = (float**)calloc( _vnCount, sizeof(float*))) ){
            std::cerr << "Didn't allocated memory for vertex normal array." << std::endl;
        }
        for( i = 0; i < _vnCount; i++ ){
            if( !(normals[i] = (float*)calloc( 3, sizeof(float) )) ){
                std::cerr << "Couldn't allocate memory for the vertex normals at location " << i << std::endl;
            }
        }

        if( !(textureVertices = (float**)calloc( _vtCount, sizeof(float*))) ){
            std::cerr << "Didn't allocated memory for texture vertex array." << std::endl;
        }
        for( i = 0; i < _vtCount; i++ ){
            if( !(textureVertices[i] = (float*)calloc( 3, sizeof(float) )) ){
                std::cerr << "Couldn't allocate memory for the texture vertices at location " << i << std::endl;
            }
        }

        if( !(faces = (int***)calloc( _fCount, sizeof(int**))) ){
            std::cerr << "Didn't allocated memory for faces array." << std::endl;
        }
        for( i = 0; i < _fCount; i++ ){
            if( !(faces[i] = (int**)calloc( 3, sizeof(int*) )) ){
                std::cerr << "Couldn't allocate memory for the face vertices at location " << i << std::endl;
            }
        }
        for( i = 0; i < _fCount; i++ ){
            for(unsigned int j = 0; j < 3; j++){
                if( !(faces[i][j] = (int*)calloc( 3, sizeof(int) )) ){
                    std::cerr << "Couldn't allocate memory for the face vertes type at location " << i << ", " << j << std::endl;
                }
            }
        }
//        cerr<< "Object Data Created\n";
    }

    ~ObjData( ){
        for( unsigned int i = 0; i < _vCount; i++ ){
            free( vertices[i] );
        }
        for( unsigned int i = 0; i < _vtCount; i++ ){
            free( textureVertices[i] );
        }
        for( unsigned int i = 0; i < _vnCount; i++ ){
            free( normals[i] );
        }
        for( unsigned int i = 0; i < _fCount; i++ ){
            for( unsigned int j = 0; j < 3; j++){
                free( faces[i][j]);
            }
            free( faces[i] );
        }
        free( vertices );
        free( textureVertices );
        free( normals );
        free( faces );
//        cerr << "Object Data freed\n";
    }

    unsigned int vertexCount( ){
        return _vCount;
    }

    unsigned int vertexNormalCount( ){
        return _vnCount;
    }

    unsigned int textureVertexCount( ){
        return _vtCount;
    }

    unsigned int faceCount( ){
        return _fCount;
    }

    std::string filename( ){
        return _filename;
    }
};

#endif // OBJDATA_H
