/** \file productionscreen.cpp
 *  @brief Definitions of ProductionScreen member functions.
 *  @author Andrew Ring
 */

#include "productionscreen.h"

ProductionScreen::ProductionScreen(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    QPushButton* boatButton = findChild<QPushButton*>("boatButton");
    QPushButton* cityButton = findChild<QPushButton*>("cityButton");
    QPushButton* ramButton = findChild<QPushButton*>("ramButton");
    QPushButton* siloButton = findChild<QPushButton*>("siloButton");
    QPushButton* wallButton = findChild<QPushButton*>("wallButton");
    QPushButton* weaponButton = findChild<QPushButton*>("weaponButton");
    //QScrollArea* scrollArea = findChild<QScrollArea*>("scrollArea");

    QObject::connect(boatButton, SIGNAL(pressed()), this, SLOT(boatButtonClicked()), Qt::UniqueConnection);
    QObject::connect(cityButton, SIGNAL(pressed()), this, SLOT(cityButtonClicked()), Qt::UniqueConnection);
    QObject::connect(ramButton, SIGNAL(pressed()), this, SLOT(ramButtonClicked()), Qt::UniqueConnection);
    QObject::connect(siloButton, SIGNAL(pressed()), this, SLOT(siloButtonClicked()), Qt::UniqueConnection);
    QObject::connect(wallButton, SIGNAL(pressed()), this, SLOT(wallButtonClicked()), Qt::UniqueConnection);
    QObject::connect(weaponButton, SIGNAL(pressed()), this, SLOT(weaponButtonClicked()), Qt::UniqueConnection);

    QPushButton* endButton = findChild<QPushButton*>("endButton");
    QObject::connect(endButton, SIGNAL(pressed()), this, SIGNAL(productionFinished()));


//    changeButtonSize(boatButton, scrollArea, endButton);
//    changeButtonSize(cityButton, scrollArea, endButton);
//    changeButtonSize(ramButton, scrollArea, endButton);
//    changeButtonSize(siloButton, scrollArea, endButton);
//    changeButtonSize(wallButton, scrollArea, endButton);
//    changeButtonSize(weaponButton, scrollArea, endButton);

}
void ProductionScreen::changeButtonSize(QPushButton* button, QScrollArea* scroll, QPushButton* endButtonPtr){
    QSize buttonSize(scroll->width(), endButtonPtr->height());
    button->setMaximumSize(buttonSize);
    button->setMinimumSize(buttonSize);
}

void ProductionScreen::setMapImage(MapImage* mapPix)
{
    this->mapPix = mapPix;

    if(this->mapPix) {
        QObject::connect(mapPix,SIGNAL(territoryNeedsUpdate(Territory*)),
                         this,SLOT(updateProductionButtons()), Qt::UniqueConnection);
    }
}

void ProductionScreen::updateProductionResourceDisplay()
{
    QLabel* resourcesLabel = findChild<QLabel*>("resourcesLabel");
    resourcesLabel->setText(MainWindow::currentPlayer->stash->getStringRepresentation());
}

void ProductionScreen::boatButtonClicked()
{
    productionCheck(production::BOAT);
}

void ProductionScreen::cityButtonClicked()
{
    productionCheck(production::CITY);
}

void ProductionScreen::ramButtonClicked()
{
    productionCheck(production::RAM);
}

void ProductionScreen::siloButtonClicked()
{
    productionCheck(production::SILO);
}

void ProductionScreen::wallButtonClicked()
{
    productionCheck(production::WALL);
}

void ProductionScreen::weaponButtonClicked()
{
    productionCheck(production::WEAPON);
}

