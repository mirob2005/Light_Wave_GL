#-------------------------------------------------
#
# Project created by QtCreator 2012-10-26T20:09:02
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Light_Wave_GL
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mypanelopengl.cpp \
    scene.cpp \
    misc.cpp \
    FrameSaver.cpp \
    GLee/GLee.c

HEADERS  += mainwindow.h \
    mypanelopengl.h \
    misc.h \
    GLSLShader.h \
    FrameSaver.h \
    GLee/GLee.h \
    mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    vertexShader.vs \
    fragShader.vs
