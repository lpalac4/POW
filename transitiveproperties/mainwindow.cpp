/** \file mainwindow.cpp
 *  @brief Definitions for MainWindow member functions.
 *  @author Luis Palacios
 *  @author Andrew Ring
 */

#include "mainwindow.h"

Player* MainWindow::currentPlayer;
MainWindow::GameStates MainWindow::gameState;
PlayerTurnStatus* MainWindow::currentTurn;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
    gameState = IN_SPLASH;
    mapCreator = NULL;
    mapPix = NULL;
    incrementAmount = 1;
    hotSeatEnabled = false;
    citiesToWin = 0;

    widgetStack = new QStackedWidget();

    mainMenu = new MainMenu();
    gameSetup = new GameSetup();
    gameOptions = new GameOptions();
    territorySelection = new TerritorySelection();
    combatModeScreen = new CombatModeScreen();
    productionScreen = new ProductionScreen();
    loadProgressScreen = new LoadProgressScreen();
    winScreen = new WinScreen();
    transportMenuScreen = new TransportMenu();
    transportOptionsScreen = new TransportMenuOptions();

    widgetStack->addWidget(loadProgressScreen);
    widgetStack->addWidget(mainMenu);
    widgetStack->addWidget(gameSetup);
    widgetStack->addWidget(gameOptions);
    widgetStack->addWidget(territorySelection);
    widgetStack->addWidget(combatModeScreen);
    widgetStack->addWidget(productionScreen);
    widgetStack->addWidget(transportMenuScreen);
    widgetStack->addWidget(transportOptionsScreen);

    widgetStack->setMinimumSize(qApp->desktop()->size());
    widgetStack->setMaximumSize(qApp->desktop()->size());

    setCentralWidget(widgetStack);
    openMainMenu();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete mapCreator;


}

//Universal slots
//Make a call to disable sound
void MainWindow::selectSound(){
}
//Brings up a widget with app/developer information
void MainWindow::selectHelp(){
}
//Bring up confirmation widget
void MainWindow::selectQuit(){
}

//Receive signal from splashScreen timing object
//or game quit/back calls
void MainWindow::openMainMenu(){
    gameState = IN_MENU;
    formWidget = mainMenu;

    QPushButton* localGame = mainMenu->findChild<QPushButton*>("localButton");
    QObject::connect(localGame,SIGNAL(released()),this,SLOT(selectLocalGame()));

    QPushButton* onlineGame = mainMenu->findChild<QPushButton*>("onlineButton");
    QObject::connect(onlineGame,SIGNAL(released()),this,SLOT(selectOnlineGame()));
    onlineGame->setEnabled(false);

    QPushButton* options = mainMenu->findChild<QPushButton*>("optionsButton");
    QObject::connect(options,SIGNAL(released()),this,SLOT(selectOptions()));
    options->setEnabled(false);

    QPushButton* quit = mainMenu->findChild<QPushButton*>("quitButton");
    QObject::connect(quit,SIGNAL(released()),this,SLOT(selectQuit()));
    quit->setEnabled(false);

    widgetStack->setCurrentWidget(mainMenu);
}

void MainWindow::selectLocalGame(){
    openGameSetup();
}
void MainWindow::selectOnlineGame(){
}
void MainWindow::selectOptions(){
}

//Game setup slots
void MainWindow::openGameSetup(){
    gameState = IN_GAMESETUP;
    formWidget = gameSetup;

    QPushButton* continueButtonSelect = gameSetup->findChild<QPushButton*>("continueButton");
    QObject::connect(continueButtonSelect,SIGNAL(pressed()),this,SLOT(setPlayerNumberAndAIFlags()));

    widgetStack->setCurrentWidget(gameSetup);
}
void MainWindow::setPlayerNumberAndAIFlags(){

    numberOfPlayers = gameSetup->findChild<QComboBox*>("numberOfPlayersSelect")->currentIndex() + 2;

    for(int i=0; i<numberOfPlayers; i++){
        if(gameSetup->toggleButtons[i]->text().compare("AI") == 0){
            aiStatuses[i] = true;
        }else{
            aiStatuses[i] = false;
        }
    }

    openGameOptions();
}

