#include "loadprogressscreen.h"
#include "ui_loadprogressscreen.h"

LoadProgressScreen::LoadProgressScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadProgressScreen)
{
    ui->setupUi(this);

    hexView = findChild<QGraphicsView*>("hexView");
    progressLabel = findChild<QLabel*>("progressLabel");
    tips = findChild<QLabel*>("tipLabel");
    progression = 0;
    setTips();
    setProgressText();

}

LoadProgressScreen::~LoadProgressScreen(){
    delete ui;
}

void LoadProgressScreen::setNumberOfHexes(int width, int height){
    numberOfHex = width * height;
}

void LoadProgressScreen::startAnimation(){
    QGraphicsScene* hexScene = new QGraphicsScene;
    QImage hexagonImage(":images/hexagon.png");
    QGraphicsPixmapItem hexagon(QPixmap::fromImage(hexagonImage));
    hexScene->addItem(&hexagon);
    hexView->setScene(hexScene);

}

void LoadProgressScreen::setTips(){
    int randomTip = rand() % 3;
    QFont font = tips->font();
    font.setItalic(true);
    tips->setFont(font);

    switch(randomTip){
        case 0:
            tips->setText("Dont forget to bring a weapon to a fight!");
            break;
        case 1:
            tips->setText("Building a Silo will greatly increase production in a territory");
            break;
        case 2:
            tips->setText("Build a ram to counter opponents with walls");
            break;

    }


}

void LoadProgressScreen::setProgressText(){

    progressLabel->setText("");
    progressLabel->setText("Loading...");

}

void LoadProgressScreen::setProgress(){
    progression++;
    setProgressText();
}

