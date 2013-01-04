#include "ObjData.h"
#include "readObjData.h"

#ifndef SCENE_H
#define SCENE_H

void sceneInit(ObjData *Obj);
//Chessboard Scene
void drawChessScene();
void drawWhtPieces();
void drawBlkPieces();
void drawBoard();
void drawCase();
//Patio Scene
void drawScene(GLfloat *object1Position, GLfloat *object2Position);
void drawFloor();
void drawTableAndChairs();
void drawTable();
void drawChairR();
void drawChairL();
void drawRoom();
void drawAwning();
void drawTeapot(GLfloat *object1Position, GLfloat *object2Position);
void drawGrasses();
//Grass Scene
void drawGrassScene();
//Primitives
void drawBox(GLfloat *scale, GLfloat *translate, GLfloat *color);
void drawGrass(GLfloat *scale, GLfloat *translate);

#endif