void MainWindow::clickPlayer(){
}
void MainWindow::selectLinear(){
}
void MainWindow::selectParallel(){
}
//Game options slots
void MainWindow::openGameOptions(){
    gameState = IN_GAMEOPTIONS;
    formWidget = gameOptions;

    QPushButton* continueButtonSelect = gameOptions->findChild<QPushButton*>("continueButton");
    QObject::connect(continueButtonSelect,SIGNAL(pressed()), this,SLOT(pullUpLoadScreen()));

    widgetStack->setCurrentWidget(gameOptions);
}
void MainWindow::cityVictoryOption(){


}
void MainWindow::resourceOption(){

}
void MainWindow::waterAreaOption(){

}

void MainWindow::beginProduction()
{
    gameState = IN_PRODUCTION;
    productionScreen->setMapImage(mapPix);
    productionScreen->updateProductionResourceDisplay();
    productionScreen->updateProductionButtons();

    formWidget = productionScreen;
    QObject::connect(productionScreen, SIGNAL(productionFinished()), this, SLOT(updatePlayerTurn()), Qt::UniqueConnection);
    generateMap();
    widgetStack->setCurrentWidget(formWidget);
}

/**
 * @brief pullUpLoadScreen called by a signal from the gameOptions screen signal emitted by the continue button.
 *  will set the current widget on the stack to the loadScreen and connect its signal finishedLoading to
 *  selectTerritories.
 */
void MainWindow::pullUpLoadScreen(){
    gameState = IN_GAMELOADSCREEN;
    formWidget = loadProgressScreen;
    widgetStack->setCurrentWidget(loadProgressScreen);
    qApp->processEvents();
    loadGame();

}



/**
 * @brief hotSeatInit will initialize all players depending on the number selected in the Game Setup screen or
 * default value of 1 player.
 * @param playerSize
 */
void MainWindow::hotSeatInit(int playerSize){
    if(hotSeatEnabled == false){
        hotSeatEnabled = true;
    }
    else{
        playersInGame.clear();
    }

    for(int i = 0; i < playerSize; i++){
        Player* newPlayer = new Player(aiStatuses[i]);
        //newPlayer->name = "player" + i;
        combatModeScreen->addPlayerMoveStruct(newPlayer);
        playersInGame.push_back(newPlayer);

        switch(i){
        case 0:
            newPlayer->playerColorSet = new Color(qRgb(18,43,2), qRgb(39,134,9), qRgb(183,255,92));
            break;
        case 1:
            newPlayer->playerColorSet = new Color(qRgb(93,0,128), qRgb(188,100,220), qRgb(212,191,220));
            break;
        case 2:
            newPlayer->playerColorSet = new Color(qRgb(80,89,18), qRgb(22,219,176), qRgb(212,219,161));
            break;
        case 3:
            newPlayer->playerColorSet = new Color(qRgb(154,16,45), qRgb(219,22,87), qRgb(219,120,153));
            break;
        case 4:
            newPlayer->playerColorSet = new Color(qRgb(245,38,0), qRgb(255,196,11), qRgb(255,228,56));
            break;
        case 5:
            newPlayer->playerColorSet = new Color(qRgb(13,15,64), qRgb(35,42,173), qRgb(83,92,252));
            break;
        default:
            qDebug() << "Messed up somewhere!!!" << endl;
            break;
        }
//        newPlayer->name = "Transitive Properties";
    }
    combatModeScreen->clearPlayerMoves();
    playerIndicator = new PlayerIndicator(playersInGame);
    currentPlayer = playerIndicator->whoseTurnIsIt();
    Player::initializeAllForeignAffairs();
    ForeignAffairsMenu* foreignAffairsMenu = ForeignAffairsMenu::getForeignAffairsMenu();
    widgetStack->addWidget(foreignAffairsMenu);

}

/**
 * @brief Loads Territory selection phase.
 */
