/** \file mapimage.cpp
 *  @brief Definitions of MapImage member functions.
 *  @author Luis Palacios
 *  @author Andrew Ring
 *  @author Joseph Katzin
 */

#include "mapimage.h"

#define DISCARD_UNUNSED_PARAMETER(x) (void)x

MapImage::MapImage(QImage* map, MainWindow* parentW) : QGraphicsObject(){

    parentWindow = parentW;
    drawnMap = map;
    originalMap = *map;
    currentSelection = NULL;
    touchPixel = NULL;
    scaleFactor = 1;
    currentScaleFactor = 1;
    horizontalOffset = 0;
    verticalOffset = 0;
    sourceSelected = false;

    setAcceptTouchEvents(true);
    setFlag(QGraphicsPixmapItem::ItemIsMovable);
}

MapImage::~MapImage(){
    delete parentWindow;
    delete drawnMap;
    //From Yossi (11-25-2012) - Why is the current selection being deleted? Wouldn't that delete
    //it from the map?
    delete currentSelection;
    delete touchPixel;

}

QRectF MapImage::boundingRect() const{
    return QRectF(0,0,drawnMap->width(),drawnMap->height());
}

/**
 * @brief MapImage::sceneEvent
 * @param event
 * @return
 */
bool MapImage::sceneEvent(QEvent *event){
    QList<QTouchEvent::TouchPoint> touchPoints;

    switch(event->type()){
    case QEvent::TouchBegin:
    {   //Store initial touch location-verify that finger was panned at TouchEnd.
        QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
        touchPoints = touchEvent->touchPoints();
        if(touchPoints.count() == 1){ //Tap
            QTouchEvent::TouchPoint tapPoint = touchPoints.first();
            QTouchEvent::TouchPoint lastTapPoint = touchPoints.last();
            if(tapPoint.pos() == lastTapPoint.pos()){
                int x = tapPoint.pos().x();
                int y = tapPoint.pos().y();
                touchTriggered(parentWindow->mapCreator->getMapEditor()->getMapData()->whatDidITouch(y, x));
                return true;
            }
            break;
        }
    }
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
        touchPoints = touchEvent->touchPoints();
        if(touchPoints.count() == 2){
            //Check the ratio of the two line segments drawn from original touchPoints with the final touchPoints.
            qreal firstDistance = QLineF(touchPoints.first().pos(), touchPoints.last().pos()).length();
            qreal secondDistance = QLineF(touchPoints.first().startPos(), touchPoints.last().startPos()).length();
            if(scaleFactor < 1.2 || scaleFactor > 0.8){
                currentScaleFactor = firstDistance/secondDistance;
            //If one finger is lifted store the current scaleFactor to allow the user to put their finger back on the screen.
                if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                    scaleFactor *= currentScaleFactor;
                    currentScaleFactor = 1;
                }
            }
            setTransform(QTransform().scale(scaleFactor * currentScaleFactor,scaleFactor * currentScaleFactor));
        }
        update(boundingRect());
        return true;
        break;
    }


    default:
        break;
    }
    return QGraphicsItem::sceneEvent(event);

}

/**
 * @brief MapImage::paint
 * @param painter
 * @param option
 * @param widget
 */
void MapImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    DISCARD_UNUNSED_PARAMETER(option);
    DISCARD_UNUNSED_PARAMETER(widget);
    painter->drawImage(0,0, *drawnMap);
}

/**
 * @brief MapImage::touchTriggered
 * @param touchedTerritory
 */
