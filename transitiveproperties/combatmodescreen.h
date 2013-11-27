/** \file combatmodescreen.h
 *  @brief Declarations for the CombatModeScreen class.
 *  @author Luis Palacios
 *  @author Andrew Ring
 */

#ifndef COMBATMODESCREEN_H
#define COMBATMODESCREEN_H

#include <QWidget>
#include <vector>
#include "ui_gamescreen.h"
#include "mainwindow.h"
#include "player.h"
#include "mapimage.h"
#include "combatResultStruct.h"

/**
 * @brief The CombatModeScreen class used to show the interface during the Combat Phase.
 *  @author Luis Palacios
 */

class MapImage;

class CombatModeScreen : public QWidget, private Ui::GameScreenUi
{
    Q_OBJECT
public:

    /**
     * @brief default constructor
     * @param parent
     */
    explicit CombatModeScreen(QWidget *parent = 0);
    /**
     * @brief Void method that updates the QLabel text at the bottom of the screen.
     */
    void updateSelectionPhaseLabel();
    /**
     * @brief Method that generates the message to be placed inside the QLabel at the bottom of the screen.
     * @return QString representing the message.
     */
    QString generateSelectionStateMessage();
    /**
     * @brief Method to set between first and second attack, should have used a bool to save space ...
     * @param attackNum is representing the between the first or second attack.
     */
    void setAttackNumber(int attackNum);
    /**
     * @brief Getter method for the number of attack for the current player.
     * @return integer representing which attack the current player is choosing.
     */
    int getAttackNumber();
    /**
     * @brief Pointer to our MapImage object.
     */
    MapImage* mapPix;
    /**
     * @brief First or second attack.
     */
    int attackNumber;
    /**
     * @brief Struct to hold a player's move.
     */
    struct moveStruct{
        Player* player;
        Territory* firstMove;
        Territory* secondMove;
    };
    /**
     * @brief Vector holding all the player's moves.
     */
    std::vector<moveStruct*>* playerMoves;
    /**
     * @brief Helper method to add to the vector container everyone's moves.
     * @param Player pointer for the player.
     */
    void addPlayerMoveStruct(Player* newPlayer);
    /**
     * @brief Helper method to add a territory a player's moveStruct.
     * @param Territory to add.
     */
    void poolMoves(Territory *selectedTerritory);
    /**
     * @brief Delete all entries in the vector container for all player's moves. Called at the end of the round.
     */
    void clearPlayerMoves();
    /**
     * @brief Run the combat algorithm on all the moves stored within our playerMoves vector.
     */
    void calculateCombat();
    /**
     * @brief Setter for the MapImage object.
     * @param Pointer to the MapImage.
     */
    void setMapPix(MapImage* mapPix);

signals:
    /**
     * @brief Signals the end of the combat phase.
     */
    void doneAttacking();
public slots:
    
};

#endif // COMBATMODESCREEN_H