void MainWindow::selectTerritories() {
    gameState = IN_TERRITORYSELECTION;
    territorySelection->initTerritorySelection();
    formWidget = territorySelection;
    territorySelection->setMapImage(mapPix);

    QPushButton* resetButton = territorySelection->findChild<QPushButton*>("resetButton");
    QObject::connect(resetButton, SIGNAL(released()), mapPix, SLOT(deselectAll()), Qt::UniqueConnection);
    QObject::connect(mapPix,SIGNAL(territoryNeedsUpdate(Territory*)),territorySelection,SLOT(updateSelectionPhaseLabel()), Qt::UniqueConnection);

    QPushButton* acceptButton = territorySelection->findChild<QPushButton*>("acceptButton");
    QObject::connect(acceptButton, SIGNAL(released()), territorySelection, SLOT(territorySelectionAccepted()), Qt::UniqueConnection);
    //Connect when player finishes choosing territories with updating of player pointer and playerIndicator
    QObject::connect(territorySelection,SIGNAL(acceptedTerritories()), this,SLOT(updatePlayerTurn()), Qt::UniqueConnection);
    QObject::connect(territorySelection,SIGNAL(selectionPhaseOver()),this,SLOT(combatMode()), Qt::UniqueConnection);
    QObject::connect(territorySelection, SIGNAL(autoSelectStart()), this, SLOT(simulateAi()), Qt::UniqueConnection);

    currentPlayer = playerIndicator->whoseTurnIsIt();
    generateMap();
    widgetStack->setCurrentWidget(territorySelection);

    if(currentPlayer->isAI){
        simulateAi();
    }

}

/**
 * @brief endOfRound called when when all players have taken their
 * turns in combat phase, territory selection phase, production.
 */
void MainWindow::endOfRound(){
    incrementAmount *= -1;
    currentTurn->clearTurn();

    switch(gameState){

    case IN_TERRITORYSELECTION:
        territorySelection->updateTerritoriesThisRound();
        break;

    case IN_COMBATMODE:
    {
        combatModeScreen->updateSelectionPhaseLabel();
        combatModeScreen->calculateCombat();
        combatModeScreen->clearPlayerMoves();

        Player* winner = someoneWins();
        if(winner) {
            winScreen->setWinner(winner);
            setCentralWidget(winScreen);
        }

        beginProduction();
    }
        break;

    case IN_PRODUCTION:
    {
        std::list<Territory*>* toUpdate = Player::allPlayersCollectResources();
        while(toUpdate->size()) {
            Territory* t = toUpdate->front();
            toUpdate->pop_front();
            mapPix->updateTerritory(t);
        }
        delete(toUpdate);

        Transport();
        break;
    }
    case IN_TRANSPORT:
        combatMode();
        break;

    default:
        break;

    }
}

Player* MainWindow::someoneWins()
{
    if(!citiesToWin) {
        QSpinBox* cityCount = gameOptions->findChild<QSpinBox*>("cityCount");
        citiesToWin = (unsigned int)cityCount->value();
    }
    unsigned int highestNumberOfCities = 0;
    bool unique = false;
    bool winByDomination = false;
    Player* winner = NULL;
    unsigned int totalNumPlayerOwnedTerritories = Territory::allTerritoryList.size() - NullPlayer::getNullPlayer()->territoryList->size();

    std::list<Player*>::iterator playerItr;
    for(playerItr = Player::allPlayers.begin(); playerItr != Player::allPlayers.end(); ++playerItr) {
        Player* currentPlayer = (*playerItr);
        unsigned int numCities = currentPlayer->numberOfCities();
        if(currentPlayer->territoryList->size() == totalNumPlayerOwnedTerritories) {
            winner = currentPlayer;
            winByDomination = true;
            break;
        }
        if(numCities > highestNumberOfCities) {
            highestNumberOfCities = numCities;
            unique = true;
            winner = currentPlayer;
        } else if(numCities == highestNumberOfCities) {
            unique = false;
            winner = NULL;
        }
    }

    if(!winByDomination && (highestNumberOfCities < citiesToWin || !unique))
        winner = NULL;

    return winner;
}

/**
 * @brief Updates the current player pointer and playerIndicator.  Will call endOfRound() if all players have
 * had their turn.  Resets the attack number in CombatModeScreen, skips any Players with 0 territories in
 * non-TerritorySelectionScreen phases.  Updates textLabels and will call simulateAI() if new player is an AI.
 */