void MapImage::touchTriggered(Territory* touchedTerritory){
    // No touch events while AI is running!!
    if(MainWindow::currentPlayer->isAI) {
        return;
    }

    switch(parentWindow->gameState){
    case MainWindow::IN_COMBATMODE:
        if(touchedTerritory == NULL){
            qDebug() << "bad territory" << endl;
            if(currentSelection) {
                currentSelection->selected = false;
                emit territoryNeedsUpdate(currentSelection);
                currentSelection = NULL;
            }
        }
        else if(touchedTerritory->ownedBy == MainWindow::currentPlayer) {
            qDebug() << "can't select own territory to attack";
            if(currentSelection) {
                currentSelection->selected = false;
                emit territoryNeedsUpdate(currentSelection);
                currentSelection = NULL;
            }
        }
        else if(currentSelection == NULL && touchedTerritory != NULL){
            qDebug() << "new territory" << endl;
            currentSelection = touchedTerritory;
            currentSelection->selected = true;
            emit territoryNeedsUpdate(currentSelection);
        }
        else if(currentSelection != NULL && currentSelection != touchedTerritory){
            qDebug() << "cancelled choice" << endl;
            currentSelection->selected = false;
            emit territoryNeedsUpdate(currentSelection);
            currentSelection = touchedTerritory;
            currentSelection->selected = true;
            emit territoryNeedsUpdate(currentSelection);
        }
        else if(currentSelection != NULL && currentSelection == touchedTerritory){
            qDebug() << "confirmed choice" << endl;
            currentSelection->selected = false;
            emit territoryNeedsUpdate(currentSelection);
            parentWindow->combatModeScreen->poolMoves(currentSelection);
            currentSelection = NULL;
        }
        break;
    case MainWindow::IN_TERRITORYSELECTION:

        if(touchedTerritory == NULL){
            qDebug() << "bad territory" << endl;
        }
        else if(!(touchedTerritory->ownedBy == (Player*)NullPlayer::getNullPlayer())) {
            qDebug() << "tried to select owned territory";
            // do nothing
        }
        else {
            bool alreadySelected = false;
            bool maxSelected = (TerritorySelection::currentlySelected.size() == TerritorySelection::territoriesThisRound);
            list<Territory*>::iterator i;
            for(i = TerritorySelection::currentlySelected.begin(); !alreadySelected && i != TerritorySelection::currentlySelected.end(); ++i) {
                if(touchedTerritory == (*i)) {
                    alreadySelected = true;
                }
            }
            if(alreadySelected) {
                qDebug() << "deselect territory";
                TerritorySelection::currentlySelected.remove(touchedTerritory);
                touchedTerritory->selected = false;
                emit territoryNeedsUpdate(touchedTerritory);
            } else if(maxSelected) {
                qDebug() << "selection at max, can't select more";
                // popup message saying max selected here!
            } else {
                qDebug() << "selected a territory";
                TerritorySelection::currentlySelected.push_back(touchedTerritory);
                touchedTerritory->selected = true;
                emit territoryNeedsUpdate(touchedTerritory);
                if(touchedTerritory != TerritorySelection::currentlySelected.front()) {
                    qDebug() << "Distance between first selection and this is: "
                                << Territory::getDistance(TerritorySelection::currentlySelected.front(), touchedTerritory);
                }

                maxSelected = (TerritorySelection::currentlySelected.size() == TerritorySelection::territoriesThisRound);
                if(maxSelected) {
                    emit maxIsSelected();
                }
            }
        }
        break;
    case MainWindow::IN_PRODUCTION:
        if(touchedTerritory && touchedTerritory->ownedBy != MainWindow::currentPlayer) {
            break;
        }
        if(touchedTerritory == currentSelection) {
            break;
        }
        if(currentSelection) {
            currentSelection->selected = false;
            emit territoryNeedsUpdate(currentSelection);
        }
        currentSelection = touchedTerritory;
        if(currentSelection) {
            currentSelection->selected = true;
            emit territoryNeedsUpdate(currentSelection);
        }
        break;
    case MainWindow::IN_TRANSPORT:
        if(touchedTerritory == NULL){
            //unhighlightAll();
            //currentSelection = NULL;
            return;
        }else if(currentSelection != NULL){
            //currentSelection->selected = false;
            //emit territoryNeedsUpdate(currentSelection);
            if(touchedTerritory == currentSelection){
                //currentSelection = NULL;
                //unhighlightAll();
                return;
            }
        }
        if(touchedTerritory->ownedBy == parentWindow->playerIndicator->whoseTurnIsIt()){
            if(!sourceSelected){
                sourceSelected = true;
                unhighlightAll();
                currentSelection = touchedTerritory;
                currentSelection->selected = true;
                emit territoryNeedsUpdate(currentSelection);
                emit selectSource(currentSelection);
            }else if(touchedTerritory->selected){
                unhighlightAll();
                currentSelection->selected = true;
                emit territoryNeedsUpdate(currentSelection);
                touchedTerritory->selected = true;
                emit territoryNeedsUpdate(touchedTerritory);
                emit selectDestination(touchedTerritory);
            }
        }else{
            //unhighlightAll();
            //currentSelection = NULL;
            return;
        }
    default:
        break;
    }
}



