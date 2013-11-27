/** \file mainwindow.h
 *  @brief MainWindow class is the highest scope class in the game used to maintain the game cycle and ui elements.
 *  @author Luis Palacios, Andrew Ring
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUiLoader>
#include <QFile>
#include <QImage>
#include <QGraphicsView>
#include <ui_mainwindow.h>
#include <mapcreator.h>
#include <QImage>
#include <QtGui>
#include <maptest.h>
#include <testingflags.h>
#include <sstream>
#include <QMessageBox>
#include <QStackedWidget>

#include "player.h"
#include "mapimage.h"
#include "territory.h"
#include "mapeditor.h"
#include "nullplayer.h"
#include "list"
#include "playerturnstatus.h"
#include "playerindicator.h"
#include "playerindicatorview.h"
#include "playerresourceswidget.h"
#include "production.h"
#include "mainmenu.h"
#include "gamesetup.h"
#include "gameoptions.h"
#include "loadprogressscreen.h"
#include "territoryselection.h"
#include "combatmodescreen.h"
#include "productionscreen.h"
#include "foreignaffairsmenu.h"
#include "transportmenu.h"
#include "winscreen.h"
#include "transportmenuoptions.h"

namespace Ui {
    class MainWindow;

}
class QUiLoader;
class QFile;
class MapImage;
class LoadProgressScreen;
class TerritorySelection;
class CombatModeScreen;
class ProductionScreen;
class ForeignAffairsMenu;
class TransportMenu;


class MainWindow : public QMainWindow
{
private:

    Q_OBJECT
    /**
     * @brief Helper function used to dynamically load ui files.
     * @param fileName
     */
    void loadUI(const char* fileName);
    /**
     * @brief TerritorySelection helper function used to set text labels.
     * @return
     */
    QString generateSelectionStateMessage();
    /**
     * @brief Updates number of territories to be selected for current round during the territory selection phase.
     */
    void updateTerritoriesThisRound();
    /**
     * @brief Pointer to the winning player.
     * @return
     */
    Player *someoneWins();
    /**
     * @brief Stack containing all UI widgets in the game.
     */
    QStackedWidget* widgetStack;
    /**
     * @brief Keeps the value for the number of cities to win condition.
     */
    unsigned int citiesToWin;

    bool aiStatuses[6];