void ProductionScreen::productionCheck(production::units typeToBuild, bool ai)
{
    if(mapPix->currentSelection) {
        switch(typeToBuild) {
        case production::BOAT:
            if(ai || popupQuestion("Are you sure you want to build a BOAT here?")) {
                // not yet implemented!
            }
            break;
        case production::GOLD_BOAT:
            if(ai || popupQuestion("Are you sure you want to buy a BOAT with GOLD here?")) {
                // not yet implemented!
            }
            break;
        case production::CITY:
            if(ai || popupQuestion("Are you sure you want to build a CITY here?")) {
                mapPix->currentSelection->cityPresent = true;
                MainWindow::currentPlayer->stash->subtract(&production::CITY_COST);
                mapPix->currentSelection->radialCityCheck();
                mapPix->radialTerritoryUpdate(mapPix->currentSelection);
            }
            break;
        case production::GOLD_CITY:
            if(ai || popupQuestion("Are you sure you want to buy a CITY with GOLD here?")) {
                mapPix->currentSelection->cityPresent = true;
                MainWindow::currentPlayer->stash->subtract(&production::GOLD_CITY_COST);
                mapPix->currentSelection->radialCityCheck();
                mapPix->radialTerritoryUpdate(mapPix->currentSelection);
            }
            break;
        case production::RAM:
            if(ai || popupQuestion("Are you sure you want to build a RAM here?")) {
                mapPix->currentSelection->ramPresent = true;
                MainWindow::currentPlayer->stash->subtract(&production::RAM_COST);
                mapPix->updateTerritory(mapPix->currentSelection);
            }
            break;
        case production::GOLD_RAM:
            if(ai || popupQuestion("Are you sure you want to buy a RAM with GOLD here?")) {
                mapPix->currentSelection->ramPresent = true;
                MainWindow::currentPlayer->stash->subtract(&production::GOLD_RAM_COST);
                mapPix->updateTerritory(mapPix->currentSelection);
            }
            break;
        case production::SILO:
            if(ai || popupQuestion("Are you sure you want to build a SILO here?")) {
                mapPix->currentSelection->siloPresent = true;
                MainWindow::currentPlayer->stash->subtract(&production::SILO_COST);
                mapPix->updateTerritory(mapPix->currentSelection);
            }
            break;
        case production::GOLD_SILO:
            if(ai || popupQuestion("Are you sure you want to buy a SILO with GOLD here?")) {
                mapPix->currentSelection->siloPresent = true;
                MainWindow::currentPlayer->stash->subtract(&production::GOLD_SILO_COST);
                mapPix->updateTerritory(mapPix->currentSelection);
            }
            break;
        case production::WALL:
            if(ai || popupQuestion("Are you sure you want to build a WALL here?")) {
                mapPix->currentSelection->wallPresent = true;
                MainWindow::currentPlayer->stash->subtract(&production::WALL_COST);
                mapPix->updateTerritory(mapPix->currentSelection);
            }
            break;
        case production::GOLD_WALL:
            if(ai || popupQuestion("Are you sure you want to buy a WALL with GOLD here?")) {
                mapPix->currentSelection->wallPresent = true;
                MainWindow::currentPlayer->stash->subtract(&production::GOLD_WALL_COST);
                mapPix->updateTerritory(mapPix->currentSelection);
            }
            break;
        case production::WEAPON:
            if(ai || popupQuestion("Are you sure you want to build a WEAPON here?")) {
                mapPix->currentSelection->weaponPresent = true;
                MainWindow::currentPlayer->stash->subtract(&production::WEAPON_COST);
                mapPix->updateTerritory(mapPix->currentSelection);
            }
            break;
        case production::GOLD_WEAPON:
            if(ai || popupQuestion("Are you sure you want to buy a WEAPON with GOLD here?")) {
                mapPix->currentSelection->weaponPresent = true;
                MainWindow::currentPlayer->stash->subtract(&production::GOLD_WEAPON_COST);
                mapPix->updateTerritory(mapPix->currentSelection);
            }
            break;
        case production::COUNT:
            qDebug() << "Who tried to production a COUNT???";
            break;
        }

        updateProductionButtons();
        updateProductionResourceDisplay();
        mapPix->currentSelection->selected = false;
        mapPix->updateTerritory(mapPix->currentSelection);
    }
}

void ProductionScreen::updateProductionButtons()
{
    if(MainWindow::gameState == MainWindow::IN_PRODUCTION) {
        QPushButton* boatButton = findChild<QPushButton*>("boatButton");
        QPushButton* cityButton = findChild<QPushButton*>("cityButton");
        QPushButton* ramButton = findChild<QPushButton*>("ramButton");
        QPushButton* siloButton = findChild<QPushButton*>("siloButton");
        QPushButton* wallButton = findChild<QPushButton*>("wallButton");
        QPushButton* weaponButton = findChild<QPushButton*>("weaponButton");

        if(mapPix->currentSelection && !(mapPix->currentSelection->boatPresent)
                && MainWindow::currentPlayer->stash->equalOrGreaterThan(&production::BOAT_COST)) {
            boatButton->setEnabled(true);
        } else {
            boatButton->setEnabled(false);
        }

        if(mapPix->currentSelection && !(mapPix->currentSelection->cityPresent)
                && MainWindow::currentPlayer->stash->equalOrGreaterThan(&production::CITY_COST)) {
            cityButton->setEnabled(true);
        } else {
            cityButton->setEnabled(false);
        }

        if(mapPix->currentSelection && !(mapPix->currentSelection->ramPresent)
                && MainWindow::currentPlayer->stash->equalOrGreaterThan(&production::RAM_COST)) {
            ramButton->setEnabled(true);
        } else {
            ramButton->setEnabled(false);
        }

        if(mapPix->currentSelection && !(mapPix->currentSelection->siloPresent)
                && MainWindow::currentPlayer->stash->equalOrGreaterThan(&production::SILO_COST)
                && mapPix->currentSelection->resourceGen != HORSE) {
            siloButton->setEnabled(true);
        } else {
            siloButton->setEnabled(false);
        }

        if(mapPix->currentSelection && !(mapPix->currentSelection->wallPresent)
                && MainWindow::currentPlayer->stash->equalOrGreaterThan(&production::WALL_COST)) {
            wallButton->setEnabled(true);
        } else {
            wallButton->setEnabled(false);
        }

        if(mapPix->currentSelection && !(mapPix->currentSelection->weaponPresent)
                && MainWindow::currentPlayer->stash->equalOrGreaterThan(&production::WEAPON_COST)) {
            weaponButton->setEnabled(true);
        } else {
            weaponButton->setEnabled(false);
        }
    }
}

bool ProductionScreen::popupQuestion(const char* question) {
    QMessageBox mb(QMessageBox::Question, tr("?"), tr(question),
                   QMessageBox::Yes | QMessageBox::No, this);
    mb.setDefaultButton(QMessageBox::Yes);
    mb.show();
    QPoint pos = mapToGlobal(QPoint(width()/2  - mb.width()/2, height()/2 - mb.height()/2));
    mb.move(pos);
    if(mb.exec() == QMessageBox::Yes) {
        qDebug() << "Clicked Yes in Text Box";
        return true;
    } else {
        qDebug() << "Clicked No in Text Box";
        return false;
    }
}