void MainWindow::updatePlayerTurn(){
    mapPix->clearSelection();

    currentTurn->setPlayerComplete(currentPlayer);

    if(currentTurn->allPlayersComplete()){
        endOfRound();

    }

    if(gameState == IN_COMBATMODE) {
        if(combatModeScreen->getAttackNumber() >= 1)
            combatModeScreen->setAttackNumber(0);
    }

    if(currentTurn->getPlayerComplete(currentPlayer)) {
        playerIndicator->incrementTurn(incrementAmount);
    }

    currentPlayer = playerIndicator->whoseTurnIsIt();


    //Skip as many times as neccessary until a new player with some territories is found
    while(currentPlayer->territoryList->empty() && gameState != IN_TERRITORYSELECTION){
        currentTurn->setPlayerComplete(currentPlayer);
        playerIndicator->incrementTurn(incrementAmount);

        if(currentTurn->allPlayersComplete()){
            endOfRound();
        }

        if(currentTurn->getPlayerComplete(currentPlayer)){
            playerIndicator->incrementTurn(incrementAmount);
        }

        currentPlayer = playerIndicator->whoseTurnIsIt();
    }

    if(gameState == IN_COMBATMODE){
        combatModeScreen->updateSelectionPhaseLabel();
    }

    if(gameState == IN_TERRITORYSELECTION){
        territorySelection->updateSelectionPhaseLabel();
    }

    if(gameState == IN_PRODUCTION) {
        productionScreen->updateProductionResourceDisplay();
        productionScreen->updateProductionButtons();
    }

    if(gameState == IN_TRANSPORT){
        mapPix->unhighlightAll();
        mapPix->currentSelection = NULL;
    }

    if(currentPlayer->isAI)
        simulateAi();

}

/**
 * @brief Loads a ui file form into the formWidget.
 * @param fileName The path to the ui file to be loaded.
 */
void MainWindow::loadUI(const char* fileName) {
    file.setFileName(fileName);
    file.open(QFile::ReadOnly);
    formWidget = loader.load(&file, this);
    file.close();
}

/**
 * @brief loadGame creates the mapCreator object that holds the map data structure as well as the
 * initiate the Qimage from that data structure.
 */
void MainWindow::loadGame(){
    int width = 20;
    int height = 40;

    mapCreator = new MapCreator(width,height);
    hotSeatInit(numberOfPlayers);
    currentTurn = new PlayerTurnStatus();

    mapCreator->makeMeAMap();
    QImage* drawnMap = mapCreator->getMapEditor()->getMapImage();
    mapPix = new MapImage(drawnMap, this);
    QObject::connect(mapPix,SIGNAL(territoryNeedsUpdate(Territory*)), mapCreator->getMapEditor(), SLOT(redraw(Territory*)), Qt::UniqueConnection);
    QObject::connect(mapPix, SIGNAL(territoryNeedsUpdate(std::set<Territory*>)), mapCreator->getMapEditor(), SLOT(redraw(std::set<Territory*>)), Qt::UniqueConnection);
    QObject::connect(mapCreator->getMapEditor(),SIGNAL(imageUpdated(QImage*)), mapPix, SLOT(redrawMapImage(QImage*)), Qt::UniqueConnection);

    //hotSeatInit(numberOfPlayers);
    //currentTurn = new PlayerTurnStatus();
    selectTerritories();
}


