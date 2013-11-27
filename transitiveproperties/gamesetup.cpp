/** \file gamesetup.cpp
 *  @brief Definition of programmatic elements of game setup menu.
 *  @author Charles Myers
 */

#include "gamesetup.h"

GameSetup::GameSetup(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    toggleButtons.push_back(findChild<QPushButton*>("playerToggle1"));
    toggleButtons.push_back(findChild<QPushButton*>("playerToggle2"));
    toggleButtons.push_back(findChild<QPushButton*>("playerToggle3"));
    toggleButtons.push_back(findChild<QPushButton*>("playerToggle4"));
    toggleButtons.push_back(findChild<QPushButton*>("playerToggle5"));
    toggleButtons.push_back(findChild<QPushButton*>("playerToggle6"));

    toggleButtons[0]->setEnabled(true);
    toggleButtons[1]->setEnabled(true);
    toggleButtons[2]->setEnabled(false);
    toggleButtons[3]->setEnabled(false);
    toggleButtons[4]->setEnabled(false);
    toggleButtons[5]->setEnabled(false);

    for(int i=0; i<6; i++){
        toggleButtons[i]->setText("Human");
    }

    QObject::connect(findChild<QComboBox*>("numberOfPlayersSelect"), SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtonEnabledness(int)));

    QSignalMapper* sigMap = new QSignalMapper(this);
    sigMap->setMapping(toggleButtons[0], QString("1"));
    sigMap->setMapping(toggleButtons[1], QString("2"));
    sigMap->setMapping(toggleButtons[2], QString("3"));
    sigMap->setMapping(toggleButtons[3], QString("4"));
    sigMap->setMapping(toggleButtons[4], QString("5"));
    sigMap->setMapping(toggleButtons[5], QString("6"));

    QObject::connect(toggleButtons[0], SIGNAL(pressed()), sigMap, SLOT(map()));
    QObject::connect(toggleButtons[1], SIGNAL(pressed()), sigMap, SLOT(map()));
    QObject::connect(toggleButtons[2], SIGNAL(pressed()), sigMap, SLOT(map()));
    QObject::connect(toggleButtons[3], SIGNAL(pressed()), sigMap, SLOT(map()));
    QObject::connect(toggleButtons[4], SIGNAL(pressed()), sigMap, SLOT(map()));
    QObject::connect(toggleButtons[5], SIGNAL(pressed()), sigMap, SLOT(map()));

    QObject::connect(sigMap, SIGNAL(mapped(QString)), this, SLOT(toggleAIness(QString)));
}

void GameSetup::updateButtonEnabledness(int whichButton){
    for(int i=2; i<6; i++){
        toggleButtons[i]->setEnabled(false);
    }
    for(int i=2; i<=whichButton+1; i++){
        toggleButtons[i]->setEnabled(true);
    }
}

void GameSetup::toggleAIness(QString whichButton){
    QPushButton* tempButton;

    if(whichButton.compare("1") == 0){
        tempButton = toggleButtons[0];
    }else if(whichButton.compare("2") == 0){
        tempButton = toggleButtons[1];
    }else if(whichButton.compare("3") == 0){
        tempButton = toggleButtons[2];
    }else if(whichButton.compare("4") == 0){
        tempButton = toggleButtons[3];
    }else if(whichButton.compare("5") == 0){
        tempButton = toggleButtons[4];
    }else{
        tempButton = toggleButtons[5];
    }

    if(tempButton->text().compare("Human") == 0){
        tempButton->setText("AI");
    }else{
        tempButton->setText("Human");
    }
}
