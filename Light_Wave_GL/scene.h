#ifndef SCENE_H
#define SCENE_H

void drawScene(GLfloat *object1Position, GLfloat *object2Position, bool box);
void drawFloor();
void drawTableAndChairs();
void drawBox(GLfloat *scale, GLfloat *translate, GLfloat *color);
void drawRoom();
void drawAwning();
void drawTeapot(GLfloat *object1Position, GLfloat *object2Position, bool box);

#endif