void MainWindow::combatMode(){
    if(gameState == IN_TERRITORYSELECTION) {
        std::list<Territory*>* toUpdate = Player::allPlayersCollectResources();
        while(toUpdate->size()) {
            Territory* t = toUpdate->front();
            toUpdate->pop_front();
            mapPix->updateTerritory(t);
        }
        delete(toUpdate);
    }

    gameState = IN_COMBATMODE;
    formWidget = combatModeScreen;

    generateMap();

    QPushButton* continueButton = formWidget->findChild<QPushButton*>("continueButton");
    QObject::connect(continueButton,SIGNAL(released()), this, SLOT(updatePlayerTurn()), Qt::UniqueConnection);
    QObject::connect(mapPix, SIGNAL(playerFinishedAttacks()), this, SLOT(updatePlayerTurn()), Qt::UniqueConnection);
    QObject::connect(mapPix, SIGNAL(territoryNeedsUpdate(Territory*)), this, SLOT(updatePlayerIndicatorHighlight()), Qt::UniqueConnection);
    QObject::connect(combatModeScreen, SIGNAL(doneAttacking()), this,SLOT(updatePlayerTurn()), Qt::UniqueConnection);
    combatModeScreen->setMapPix(mapPix);

    combatModeScreen->setAttackNumber(0);
    widgetStack->setCurrentWidget(combatModeScreen);
}

/**
 * @brief simulateAi will take the current player and run our AI to automatically select territories, run combat
 * and run production.  Set off by the currentPlayer.isAI flag check in updatePlayerTurn().
 */
void MainWindow::simulateAi(){
    switch(gameState){
    case IN_TERRITORYSELECTION:
    {
        std::list<Territory*>::iterator itr;
        std::list<Territory*> aiSelectionChoices = currentPlayer->claimChoice(territorySelection->territoriesThisRound);

        for(itr = aiSelectionChoices.begin(); itr != aiSelectionChoices.end(); ++itr){
            //mapPix->touchTriggered((*itr));
            Territory* currentTerritory = (*itr);
            assert(currentTerritory->ownedBy == NullPlayer::getNullPlayer());
            currentTerritory->changeOwnershipTo(currentPlayer);
            mapPix->updateTerritory(currentTerritory);
        }
        break;

    }
    case IN_COMBATMODE:
    {
        Territory* aiChoiceAttack;
        if(combatModeScreen->getAttackNumber() == 0)
            aiChoiceAttack = currentPlayer->attackChoice(true);

        if(combatModeScreen->getAttackNumber() == 1)
            aiChoiceAttack = currentPlayer->attackChoice(false);

        combatModeScreen->poolMoves(aiChoiceAttack);
        break;
    }
    case IN_PRODUCTION:
    {
        productionItemAndLocation nextAction = currentPlayer->buildChoice();

        while(nextAction.location) {
            mapPix->currentSelection = nextAction.location;
            productionScreen->productionCheck(nextAction.item, true);
            mapPix->currentSelection = NULL;

            nextAction = currentPlayer->buildChoice();
        }

        break;
    }

    case IN_TRANSPORT:
        //No transport AI will be implemented.  Skip Turn outside switch
        break;

    default:
        break;
    }

    updatePlayerTurn();
}

void MainWindow::pullUpForeignAffairs(){
    ForeignAffairsMenu* foreignAffairsScreen = ForeignAffairsMenu::getForeignAffairsMenu();
    QObject::connect(foreignAffairsScreen,SIGNAL(foreignAffairsDone()), this, SLOT(exitForeignAffairsScreen()),Qt::UniqueConnection);
    widgetStack->setCurrentWidget(foreignAffairsScreen);
    foreignAffairsScreen->inFocus = true;
}

void MainWindow::exitForeignAffairsScreen(){
    ForeignAffairsMenu* foreignAffairsScreen = (ForeignAffairsMenu*)widgetStack->currentWidget();
    foreignAffairsScreen->inFocus = false;
    widgetStack->setCurrentWidget(formWidget);
}

