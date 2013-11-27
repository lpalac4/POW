#include<QGraphicsScene>
#include <QPointF>
#include <QPolygonF>
#include <QGraphicsPolygonItem>
#include <QPainter>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <vector>
#include <list>

#include "playerindicator.h"
#include "player.h"
#include "nullplayer.h"
#include "mapeditor.h"

using namespace std;

PlayerIndicator::PlayerIndicator(list<Player*> players, QObject *parent) :
    QGraphicsScene(parent)
{
    currentPlayer = 0;
    highlightedPlayer = -1;

    qreal height, partWidth, fullWidth;
    height = LENGTH_OF_SIDES * MapEditor::ROOT_THREE / 2;
    partWidth = LENGTH_OF_SIDES / 2;
    fullWidth = LENGTH_OF_SIDES;
    QPointF left(-fullWidth, 0);
    QPointF topLeft(-partWidth, -height);
    QPointF topRight(partWidth, -height);
    QPointF right(fullWidth, 0);
    QPointF bottomRight(partWidth, height);
    QPointF bottomLeft(-partWidth, height);
    QPointF origin(0, 0);

    QPolygonF poly;





    list<QPolygonF> triangles;

    poly << origin << topLeft << topRight << origin;
    triangles.push_back(poly);
    poly.clear();

    poly << origin << topRight << right << origin;
    triangles.push_back(poly);
    poly.clear();

    poly << origin << right << bottomRight << origin;
    triangles.push_back(poly);
    poly.clear();

    poly << origin << bottomRight << bottomLeft << origin;
    triangles.push_back(poly);
    poly.clear();

    poly << origin << bottomLeft << left << origin;
    triangles.push_back(poly);
    poly.clear();

    poly << origin << left << topLeft << origin;
    triangles.push_back(poly);

    Player* playerPointer;
    QGraphicsPolygonItem* polyItem;

    while(!triangles.empty()){
        poly = triangles.front();
        triangles.pop_front();

        if(!players.empty()){
            playerPointer = players.front();
            players.pop_front();
            playerList.push_back(playerPointer);
        }else{
            playerPointer = NullPlayer::getNullPlayer();
        }
        QPen pen(QColor(playerPointer->playerColorSet->getBorderColor()));
        QBrush brush(QColor(playerPointer->playerColorSet->getFillColor()));
        polyItem = addPolygon(poly, pen, brush);
        playerTriangles.push_back(polyItem);
    }

    viewer = new QGraphicsView(this);
    viewer->setSceneRect(-partWidth * SCALE, -height * SCALE, fullWidth * SCALE, height * SCALE * 2);
}

Player* PlayerIndicator::whoseTurnIsIt(){
    return playerList.at(currentPlayer);
}

QGraphicsView* PlayerIndicator::getView(){
    return viewer;
}

void PlayerIndicator::highlightPlayer(Player* targetPlayer){
    int highlightNext = findPlayer(targetPlayer);
    if(highlightNext != -1){
        if(highlightedPlayer == highlightNext){
            return;
        }
        unhighlight();
        QGraphicsPolygonItem* polyItem = playerTriangles.at(highlightNext);
        polyItem->setScale(SCALE);
        QBrush brush(QColor(targetPlayer->playerColorSet->getHighlightColor()));
        polyItem->setBrush(brush);
        highlightedPlayer = highlightNext;
    }
}

void PlayerIndicator::unhighlight(){
    if(highlightedPlayer == -1 || highlightedPlayer >= (int)playerList.size()){
        return;
    }
    Player* targetPlayer = playerList.at(highlightedPlayer);
    QGraphicsPolygonItem* polyItem = playerTriangles.at(highlightedPlayer);
    polyItem->setScale(1);
    QBrush brush(QColor(targetPlayer->playerColorSet->getFillColor()));
    polyItem->setBrush(brush);
    highlightedPlayer = -1;
}

int PlayerIndicator::findPlayer(Player* targetPlayer){
    int playerNum;
    for(playerNum = 0; playerNum < (int)playerList.size(); playerNum++){
        if(playerList.at(playerNum) == targetPlayer){
            return playerNum;
        }
    }
    return -1;
}

void PlayerIndicator::setTurn(Player* targetPlayer){
    int nextPlayer = findPlayer(targetPlayer);
    if(nextPlayer != -1){
        incrementTurn((MAX_NUM_PLAYERS + nextPlayer - currentPlayer) % MAX_NUM_PLAYERS);
    }
}


void PlayerIndicator::incrementTurn(int increment){
    while(increment + currentPlayer < 0){
        increment += playerList.size();
    }
    currentPlayer = (increment + currentPlayer) % playerList.size();
    rotate(currentPlayer * ROTATION_STEP);
}

void PlayerIndicator::rotate(int degrees){
    for(int i = 0; i < MAX_NUM_PLAYERS; i++){
        playerTriangles.at(i)->setRotation(degrees);
    }
}
