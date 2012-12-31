#include "ObjData.h"
#include "readObjData.h"

#ifndef SCENE_H
#define SCENE_H

void sceneInit(ObjData *Obj);
void drawObj();
void drawWhtPieces();
void drawBlkPieces();
void drawBoard();
void drawCase();
void drawScene(GLfloat *object1Position, GLfloat *object2Position, bool box);
void drawFloor();
void drawTableAndChairs();
void drawTable();
void drawChairR();
void drawChairL();
void drawBox(GLfloat *scale, GLfloat *translate, GLfloat *color);
void drawRoom();
void drawAwning();
void drawTeapot(GLfloat *object1Position, GLfloat *object2Position, bool box);

#endif
