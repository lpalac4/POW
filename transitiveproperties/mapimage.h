/**
 *  @brief MapImage is the class representing the interactive map object in most screens.
 *  @author Luis Palacios, Andrew Ring, Joseph Katzin
 */

#ifndef MAPIMAGE_H
#define MAPIMAGE_H
#include <QGraphicsItem>
#include <QPainter>
#include <QImage>
#include <QWidget>
#include <vector>
#include <list>
#include <unistd.h>
#include <QDebug>
#include <set>

#include "mainwindow.h"
#include "territory.h"
#include "combatprocessor.h"
#include "territoryselection.h"
#include "combatResultStruct.h"

class MainWindow;
class Territory;

class MapImage: public QGraphicsObject{
private:
    Q_OBJECT

    /**
     * @brief Constant used for combat animation.
     */
    static const unsigned int NUM_FLASHES = 5;
    /**
     * @brief Constant used for combat animation.
     */
    static const unsigned long FLASH_DELAY = 20000; //in microseconds

public:

    /**
     * @brief Constructor taking a pointer to both a QImage and the container QMainWindow
     * @param map
     * @param parentW
     */
    MapImage(QImage *map, MainWindow *parentW);
    ~MapImage();
    /**
     * @brief Holds a pointer to the parent window
     */
    MainWindow* parentWindow;
    /**
     * @brief Holds a pointer to the map object drawn by MapCreator/MapEditor
     */
    QImage* drawnMap;
    /**
     * @brief Used to test performance, not used in the current build
     *  @deprecated
     */
    QImage originalMap;
    /**
     * @brief Pointer to a selected territory on the map
     */
    Territory* currentSelection;
    /**
     * @brief Pointer storing a point in pixels where a touchEvent occurs.
     */
    QPointF* touchPixel;
    /**
     * @brief Floating point value used to scale the image.
     */
    float scaleFactor;
    /**
     * @brief Value stored for the relative size of the map currentl compared to its original rect dimensions.
     */
    float currentScaleFactor;
    /**
     * @brief Floating point value used to offSet map from its original point.
     *  @deprecated currently using QGraphicsView panning feature with QGraphicsItem::itemIsMovable flag.
     */
    float horizontalOffset;
    /**
     * @brief Floating point value used to offSet map from its original point.
     *  @deprecated currently using QGraphicsView panning feature with QGraphicsItem::itemIsMovable flag.
     */
    float verticalOffset;
    /**
     * @brief Boolean value used to keep track of transport process.
     */
    bool sourceSelected;

    /**
     * @brief Overwritten call to get size of map necessary in order to inherit from QGraphicsPixmapItem.
     * @return QRect with the dimensions of the map.
     */
    QRectF boundingRect() const;
    /**
     * @brief Overwritten sceneEvent() to catch interactions with the map.
     * @param event
     * @return true if handled, false if not
     */
    bool sceneEvent(QEvent *event);
    /**
     * @brief In order to inherit from QGraphicsPixmapItem, the paint() event needs to be overwritten.
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /**
     * @brief Method used to confirm a successful touchEvent was handled and the proper results occur
     *  depending on the state of the game.
     * @param The selected territory.
     */
    void touchTriggered(Territory* selectedTerritory);
    /**
     * @brief After successful touchEvents() have been handled, during the combat phase this method will call the combat algorithm.
     * @param disputedTerritory
     * @param Vector of players taking part in the battle with the defender in pos(0).
     * @return Winner
     */
    Player *runCombat(Territory* disputedTerritory, std::vector<Player*>* attackers);
    /**
     * @brief Method used to display a helpful flash animation to distinguish winners and losers.
     * @param resultSet
     */
    void displayCombatResults(std::vector<combatResultStruct*>* resultSet);

    void radialTerritoryUpdate(Territory* center);
    /**
     * @brief Checks if a territory is valid, not NULL.
     * @param if true, then emits a signal to update the drawn territory.
     */
    void updateTerritory(Territory* t);

    void clearSelection();
    /**
     * @brief Flashing animation that sets the boolean isSelected in a territory between true/false to
     *  perform the flashing animation.
     * @param resultSet
     */
    void flashTerritoriesOnOff(std::vector<combatResultStruct*>* resultSet);
signals:

    /**
     * @brief Signal emitted after a territory has its boolean isSelected value changed
     * @param selection is the territory in question
     */
    void territoryNeedsUpdate(Territory* selection);
    /**
     * @brief Overloaded territoryNeedsUpdate with a container to update a set of territories
     * @param all is the set of territories in question
     */
    void territoryNeedsUpdate(std::set<Territory*> all);
    /**
     * @brief Signal emitted when a maximum number of territories have been selected for a given player
     *  during the territory selection phase
     */
    void maxIsSelected();
    /**
     * @brief Signal emitted when a player has finished selecting territories to attack
     */
    void playerFinishedAttacks();
    /**
     * @brief Signal emitted to make the transport screen aware a source territory has been selected
     * @param source
     */
    void selectSource(Territory* source);
    /**
     * @brief Signal emitted to inform the transport screen aware a destination territory has been selected
     * @param destination
     */
    void selectDestination(Territory* destination);

public slots:
    void redrawMapImage(QImage* map);
    void deselectAll();
    /**
     * @brief highlightWithinRadius Highlights territories within a certain distance from a particular territory.
     * @param source The territory to consider distances from.
     * @param distance The distance to use.
     */
    void highlightWithinRadius(Territory* source, int distance = 10000);
    /**
     * @brief unhighlightAll Unhighlights all highlighted territories.
     */
    void unhighlightAll();
    void resetSource();

};

#endif // MAPIMAGE_H
