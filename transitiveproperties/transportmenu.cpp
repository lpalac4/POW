#include "transportmenu.h"
#include "ui_transportmenu.h"

TransportMenu::TransportMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransportMenu)
{
    ui->setupUi(this);
//    mapView = view;
//    mapScene = mapView->scene();
//    mapImage = (MapImage*)(mapScene->items().front());
//    mapEdit = mapImage->parentWindow->mapCreator->getMapEditor();

    sourceTerritory = NULL;
    destinationTerritory = NULL;
    resetMateriel();

    QObject::connect(this, SIGNAL(finished()), this, SLOT(deselect()));
    resetButtons();

    //ui->doneButton->setEnabled(false);
    //QObject::connect(radioGroup, radioGroup.buttonClicked(), this, enableDoneButton();)


}

TransportMenu::~TransportMenu()
{
    delete ui;
}

/*void TransportMenu::setMapEditor(MapEditor* mapEditor, MapImage* map){
    mapEdit = mapEditor;
    mapImage = map;
    QObject::connect(mapImage, SIGNAL(territoryNeedsUpdate(Territory*)), this, SLOT(setSourceTerritory(Territory*)));
}*/

void TransportMenu::setSourceTerritory(Territory* targetTerritory){
    resetMateriel();

    if(targetTerritory == NULL || targetTerritory->ownedBy != MainWindow::currentPlayer || !targetTerritory->selected){
        //sourceTerritory = NULL;
        return;
    }else{
        sourceTerritory = targetTerritory;
        emit sourceHasBeenSet(sourceTerritory);
    }
}

void TransportMenu::setDestinationTerritory(Territory* targetTerritory){
    if(targetTerritory != NULL && targetTerritory->selected && targetTerritory != sourceTerritory){
        destinationTerritory = targetTerritory;
        enableAcceptButton();
    }
}

void TransportMenu::acceptTransportManifest(materiel toTransport){
    itemsToTransport = toTransport;
    if(itemsToTransport.stash){
        emit chosenShipmentFromSource(sourceTerritory, 10000);
    //}else if(itemsToTransport.boat){//Not implemented. This should not happen

    }else if(itemsToTransport.horse){
        emit chosenShipmentFromSource(sourceTerritory, 2);
    }else if(itemsToTransport.weapon || itemsToTransport.ram){
        emit chosenShipmentFromSource(sourceTerritory, 1);
    }
}

/**
 * @brief Resets push buttons. At this point, only disables the accept button.
 */
void TransportMenu::resetButtons(){
    ui->acceptButton->setEnabled(false);
}

void TransportMenu::on_acceptButton_clicked(){
    if(sourceTerritory == NULL || destinationTerritory == NULL){
        return;
    }
    if(itemsToTransport.stash){
        sourceTerritory->stockpile = false;
        destinationTerritory->stockpile = true;
    }
    if(itemsToTransport.boat){
        sourceTerritory->boatPresent = false;
        destinationTerritory->boatPresent = true;
    }
    if(itemsToTransport.horse){
        sourceTerritory->horsePresent = false;
        destinationTerritory->horsePresent = true;
    }
    if(itemsToTransport.weapon){
        sourceTerritory->weaponPresent = false;
        destinationTerritory->weaponPresent = true;
    }
    if(itemsToTransport.ram){
        sourceTerritory->ramPresent = false;
        destinationTerritory->ramPresent = true;
    }

    /*mapEdit->redraw(sourceTerritory);
    mapEdit->redraw(destinationTerritory);*/
    //Still need to figure out redrawing and moving to the next stage.
    emit finished();
}

/**
 * @brief Enables the Accept button.
 */
void TransportMenu::enableAcceptButton(){
    if(sourceTerritory != NULL && destinationTerritory != NULL){
        ui->acceptButton->setEnabled(true);
    }

}

void TransportMenu::on_skipButton_clicked(){
    emit finished();
}

void TransportMenu::on_deselectButton_clicked(){
    deselect();
}

void TransportMenu::deselect(){
    sourceTerritory = NULL;

    destinationTerritory = NULL;

    resetMateriel();

    resetButtons();

    emit deselectAll();
}


void TransportMenu::resetMateriel(){
    itemsToTransport.stash = false;
    itemsToTransport.boat = false;
    itemsToTransport.horse = false;
    itemsToTransport.weapon = false;
    itemsToTransport.ram = false;
}
