#ifndef PLAYERINDICATOR_H
#define PLAYERINDICATOR_H

#include <QObject>
#include <QGraphicsScene>
#include <QPolygonF>
#include <QGraphicsPolygonItem>
#include <QGraphicsView>
#include <list>
#include <vector>

#include "player.h"
#include "mapeditor.h"

using namespace std;

class PlayerIndicator : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PlayerIndicator(list<Player*> players, QObject *parent = 0);
    const static int LENGTH_OF_SIDES = 40;
    const static int MAX_NUM_PLAYERS = 6;
    const static int ROTATION_STEP = -60;
    const static double SCALE = 1.3;
    Player* whoseTurnIsIt(void);
    QGraphicsView* getView(void);


private:
    int currentPlayer;
    int highlightedPlayer;
    QGraphicsView* viewer;
    vector<QGraphicsPolygonItem*> playerTriangles;
    vector<Player*> playerList;
    void rotate(int degrees);
    int findPlayer(Player* targetPlayer);

signals:
    
public slots:
    void highlightPlayer(Player* targetPlayer);
    void unhighlight(void);
    void setTurn(Player* targetPlayer);
    void incrementTurn(int increment = 1);
};

#endif // PLAYERINDICATOR_H
