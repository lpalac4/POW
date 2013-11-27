#include "transportmenuoptions.h"
#include "ui_transportmenuoptions.h"

TransportMenuOptions::TransportMenuOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransportMenuOptions)
{
    ui->setupUi(this);

    radioGroup.setExclusive(true);
    boatGroup.setExclusive(false);
    horseGroup.setExclusive(false);
}

TransportMenuOptions::~TransportMenuOptions()
{
    delete ui;
}

void TransportMenuOptions::setBoatGroupEnabled(bool enabled){
    QList<QAbstractButton*> list = boatGroup.buttons();
    QAbstractButton* button;
    while(!list.empty()){
        button = list.front();
        list.pop_front();
        button->setChecked(false);
        button->setEnabled(enabled);
    }
}

void TransportMenuOptions::setHorseGroupEnabled(bool enabled){
    QList<QAbstractButton*> list = horseGroup.buttons();
    QAbstractButton* button;
    while(!list.empty()){
        button = list.front();
        list.pop_front();
        button->setChecked(false);
        button->setEnabled(enabled);
    }
}

void TransportMenuOptions::clearButtonGroups(){
    QList<QAbstractButton*> list = radioGroup.buttons();
    while(!list.empty()){
        radioGroup.removeButton(list.front());
        list.pop_front();
    }
    list = boatGroup.buttons();
    while(!list.empty()){
        boatGroup.removeButton(list.front());
        list.pop_front();
    }
    list = horseGroup.buttons();
    while(!list.empty()){
        horseGroup.removeButton(list.front());
        list.pop_front();
    }
}

void TransportMenuOptions::setButtons(Territory* sourceTerritory){
    ui->backButton->setFocus();
    clearButtonGroups();
    setUnitButtonsChecked(false);
    setUnitButtonsEnabled(false);

    if(sourceTerritory == NULL){
        return;
    }

    if(sourceTerritory->stockpile){
        radioGroup.addButton(ui->stashRadio);
        setButtonEnabled(ui->stashRadio, true);
    }
    if(sourceTerritory->boatPresent){
        radioGroup.addButton(ui->boatRadio);
        setButtonEnabled(ui->boatRadio, true);
        if(sourceTerritory->horsePresent){
            setButtonEnabled(ui->horseOnBoat, true);
            boatGroup.addButton(ui->horseOnBoat);
        }
        if(sourceTerritory->weaponPresent){
            setButtonEnabled(ui->weaponOnBoat, true);
            boatGroup.addButton(ui->weaponOnBoat);
        }
        if(sourceTerritory->ramPresent){
            setButtonEnabled(ui->ramOnBoat, true);
            boatGroup.addButton(ui->ramOnBoat);
        }
    }
    if(sourceTerritory->horsePresent){
        radioGroup.addButton(ui->horseRadio);
        setButtonEnabled(ui->horseRadio, true);
        if(sourceTerritory->weaponPresent){
            setButtonEnabled(ui->weaponOnHorse, true);
            horseGroup.addButton(ui->weaponOnHorse);
        }
        if(sourceTerritory->ramPresent){
            setButtonEnabled(ui->ramOnHorse, true);
            horseGroup.addButton(ui->ramOnHorse);
        }
    }
    if(sourceTerritory->weaponPresent){
        radioGroup.addButton(ui->weaponRadio);
        setButtonEnabled(ui->weaponRadio, true);
    }
    if(sourceTerritory->ramPresent){
        radioGroup.addButton(ui->ramRadio);
        setButtonEnabled(ui->ramRadio, true);
    }
}

void TransportMenuOptions::setUnitButtonsEnabled(bool enabled){
    setButtonEnabled(ui->stashRadio, enabled);
    setButtonEnabled(ui->boatRadio, enabled);
    setButtonEnabled(ui->horseOnBoat, enabled);
    setButtonEnabled(ui->weaponOnBoat, enabled);
    setButtonEnabled(ui->ramOnBoat, enabled);
    setButtonEnabled(ui->horseRadio, enabled);
    setButtonEnabled(ui->weaponOnHorse, enabled);
    setButtonEnabled(ui->ramOnHorse, enabled);
    setButtonEnabled(ui->weaponRadio, enabled);
    setButtonEnabled(ui->ramRadio, enabled);
}

void TransportMenuOptions::setUnitButtonsChecked(bool checked){
    ui->stashRadio->setAutoExclusive(false);
    ui->stashRadio->setChecked(checked);
    ui->stashRadio->setAutoExclusive(true);

    ui->boatRadio->setAutoExclusive(false);
    ui->boatRadio->setChecked(checked);
    ui->boatRadio->setAutoExclusive(true);

    ui->horseOnBoat->setChecked(checked);
    ui->weaponOnBoat->setChecked(checked);
    ui->ramOnBoat->setChecked(checked);

    ui->horseRadio->setAutoExclusive(false);
    ui->horseRadio->setChecked(checked);
    ui->horseRadio->setAutoExclusive(true);

    ui->weaponOnHorse->setChecked(checked);
    ui->ramOnHorse->setChecked(checked);

    ui->weaponRadio->setAutoExclusive(false);
    ui->weaponRadio->setChecked(checked);
    ui->weaponRadio->setAutoExclusive(true);

    ui->ramRadio->setAutoExclusive(false);
    ui->ramRadio->setChecked(checked);
    ui->ramRadio->setAutoExclusive(true);

}

void TransportMenuOptions::on_acceptButton_clicked(){
    materiel sending;
    sending.stash = false;
    sending.boat = false;
    sending.horse = false;
    sending.weapon = false;
    sending.ram = false;
    if(ui->stashRadio->isChecked()){
        sending.stash = true;
    }else if(ui->boatRadio->isChecked()){
        sending.boat = true;
        if(ui->horseOnBoat->isChecked()){
            sending.horse = true;
        }
        if(ui->weaponOnBoat->isChecked()){
            sending.weapon = true;
        }
        if(ui->ramOnBoat->isChecked()){
            sending.ram = true;
        }
    }else if(ui->horseRadio->isChecked()){
        sending.horse = true;
        if(ui->weaponOnHorse->isChecked()){
            sending.weapon = true;
        }
        if(ui->ramOnHorse->isChecked()){
            sending.ram = true;
        }
    }else if(ui->weaponRadio->isChecked()){
        sending.weapon = true;
    }else if(ui->ramRadio->isChecked()){
        sending.ram = true;
    }else{
        return;
    }
    emit chosenMateriel(sending);
}

void TransportMenuOptions::on_boatRadio_toggled(bool checked){
    setBoatGroupEnabled(checked);
}

void TransportMenuOptions::on_horseRadio_toggled(bool checked){
    setHorseGroupEnabled(checked);
}

void TransportMenuOptions::on_backButton_clicked(){
    emit goBack();
}

void TransportMenuOptions::setButtonEnabled(QAbstractButton* button, bool enable) {
    button->setEnabled(enable);
    if(enable) {
        button->setStyleSheet("QAbstractButton {color:white}");
    } else {
        button->setStyleSheet("QAbstractButton {color:grey}");
    }
}
