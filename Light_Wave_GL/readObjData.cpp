/* Michael Robertson
 * mirob2005@gmail.com
 * Simple/Naive .obj parser
 * .Obj Specification used: http://www.martinreddy.net/gfx/3d/OBJ.spec
 */

#include "readObjData.h"
#include <cstring>
#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;

//Read through data once to calculate the amount of space to allocate
//and then read through a second time to fill data

ObjData* readObjData( const char* filename ){
    char buffer[255];
    ifstream inputfile;
    float v[3];
    int a[3], b[3], c[3];
    unsigned int vCount = 0;
    unsigned int vnCount = 0;
    unsigned int vtCount = 0;
    unsigned int fCount = 0;
    ObjData *Obj;
    assert( filename );
    inputfile.open( filename, ios::in );
    if( inputfile.fail( ) ){
        cerr << "File \"" << filename << "\" not found." << endl;
        exit( 1 );
    }

    /*
    *    Read Through the file once to size the arrays
    */
    while(inputfile.getline(buffer, sizeof(buffer), '\n') != NULL){
        //Skipping Commented Lines
        if(strncmp(buffer, "#", 1) == 0){
            //cerr << "Skipping Commented Line\n";
        }
        //Skipping Empty Line
        else if(strncmp(buffer, "", 1) == 0){
            //cerr << "Skipping Empty Line\n";
        }
        else if(strncmp(buffer, "v ", 2) == 0){
            vCount++;
        }
        else if(strncmp(buffer, "vn ", 3) == 0){
            vnCount++;
        }
        else if(strncmp(buffer, "vt ", 3) == 0){
            vtCount++;
        }
        else if(strncmp(buffer, "f ", 2) == 0){
            fCount++;
        }
        else {
            //cerr << "Character Not Found!:";
            //cerr << buffer << endl;
        }
    }

//    cout << "Number of geometric vertices = " << vCount << endl;
//    cout << "Number of vertex normals = " << vnCount << endl;
//    cout << "Number of texture vertices = " << vtCount << endl;
//    cout << "Number of faces = " << fCount << endl;

    /*
     *  Go back to beginning of file
     */
    int length;
    inputfile.seekg (0, ios::end);
    length = inputfile.tellg();

    inputfile.seekg (-length, ios::end);
    length = inputfile.tellg();


    // Allocate ObjData
    if( !(Obj = new ObjData( filename, vCount, vnCount, vtCount, fCount )) ){
      cerr << "Could not allocate Object Data." << endl;
      exit(1);
    }

    /*
    *    Read through the file again to fill the arrays
    */
    unsigned int curV = 0, curVT = 0, curVN = 0, curF = 0;
    while(inputfile.getline(buffer, sizeof(buffer), '\n') != NULL){
        //Skipping Commented Lines
        if(strncmp(buffer, "#", 1) == 0){
            //cerr << "Skipping Commented Line\n";
        }
        //Skipping Empty Line
        else if(strncmp(buffer, "", 1) == 0){
            //cerr << "Skipping Empty Line\n";
        }
        else if(strncmp(buffer, "v ", 2) == 0){
            //Gather Vertex Data
            sscanf_s(buffer,"v  %f %f %f", &v[0], &v[1], &v[2]);
            for(unsigned int j = 0; j < 3; j++ ){
                Obj->vertices[curV][j] = v[j];
            }
            curV++;
        }
        else if(strncmp(buffer, "vn ", 3) == 0){
            //Gather Vertex Normal Data
            sscanf_s(buffer,"vn %f %f %f", &v[0], &v[1], &v[2]);
            for(unsigned int j = 0; j < 3; j++ ){
                Obj->normals[curVN][j] = v[j];
            }
            curVN++;
        }
        else if(strncmp(buffer, "vt ", 3) == 0){
            //Gather Texture Vertex Data
            sscanf_s(buffer,"vt %f %f %f", &v[0], &v[1], &v[2]);
            for(unsigned int j = 0; j < 3; j++ ){
                Obj->textureVertices[curVT][j] = v[j];
            }
            curVT++;
        }
        else if(strncmp(buffer, "f ", 2) == 0){
            //Gather Face Data
            sscanf_s(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &a[0], &a[1], &a[2], &b[0], &b[1], &b[2], &c[0], &c[1], &c[2]);
            for(unsigned int j = 0; j < 3; j++ ){
                Obj->faces[curF][0][j] = a[j];
                Obj->faces[curF][1][j] = b[j];
                Obj->faces[curF][2][j] = c[j];
            }
            curF++;
        }
        else {
            //cerr << "Character Not Found!:";
            //cerr << buffer << endl;
        }
    }

    if(curV!=vCount||curVT!=vtCount||curVN!=vnCount||curF!=fCount){
        cerr << "Vertex/Face counts don't match up!\n";
        exit(1);
    }

    inputfile.close( );
    return( Obj );
}