void MapImage::radialTerritoryUpdate(Territory *center)
{
    if(!center) {
        qDebug() << "radialTerritoryUpdate recieved NULL";
        return;
    }

    emit territoryNeedsUpdate(center);

    std::list<Territory*>::iterator i;
    for(i = center->adjacentTerritories->begin(); i != center->adjacentTerritories->end(); ++i) {
        emit territoryNeedsUpdate((*i));
    }
}

void MapImage::updateTerritory(Territory *t)
{
    if(t)
        emit territoryNeedsUpdate(t);
}

/**
 * @brief MapImage::redrawMapImage
 * @param map
 * @brief Deselects all territories currently selected.  This uses the MainWindow::currentlySelected list
 * of which territories are selected.
 */
void MapImage::deselectAll()
{
    while(Territory* currentTerritory = TerritorySelection::currentlySelected.front()) {
        currentTerritory->selected = false;
        TerritorySelection::currentlySelected.pop_front();
        emit territoryNeedsUpdate(currentTerritory);
    }
}

void MapImage::clearSelection() {
    if(currentSelection) {
        currentSelection->selected = false;
        emit territoryNeedsUpdate(currentSelection);
        currentSelection = NULL;
    }
}

/**
 * @brief MapImage::highlightWithinRadius Highlights all territories belonging to the current player
 * within <tt>distance</tt> of <tt>source</tt>.
 * @param source The territory to consider distance from.
 * @param distance The radius to highlight within.
 */
void MapImage::highlightWithinRadius(Territory* source, int distance){
    if(source == NULL){
        return;
    }
    unhighlightAll();
    std::set<Territory*> toHighlight;
    for(std::list<Territory*>::iterator iter = MainWindow::currentPlayer->territoryList->begin();
        iter != MainWindow::currentPlayer->territoryList->end(); iter++){
        if(Territory::getDistance(source, *iter) <= distance && source != *iter){
            (*iter)->selected = true;
            toHighlight.insert((*iter));
        }
    }
    if(toHighlight.size()>0) emit territoryNeedsUpdate(toHighlight);
}

/**
 * @brief MapImage::unhighlightAll Unhighlights all highlighted territoiries.
 * @note After this function call, currentSelection will be NULL.
 */
void MapImage::unhighlightAll(){
    std::set<Territory*> toUnhighlight;
    for(std::list<Territory*>::iterator iter = Territory::allTerritoryList.begin();
        iter != Territory::allTerritoryList.end(); iter++){
        if((*iter)->selected){
            (*iter)->selected = false;
            toUnhighlight.insert((*iter));
        }
    }
    if(toUnhighlight.size()>0) emit territoryNeedsUpdate(toUnhighlight);
}

void MapImage::resetSource(){
    sourceSelected = false;
    if(currentSelection && currentSelection->selected) {
        currentSelection->selected = false;
        emit territoryNeedsUpdate(currentSelection);
    }
    currentSelection = NULL;
}