void MainWindow::Transport(){
    gameState = IN_TRANSPORT;
    formWidget = transportMenuScreen;
    //transportMenuScreen->setMapEditor(mapCreator->getMapEditor(), mapPix);
    generateMap();
    //set signals if needed
    QObject::connect(transportMenuScreen, SIGNAL(finished()), this, SLOT(updatePlayerTurn()),Qt::UniqueConnection);
    QObject::connect(transportMenuScreen, SIGNAL(finished()), mapPix, SLOT(unhighlightAll()),Qt::UniqueConnection);
    QObject::connect(transportMenuScreen, SIGNAL(deselectAll()), mapPix, SLOT(unhighlightAll()),Qt::UniqueConnection);
    QObject::connect(mapPix, SIGNAL(selectSource(Territory*)), transportMenuScreen, SLOT(setSourceTerritory(Territory*)),Qt::UniqueConnection);
    QObject::connect(mapPix, SIGNAL(selectDestination(Territory*)), transportMenuScreen,
                     SLOT(setDestinationTerritory(Territory*)),Qt::UniqueConnection);
    QObject::connect(transportMenuScreen, SIGNAL(chosenShipmentFromSource(Territory*,int)), mapPix,
                     SLOT(highlightWithinRadius(Territory*,int)),Qt::UniqueConnection);
    QObject::connect(transportMenuScreen, SIGNAL(sourceHasBeenSet(Territory*)), transportOptionsScreen,
                     SLOT(setButtons(Territory*)),Qt::UniqueConnection);
    QObject::connect(transportOptionsScreen, SIGNAL(chosenMateriel(materiel)), transportMenuScreen,
                     SLOT(acceptTransportManifest(materiel)),Qt::UniqueConnection);
    QObject::connect(transportMenuScreen, SIGNAL(sourceHasBeenSet(Territory*)), this, SLOT(openTransportOptions()),Qt::UniqueConnection);
    QObject::connect(transportOptionsScreen, SIGNAL(chosenMateriel(materiel)), this, SLOT(openTransportMenu()),Qt::UniqueConnection);
    QObject::connect(transportOptionsScreen, SIGNAL(goBack()), this, SLOT(openTransportMenu()),Qt::UniqueConnection);
    QObject::connect(transportMenuScreen, SIGNAL(deselectAll()), mapPix, SLOT(resetSource()),Qt::UniqueConnection);
    QObject::connect(transportMenuScreen, SIGNAL(finished()), mapPix, SLOT(resetSource()),Qt::UniqueConnection);
    QObject::connect(transportOptionsScreen, SIGNAL(goBack()), mapPix, SLOT(resetSource()), Qt::UniqueConnection);
    QObject::connect(transportOptionsScreen, SIGNAL(goBack()), transportMenuScreen, SLOT(resetButtons()), Qt::UniqueConnection);

    widgetStack->setCurrentWidget(transportMenuScreen);
}

void MainWindow::openTransportMenu(){
    widgetStack->setCurrentWidget(transportMenuScreen);
}

void MainWindow::openTransportOptions(){
    widgetStack->setCurrentWidget(transportOptionsScreen);
}

void MainWindow::updatePlayerIndicatorHighlight()
{
    if(!mapPix->currentSelection) {
        return;
    }

    if(mapPix->currentSelection->selected) {
        playerIndicator->highlightPlayer(mapPix->currentSelection->ownedBy);
    } else {
        playerIndicator->unhighlight();
    }
}

/**
 * @brief generateMap will create the map object along with its scene and view using a Qimage
 * drawn by the graphics engine.
 * @param map
 */
void MainWindow::generateMap(){

    QGraphicsView* view = formWidget->findChild<QGraphicsView*>("mapView");
    view->setAttribute(Qt::WA_AcceptTouchEvents);
    view->setRenderHint(QPainter::Antialiasing);
    view->setAlignment(Qt::AlignCenter);

    PlayerIndicatorView* playerHex = formWidget->findChild<PlayerIndicatorView*>("playerHex");
    playerHex->setScene(playerIndicator);
    playerHex->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    playerHex->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if(gameState != IN_COMBATMODE){
        QObject::connect(playerHex,SIGNAL(indicatorTouched()), this, SLOT(pullUpForeignAffairs()),Qt::UniqueConnection);
    }
    playerHex->show();
    QGraphicsScene* gameScene = new QGraphicsScene();
    gameScene->addItem(mapPix);

    view->setScene(gameScene);

    mapPix->parentWindow = this;
    view->centerOn(mapPix);
    view->show();
}

/**
 * @brief MainWindow::isCurrentView
 * @param screen
 * @return
 * @deprecated
 */
bool MainWindow::isCurrentView(QWidget *screen)
{
    if(widgetStack->currentWidget() == screen) {
        return true;
    }
    else {
        return false;
    }
}
