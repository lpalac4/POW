/** \file territoryselection.cpp
 *  @brief Definitions of TerritorySelection member functions.
 *  @author Andrew Ring
 */

#include "territoryselection.h"

unsigned int TerritorySelection::territoriesThisRound;
std::list<Territory*> TerritorySelection::currentlySelected;

/**
 * @brief Default constructor.
 * @param parent The parent widget.
 */
TerritorySelection::TerritorySelection(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    QPushButton* autoButton = this->findChild<QPushButton*>("autoButton");
    QObject::connect(autoButton, SIGNAL(clicked()), this, SLOT(autoSelectChosen()), Qt::UniqueConnection);
}

/**
 * @brief Rewrites the text in the phaseLabel to show appropriate values.  Also enables/disables the
 * accept button as appropriate.
 */
void TerritorySelection::updateSelectionPhaseLabel() {
    if(MainWindow::gameState == MainWindow::IN_TERRITORYSELECTION) {
        QPushButton* acceptButton = this->findChild<QPushButton*>("acceptButton");
        QPushButton* resetButton = this->findChild<QPushButton*>("resetButton");
        QPushButton* autoButton = this->findChild<QPushButton*>("autoButton");
        QLabel* phaseLabel = this->findChild<QLabel*>("phaseLabel");

        if((!MainWindow::currentPlayer) || MainWindow::currentPlayer->isAI) {
            autoButton->setEnabled(false);
        } else {
            autoButton->setEnabled(true);
        }
        if(!MainWindow::currentPlayer || !MainWindow::currentTurn->getPlayerComplete(MainWindow::currentPlayer)) {
            // Player has not yet accepted this turn.
            phaseLabel->setText(generateSelectionStateMessage());
            if(currentlySelected.size() == territoriesThisRound) {
                acceptButton->setEnabled(true);
            } else {
                acceptButton->setEnabled(false);
            }
            resetButton->setEnabled(true);
        } else {
            // Player already accepted this turn.
            phaseLabel->setText("Waiting for other players...");
            acceptButton->setEnabled(false);
            resetButton->setEnabled(false);
        }
    }
}

/**
 * @brief Generates a QString of the format "Selected <NumSelected>/<AvailableToSelectThisRound> territories"
 * @return A string like "Selected 1/4 territories" with appropriate values.
 */
QString TerritorySelection::generateSelectionStateMessage() {
    stringstream messageStream;

    if(MainWindow::gameState == MainWindow::IN_TERRITORYSELECTION){
        messageStream << "Selected " << currentlySelected.size() << "/" << territoriesThisRound << " territories";
    }
    return QString::fromStdString(messageStream.str());
}

/**
 * @brief Sets territoriesThisRound to the appropriate next value.  This will be one more than the previous
 * value until there are not enough territories left to support this rate.  At that point it is the maximum
 * number of territories each player can choose while maintaining an even distribution.
 */

void TerritorySelection::updateTerritoriesThisRound() {
    int numAvailTerritories = NullPlayer::getNullPlayer()->territoryList->size();
    int numPlayers = Player::allPlayers.size();
    unsigned int availPerPlayer = numAvailTerritories / numPlayers;

    qDebug() << numAvailTerritories << "territories / " << numPlayers << "players"
             << " = " << availPerPlayer << "each";
    if(++territoriesThisRound > availPerPlayer) {
        territoriesThisRound = availPerPlayer;
    }
    if(territoriesThisRound == 0){
        clearAutoSelecting();
        emit selectionPhaseOver();
    }
}

/**
 * @brief Changes ownership of all selected territories and deselects them.
 */
void TerritorySelection::territorySelectionAccepted() {
    list<Territory*>::iterator i;
    for(i = currentlySelected.begin(); i != currentlySelected.end(); ++i) {
        Territory* currentTerritory = (*i);
        currentTerritory->changeOwnershipTo(MainWindow::currentPlayer);
    }
    MainWindow::currentTurn->setPlayerComplete(MainWindow::currentPlayer);
    mapPix->deselectAll();
    emit acceptedTerritories();
}

/**
 * @brief Prepares initial values for the Territory selection phase.  Should only be called once.
 */
void TerritorySelection::initTerritorySelection() {
    MainWindow::gameState = MainWindow::IN_TERRITORYSELECTION;
    initialTerritoriesPerPlayer = Territory::allTerritoryList.size() / Player::allPlayers.size();
    territorySelectionRound = 1;
    territoriesThisRound = 1;
}

/**
 * @brief Setter for mapPix.
 * @param mapPix The map image to use.
 */
void TerritorySelection::setMapImage(MapImage* mapPix)
{
    this->mapPix = mapPix;
}

void TerritorySelection::autoSelectChosen()
{
    QPushButton* autoButton = this->findChild<QPushButton*>("autoButton");
    autoButton->setEnabled(false);
    autoSelecting.push_back(MainWindow::currentPlayer);
    MainWindow::currentPlayer->isAI = true;

    mapPix->deselectAll();
    emit autoSelectStart();
}

void TerritorySelection::clearAutoSelecting()
{
    Player* currentPlayer;
    while(autoSelecting.size() > 0) {
        currentPlayer = autoSelecting.front();
        autoSelecting.pop_front();
        currentPlayer->isAI = false;
    }
}