public:
    /**
     * @brief Widget for the main menu screen.
     */
    MainMenu* mainMenu;
    /**
     * @brief Widget for the game setup screen with selections for the number of players in the game,
     *  the parallel/series game mode options.
     */
    GameSetup* gameSetup;
    /**
     * @brief Widget containing options for the game map, including resource/water levels and size of the map.
     */
    GameOptions* gameOptions;
    /**
     * @brief Widget pointer for the territory selection phase of the game.
     */
    TerritorySelection* territorySelection;
    /**
     * @brief Widget pointer for the combat phase of the game.
     */
    CombatModeScreen* combatModeScreen;
    /**
     * @brief Widget pointer for the production phase of the game.
     */
    ProductionScreen* productionScreen;
    TransportMenu* transportMenuScreen;
    LoadProgressScreen* loadProgressScreen;
    TransportMenuOptions* transportOptionsScreen;

    /**
     * @brief Widget for the win screen with color background matching the winning player's color.
     */
    WinScreen* winScreen;

    /**
     * @brief Maintains player pointer to the current player.
     */
    static Player* currentPlayer;
    /**
     * @brief Maintains a pointer to the current turn status for all players.
     */
    static PlayerTurnStatus* currentTurn;

    /**
     * @brief Number of players in the game.
     */
    int numberOfPlayers;

    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };
    /**
     * @brief The GameStates enum holds an enum for each of the different phases/screens in the game.
     */
    enum GameStates {
        IN_SPLASH,              // 0
        IN_MENU,                // 1
        IN_GAMESETUP,           // 2
        IN_GAMEOPTIONS,         // 3
        IN_GAMELOADSCREEN,      // 4
        IN_TERRITORYSELECTION,  // 5
        IN_COMBATMODE,          // 6
        IN_PRODUCTION,          // 7
        IN_TRANSPORT           // 8
    };

    /**
     * @brief Default constructor
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);
    /**
     * @brief Default destructor
     */
    virtual ~MainWindow();
    /**
     * @brief Maintains the enum for the current game state.
     */
    static GameStates gameState;

    QUiLoader loader;
    QWidget* formWidget;
    QFile file;
    /**
     * @brief Contains a pointer to the mapCreator class.
     */
    MapCreator* mapCreator;
    /**
     * @brief Holds a pointer to the view used in all windows to display the map.
     */
    //QGraphicsView* mapView;
    //QGraphicsView* territorySelectView;
    /**
     * @brief Holds the MapImage object of the map.
     */
    MapImage* mapPix;
    //QGraphicsScene* gameScreenScene;
    //QGraphicsScene* territorySelectScene;
    /**
     * @brief Holds the list of players in the game.
     */
    list<Player*> playersInGame;
    /**
     * @brief Used to change the current player pointer will reverse direction when all players
     *  have had their turn.
     */
    int incrementAmount;
    /**
     * @brief Holds a pointer to the scene used to display the hexagon which shows whose turn it is.
     */
    PlayerIndicator* playerIndicator;

    /**
     * @brief Boolean value used to start game over.(currently not fully implemented)
     */
    bool hotSeatEnabled;
    /**
     * @brief Will take QGraphicsView pointers and initialize them with the map and hexagon images currently used.
     */
    void generateMap();

    Ui::MainWindow *ui;

    bool isCurrentView(QWidget* screen);

public slots:

    void selectSound(); //not implemented
    void selectHelp(); //not implemented
    void selectQuit(); //not implemented
    /**
     * @brief Slot to show the mainmenu.ui screen.
     */
    void openMainMenu();
    /**
     * @brief Slot to setup a local game in hotseat mode.(Default)
     */
    void selectLocalGame();
    /**
     * @brief Slot to setup online game. (currently disabled)
     */
    void selectOnlineGame();
    /**
     * @brief Slot to setup android options. (currently disabled)
     */
    void selectOptions();
    /**
     * @brief Shows the gamesetup.ui widget putting it at the top of the stack.
     */
    void openGameSetup();
    void clickPlayer(); //not implemented
    void selectLinear(); //not implemented
    void selectParallel(); //not implemented

    /**
     * @brief Shows the gameoptions.ui widget by putting it at the top of the stack.
     */
    void openGameOptions();
    /**
     * @brief Sets the number of cities needed for victory.
     */
    void cityVictoryOption();
    void resourceOption(); //not implemented
    void waterAreaOption(); //not implemented
    /**
     * @brief Initializes all players in the game and their colors.
     * @param playerSize
     */
    void hotSeatInit(int playerSize);
    /**
     * @brief Starts the territory selection phase.
     */
    void selectTerritories();
    /**
     * @brief Indicates all players have taken their turn using the PlayerTurnStatus class.
     */
    void endOfRound();
    /**
     * @brief Sets the current players turn flag to be taken and sets the new player to take their turn.
     */
    void updatePlayerTurn();
    /**
     * @brief Calls for the beginning of the production phase.
     */
    void beginProduction();
    void pullUpLoadScreen();
    void loadGame();
    void simulateAi();
    void pullUpForeignAffairs();
    void exitForeignAffairsScreen();
    void Transport();
    void openTransportMenu();
    void openTransportOptions();

    /**
     * @brief Starts the combat phase.
     */
    void combatMode();
    /**
     * @brief Calls for special highlighting in the hexagon object in the PlayerIndicator class.
     */
    void updatePlayerIndicatorHighlight();

private slots:
    void setPlayerNumberAndAIFlags();
};


#endif // MAINWINDOW_H
