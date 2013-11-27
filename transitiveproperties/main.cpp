/** \file main.cpp
 *  @brief main function
 *  @author Luis Palacios
 *  @author Andrew Ring
 */

#include "mainwindow.h"
#include "mapdata.h"
#include "mapeditor.h"
#include "maptest.h"
#include "mapcreatortest.h"
#include "testingflags.h"
#include <QtUiTools>
#include <QFile>
#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPushButton>
#include "mybutton.h"

using namespace std;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if(testingFlags::TEST_MAP_CREATOR){
        mapGenTestSpace::runBasicTest();
        return 0;
    }

    QApplication app(argc, argv);
    int screenHeight = app.desktop()->screenGeometry().height();
    int screenWidth = app.desktop()->screenGeometry().width();
    MainWindow window;
    window.setStyleSheet(QString::fromUtf8("MyButton { \n"
                                           "border-image:url(:/images/button.png);\n"
                                           "color: rgb(255,255,255);\n"
                                           "font: 6pt"
                                           "}\n"

                                           "MyButton:pressed {\n"
                                           "border-image:url(:/images/buttonPressed.png);\n"
                                           "color: rgb(255,255,255);\n"
                                           "font: 6pt"
                                           "}\n"

                                           "MyButton:disabled {\n"
                                           "border-image:url(:/images/buttonDisabled.png);\n"
                                           "color: rgb(255,255,255);\n"
                                           "font: 6pt"
                                           "}"));
    window.resize(screenWidth, screenHeight);
    window.showMaximized();

    return app.exec();



}