void MapImage::redrawMapImage(QImage *map){
    DISCARD_UNUNSED_PARAMETER(map);

    setTransform(QTransform().scale(scaleFactor * currentScaleFactor,scaleFactor * currentScaleFactor));
    update(boundingRect());
    qDebug() << "in MapImage::redrawMapImage()" << endl;
    qApp->processEvents();

    // Banish unused warning!
    map = map;
}

/**
 * @brief Highlights all Territory areas for FLASH_DELAY amount of time, then unhighlights them.
 * @param resultSet The results from combat.
 */
void MapImage::flashTerritoriesOnOff(vector<combatResultStruct*>* resultSet)
{
    unsigned int i;
    set<Territory*> all;

    for(i = 0; i < resultSet->size(); ++i) {
        combatResultStruct* results = resultSet->at(i);
        results->contestedArea->selected = true;
        if(results->contestedArea) all.insert(results->contestedArea);
    }
    if(!all.empty()) emit territoryNeedsUpdate(all);
    usleep(FLASH_DELAY);
    for(i = 0; i < resultSet->size(); ++i) {
        combatResultStruct* results = resultSet->at(i);
        results->contestedArea->selected = false;
    }
    if(!all.empty()) emit territoryNeedsUpdate(all);
}

/**
 * @brief Displays combat as group of flashing Territory areas on the map. Adjusts ownership to
 * winner of combat.
 * @param resultSet The results from combat.
 */
void MapImage::displayCombatResults(vector<combatResultStruct*>* resultSet)
{
    unsigned int i;

    // set owner to defender
    for(i = 0; i < resultSet->size(); ++i) {
        combatResultStruct* results = resultSet->at(i);
        results->contestedArea->changeOwnershipTo(results->defender);
    }

    // flash as defender
    for(i = 0; i < NUM_FLASHES; ++i) {
        flashTerritoriesOnOff(resultSet);
        usleep(FLASH_DELAY);
    }

    // set owner as winner
    for(i = 0; i < resultSet->size(); ++i) {
        combatResultStruct* results = resultSet->at(i);
        results->contestedArea->changeOwnershipTo(results->winner);
    }

    // flash as winner
    for(i = 0; i < NUM_FLASHES; ++i) {
        flashTerritoriesOnOff(resultSet);
    }

}

/**
 * @brief MapImage::runCombat
 * @param disputedTerritory
 * @deprecated
 */
Player* MapImage::runCombat(Territory *disputedTerritory, vector<Player*>*attackers){

    vector<Player*>* contestingPlayers = attackers;
    Player* winner = combat::resolve(contestingPlayers, disputedTerritory);

    disputedTerritory->selected = true;
    emit territoryNeedsUpdate(disputedTerritory);
    disputedTerritory->selected = false;
    emit territoryNeedsUpdate(disputedTerritory);
    disputedTerritory->selected = true;
    emit territoryNeedsUpdate(disputedTerritory);
    disputedTerritory->selected = false;
    emit territoryNeedsUpdate(disputedTerritory);

    if(winner != contestingPlayers->front()){
        disputedTerritory->changeOwnershipTo(winner);
        emit territoryNeedsUpdate(disputedTerritory);
        disputedTerritory->selected = true;
        emit territoryNeedsUpdate(disputedTerritory);
        disputedTerritory->selected = false;
        emit territoryNeedsUpdate(disputedTerritory);
        disputedTerritory->selected = true;
        emit territoryNeedsUpdate(disputedTerritory);
        disputedTerritory->selected = false;
        emit territoryNeedsUpdate(disputedTerritory);
    }else{
        disputedTerritory->selected = true;
        emit territoryNeedsUpdate(disputedTerritory);
        disputedTerritory->selected = false;
        emit territoryNeedsUpdate(disputedTerritory);
        disputedTerritory->selected = true;
        emit territoryNeedsUpdate(disputedTerritory);
        disputedTerritory->selected = false;
        emit territoryNeedsUpdate(disputedTerritory);
    }

    update(boundingRect());
    return winner;

}
