#include "combatmodescreen.h"

CombatModeScreen::CombatModeScreen(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    playerMoves = new vector<moveStruct*>();

}

void CombatModeScreen::updateSelectionPhaseLabel() {

    if(MainWindow::gameState == MainWindow::IN_COMBATMODE){
        QLabel* phaseLabel = this->findChild<QLabel*>("phaseLabel");

        if(!MainWindow::currentTurn->getPlayerComplete(MainWindow::currentPlayer)){
            phaseLabel->setText(generateSelectionStateMessage());
        }

        if(MainWindow::currentTurn->allPlayersComplete()){
            phaseLabel->setText(generateSelectionStateMessage());
        }

    }
}

QString CombatModeScreen::generateSelectionStateMessage() {
    stringstream messageStream;

    if(MainWindow::gameState == MainWindow::IN_COMBATMODE){
        if(!MainWindow::currentTurn->allPlayersComplete()){
            messageStream << "Please confirm attack # " << attackNumber + 1;
        }else{
            messageStream << "Conflicts have spread throughout the land...";
        }
    }

    return QString::fromStdString(messageStream.str());
}


void CombatModeScreen::setAttackNumber(int attackNum){
    this->attackNumber = attackNum;
}

int CombatModeScreen::getAttackNumber(){
    return attackNumber;
}

void CombatModeScreen::addPlayerMoveStruct(Player *newPlayer){
    struct moveStruct* newMoves = new struct moveStruct;
    newMoves->player = newPlayer;
    playerMoves->push_back(newMoves);
}

/**
 * @brief poolMoves will store an attack for the current player to be processed at the end of the round.
 * @param selectedTerritory
 */
void CombatModeScreen::poolMoves(Territory *selectedTerritory){
    struct moveStruct* currentPlayerMoves;

    for(unsigned int i = 0;  i < playerMoves->size(); i++){
        struct moveStruct* playerRunner = playerMoves->at(i);
        if(playerRunner->player == MainWindow::currentPlayer)
            currentPlayerMoves = playerRunner;
    }

    if(selectedTerritory == NULL){
        if(attackNumber == 0){
            currentPlayerMoves->firstMove = NULL;
            attackNumber++;
            updateSelectionPhaseLabel();
        }
        else if(attackNumber == 1){
            currentPlayerMoves->secondMove = NULL;
            emit doneAttacking(); //updatePlayerTurn();
        }
    }
    else{
        if(attackNumber == 0){
            currentPlayerMoves->firstMove = selectedTerritory;
            attackNumber++;
            updateSelectionPhaseLabel();
        }

        else if(attackNumber == 1){
            currentPlayerMoves->secondMove = selectedTerritory;
            emit doneAttacking(); //updatePlayerTurn();
        }
    }

}

/**
 * @brief clearPlayerMoves will clear any moves a player has taken. To be called at the
 * end of the round.
 */
void CombatModeScreen::clearPlayerMoves(){
    for(unsigned int i = 0; i < playerMoves->size(); i++){
        struct moveStruct* currentPlayerMoves = playerMoves->at(i);
        currentPlayerMoves->firstMove = NULL;
        currentPlayerMoves->secondMove = NULL;
    }
    attackNumber = 0;
    mapPix->deselectAll();
}

void CombatModeScreen::calculateCombat(){
    struct moveStruct* currentPlayersAttacks;
    vector<Player*>* attackers = new vector<Player*>();
    unsigned int i;
    vector<combatResultStruct*> firstRoundResults;
    vector<combatResultStruct*> secondRoundResults;

    for(i = 0; i < playerMoves->size(); i++){
        currentPlayersAttacks = playerMoves->at(i);
        Territory* attackedTerr = currentPlayersAttacks->firstMove;

        if(attackedTerr != NULL){
            attackers->push_back(attackedTerr->ownedBy);
            attackers->push_back(currentPlayersAttacks->player);
            currentPlayersAttacks->firstMove = NULL;

            for(unsigned int j = i+1; j < playerMoves->size(); j++){
                struct moveStruct* additionalAttacks = playerMoves->at(j);
                if(additionalAttacks->firstMove == attackedTerr){
                    attackers->push_back(additionalAttacks->player);
                    additionalAttacks->firstMove = NULL;
                }

            }
            //set loser players' 2nd attacks equal to NULL
            Player* winner = combat::resolve(attackers,attackedTerr);
            struct combatResultStruct* results = new combatResultStruct;
            results->contestedArea = attackedTerr;
            results->defender = attackers->at(0);
            results->winner = winner;
            firstRoundResults.push_back(results);

            struct moveStruct* tmpPlayerMoves;
            for(unsigned int k = 1; k < attackers->size(); k++){
                Player* currentPlayer = attackers->at(k);
                for(unsigned int l = 0; l < playerMoves->size(); l++){
                    tmpPlayerMoves = playerMoves->at(l);
                    if(currentPlayer == tmpPlayerMoves->player){
                        if(winner != tmpPlayerMoves->player)
                            tmpPlayerMoves->secondMove = NULL;
                    }
                }
            }

        }
        attackers->clear();
    }

    for(i = 0; i < firstRoundResults.size(); ++i) {
        combatResultStruct* results = firstRoundResults.at(i);
        results->contestedArea->changeOwnershipTo(results->winner);
    }

    for(i = 0; i < playerMoves->size(); i++){
        currentPlayersAttacks = playerMoves->at(i);
        Territory* attackedTerr = currentPlayersAttacks->secondMove;

        if(attackedTerr != NULL){
            attackers->push_back(attackedTerr->ownedBy);
            attackers->push_back(currentPlayersAttacks->player);
            currentPlayersAttacks->secondMove = NULL;

            for(unsigned int j = i+1; j < playerMoves->size(); j++){
                struct moveStruct* additionalAttacks = playerMoves->at(j);
                if(additionalAttacks->secondMove == attackedTerr){
                    attackers->push_back(additionalAttacks->player);
                    additionalAttacks->secondMove = NULL;
                }
            }

            Player* winner = combat::resolve(attackers,attackedTerr);
            struct combatResultStruct* results = new combatResultStruct;
            results->contestedArea = attackedTerr;
            results->defender = attackers->at(0);
            results->winner = winner;
            secondRoundResults.push_back(results);

            // hide warning
            winner=winner;

            attackers->clear();

        }
    }

    mapPix->displayCombatResults(&firstRoundResults);
    mapPix->displayCombatResults(&secondRoundResults);
}

void CombatModeScreen::setMapPix(MapImage* mapPix)
{
    this->mapPix = mapPix;
}
