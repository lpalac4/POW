/** \file territoryselection.h
 *  @brief Declaration of TerritorySelection class.
 *  @author Andrew Ring
 */

#ifndef TERRITORYSELECTION_H
#define TERRITORYSELECTION_H

#include <QWidget>
#include <QString>
#include "ui_territoryselection.h"
#include "mainwindow.h"
#include "mapimage.h"

class MapImage;

/**
 * @brief The screen used for players to select their starting territories.
 * @author Andrew Ring
 */
class TerritorySelection : public QWidget, private Ui::TerritorySelectionUi
{
private:
    Q_OBJECT

    /**
     * @brief The number of territories each player should start with.
     */
    int initialTerritoriesPerPlayer;
    /**
     * @brief Which round of territory selection is currently happening.
     */
    int territorySelectionRound;
    /**
     * @brief The image containing the map.
     */
    MapImage* mapPix;

    std::list<Player*> autoSelecting;

    QString generateSelectionStateMessage();


public:
    explicit TerritorySelection(QWidget *parent = 0);
    void updateTerritoriesThisRound();

    /**
     * @brief The number of territories each Player should select in the current round.
     */
    static unsigned int territoriesThisRound;
    /**
     * @brief A list of all territories which are currently selected.
     */
    static std::list<Territory*> currentlySelected;

    void setMapImage(MapImage* mapPix);
    void initTerritorySelection();
    void clearAutoSelecting();
    
signals:
    /**
     * @brief Signifies that all selection is complete.
     */
    void selectionPhaseOver();
    /**
     * @brief Signifies that the current Player has accepted this turn's territories.
     */
    void acceptedTerritories();

    void autoSelectStart();
    
public slots:
    void territorySelectionAccepted();
    void updateSelectionPhaseLabel();
    void autoSelectChosen();
    
};

#endif // TERRITORYSELECTION_H
