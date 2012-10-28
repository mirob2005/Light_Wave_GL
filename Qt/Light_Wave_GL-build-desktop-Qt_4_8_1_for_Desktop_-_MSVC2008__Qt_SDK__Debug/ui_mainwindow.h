/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Oct 26 23:04:53 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "mypanelopengl.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    MyPanelOpenGL *panelGL;
    QPushButton *selectObject1Button;
    QPushButton *selectObject2Button;
    QPushButton *selectCameraButton;
    QPushButton *resetLightButton;
    QLCDNumber *fpsDisplay;
    QPushButton *resetObjectsButton;
    QPushButton *changeShaderButton;
    QPushButton *quitButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 850);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        panelGL = new MyPanelOpenGL(centralWidget);
        panelGL->setObjectName(QString::fromUtf8("panelGL"));
        panelGL->setGeometry(QRect(0, 0, 1280, 720));
        selectObject1Button = new QPushButton(centralWidget);
        selectObject1Button->setObjectName(QString::fromUtf8("selectObject1Button"));
        selectObject1Button->setGeometry(QRect(60, 740, 100, 30));
        selectObject2Button = new QPushButton(centralWidget);
        selectObject2Button->setObjectName(QString::fromUtf8("selectObject2Button"));
        selectObject2Button->setGeometry(QRect(210, 740, 100, 30));
        selectCameraButton = new QPushButton(centralWidget);
        selectCameraButton->setObjectName(QString::fromUtf8("selectCameraButton"));
        selectCameraButton->setGeometry(QRect(360, 740, 100, 30));
        resetLightButton = new QPushButton(centralWidget);
        resetLightButton->setObjectName(QString::fromUtf8("resetLightButton"));
        resetLightButton->setGeometry(QRect(510, 740, 100, 30));
        fpsDisplay = new QLCDNumber(centralWidget);
        fpsDisplay->setObjectName(QString::fromUtf8("fpsDisplay"));
        fpsDisplay->setGeometry(QRect(960, 740, 100, 30));
        resetObjectsButton = new QPushButton(centralWidget);
        resetObjectsButton->setObjectName(QString::fromUtf8("resetObjectsButton"));
        resetObjectsButton->setGeometry(QRect(660, 740, 100, 30));
        changeShaderButton = new QPushButton(centralWidget);
        changeShaderButton->setObjectName(QString::fromUtf8("changeShaderButton"));
        changeShaderButton->setGeometry(QRect(810, 740, 100, 30));
        quitButton = new QPushButton(centralWidget);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));
        quitButton->setGeometry(QRect(1110, 740, 100, 30));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(quitButton, SIGNAL(clicked()), MainWindow, SLOT(close()));
        QObject::connect(selectObject1Button, SIGNAL(clicked()), panelGL, SLOT(selectObject1()));
        QObject::connect(selectObject2Button, SIGNAL(clicked()), panelGL, SLOT(selectObject2()));
        QObject::connect(selectCameraButton, SIGNAL(clicked()), panelGL, SLOT(selectCamera()));
        QObject::connect(resetLightButton, SIGNAL(clicked()), panelGL, SLOT(resetLight()));
        QObject::connect(resetObjectsButton, SIGNAL(clicked()), panelGL, SLOT(resetObjects()));
        QObject::connect(changeShaderButton, SIGNAL(clicked()), panelGL, SLOT(changeShader()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        selectObject1Button->setText(QApplication::translate("MainWindow", "Select Object 1", 0, QApplication::UnicodeUTF8));
        selectObject2Button->setText(QApplication::translate("MainWindow", "Select Object 2", 0, QApplication::UnicodeUTF8));
        selectCameraButton->setText(QApplication::translate("MainWindow", "Select Camera", 0, QApplication::UnicodeUTF8));
        resetLightButton->setText(QApplication::translate("MainWindow", "Reset Light", 0, QApplication::UnicodeUTF8));
        resetObjectsButton->setText(QApplication::translate("MainWindow", "Reset Objects", 0, QApplication::UnicodeUTF8));
        changeShaderButton->setText(QApplication::translate("MainWindow", "Shader ON/OFF", 0, QApplication::UnicodeUTF8));
        quitButton->setText(QApplication::translate("MainWindow", "QUIT", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
